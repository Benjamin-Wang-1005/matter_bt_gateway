
//------------------------------------------------------------------------------------------
//                                                                                                         
// Copyright(c) 2023 Link of Things Inc. All rights reserved.                                 
//                                                                                                         
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//
// Implement for BlueDim WiFi to BT Mesh Gateway
// Support
//      * BlueDim APP
//      * Telink Mesh
//      * MTK MT7931 WiFi
//      * Gizwits open cloud
//      * MTK Matter
// Auth: Benjamin
// E-mail: olddragonwang@gmail.com
// Date: 2023-07-27
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// File functions - Gateway UART realtive functions implement
//      - use Top_uart1 as AT communication port
//      - use UART DMA mode
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "FreeRTOS.h"
//#include "hal.h"
#include "hal_uart.h"
#include "serial_port.h"
#include "hal_pinmux_define.h"
//#include "hal_platform.h"
#include "task_def.h"
#include "lot_uart.h"
//#include "timers.h"
#include "lot_wifi.h"
#include "lot_rtos.h"
//#include "lot_timer.h"
#include "nvdm.h"

//-----------------------------------------------------------------------------------------
//HAL_UART_0 -> com33_uart
//HAL_UART_1 -> Top_uart0
//HAL_UART_2 -> Top_uart1
//HAL_UART_3 -> DSP_uart
//HAL_UART_MAX -> The total number of UART ports (invalid UART port number)
//-----------------------------------------------------------------------------------------


extern uint32_t g_lot_counter_1s;

static volatile uint32_t _receive_notice = 0;
static volatile uint32_t _send_notice = 0;
static char rx_vfifo_buffer[GATEWAY_UART_RX_VFIFO_BUFFER_SIZE] __attribute__ ((section(".noncached_zidata")));
static char tx_vfifo_buffer[GATEWAY_UART_TX_VFIFO_BUFFER_SIZE] __attribute__ ((section(".noncached_zidata")));

typedef struct
{
    uint8_t mode[2];
    uint8_t SSID[WIFI_MAX_LENGTH_OF_SSID+1];
    uint8_t PW[WIFI_LENGTH_PASSPHRASE+1];
    uint8_t SSID_len[4];
    uint8_t PW_len[4];
    uint8_t auth_mode[4];
    uint8_t encry_type[4];
} s_wifi_config_t;
s_wifi_config_t s_wifi_config;

void _s_wifi_config_init(void)
{
    uint8_t read_buf[WIFI_MAX_LENGTH_OF_SSID+1] = {0};
    uint8_t str[4];
    uint32_t len;
    len = sizeof(read_buf);
    nvdm_read_data_item("common", "OpMode", read_buf, &len);
    memcpy(s_wifi_config.mode, read_buf, len);
    if(s_wifi_config.mode[0] == '1'){
        strcpy(str, "STA");
    }else if(s_wifi_config.mode[0] == '2'){
        strcpy(str, "AP");
    }
    len = sizeof(read_buf);
    memset(read_buf, 0x00, WIFI_MAX_LENGTH_OF_SSID+1);
    nvdm_read_data_item(str, "Ssid", read_buf, &len);
    memcpy(s_wifi_config.SSID, read_buf, len);
    len = sizeof(read_buf);
    memset(read_buf, 0x00, WIFI_MAX_LENGTH_OF_SSID+1);
    nvdm_read_data_item(str, "SsidLen", read_buf, &len);
    memcpy(s_wifi_config.SSID_len, read_buf, len);
    len = sizeof(read_buf);
    memset(read_buf, 0x00, WIFI_MAX_LENGTH_OF_SSID+1);
    nvdm_read_data_item(str, "WpaPsk", read_buf, &len);
    memcpy(s_wifi_config.PW, read_buf, len);
    len = sizeof(read_buf);
    memset(read_buf, 0x00, WIFI_MAX_LENGTH_OF_SSID+1);
    nvdm_read_data_item(str, "WpaPskLen", read_buf, &len);
    memcpy(s_wifi_config.PW_len, read_buf, len);
    len = sizeof(read_buf);
    memset(read_buf, 0x00, WIFI_MAX_LENGTH_OF_SSID+1);
    nvdm_read_data_item(str, "AuthMode", read_buf, &len);
    memcpy(s_wifi_config.auth_mode, read_buf, len);
    len = sizeof(read_buf);
    memset(read_buf, 0x00, WIFI_MAX_LENGTH_OF_SSID+1);
    nvdm_read_data_item(str, "EncrypType", read_buf, &len);
    memcpy(s_wifi_config.encry_type, read_buf, len);
}

void _my_itos(uint8_t *in_str, uint32_t num)
{
    uint8_t i = 0, len = 0, j;
    uint8_t buf[10] = {0};
    do{
        buf[i++] = (num % 10) + '0';
        len++;
        num /= 10; 
    }while(num > 0);
    i--;
    for(j=0; j<len; j++){
        in_str[j] = buf[i--];
    }
}

uint32_t _my_strlen(uint8_t *input)
{
    uint32_t count = 0;
    do{
        if((input[count] == 0) || (input[count] == '\n') || (input[count] == '\r')){
            if(input[count] != 0){
                input[count] = 0;
            }
            return count++;
        }
        count++;
    }while(count < 64);
    return 0;
}

static void _gateway_bt_uart_callback(hal_uart_callback_event_t status, void *user_dat)
{
    if(status == HAL_UART_EVENT_READY_TO_WRITE)
        _send_notice = 1;
    else if(status == HAL_UART_EVENT_READY_TO_READ)
        _receive_notice = 1;
}

void gateway_uart_send_data(uint8_t *buf, uint32_t len)
{
    uint32_t _sen_cnt;
    char *pbuf;
    pbuf = buf;
    do{
        _sen_cnt = hal_uart_send_dma(GATEWAY_BT_UART, pbuf, len);
        len -= _sen_cnt;
        pbuf += _sen_cnt;
        if(len >0)
        {
            while(!_send_notice);
            _send_notice = 0;
        }
    }while(len != 0);
}

uart_data_t gateway_uart_rx_buffer;
void _reset_uart_buffer(void)
{
    gateway_uart_rx_buffer.len = 0;
    memset(gateway_uart_rx_buffer.data, 0x0, GATEWAY_UART_RX_VFIFO_BUFFER_SIZE);
}

static cmd_parse_t _parse_cmd(uint8_t *cmd, uint8_t maxLen)
{
    cmd_parse_t result;
    uint8_t i;

    result.cmd = UNKNOWN;
    result.extended = eFALSE;

    if (strncmp(cmd, (uint8_t*)"AT+SSID=", 8) == 0) {
        result.cmd = SSID;
        result.offset = 8;
    } else if (strncmp(cmd, (uint8_t*)"AT+MODE=", 8) == 0) {
        result.cmd = MODE;
        result.offset = 8;
    } else if (strncmp(cmd, (uint8_t*)"AT+AUTH=", 8) == 0) {
        result.cmd = AUTH_MODE;
        result.offset = 8;
    } else if (strncmp(cmd, (uint8_t*)"AT+ENCRYPT=", 11) == 0) {
        result.cmd = ENC_TYPE;
        result.offset = 11;
    } else if (strncmp(cmd, (uint8_t*)"AT+PW=", 6) == 0) {
        result.cmd = PW;
        result.offset = 6;
    }else if (strncmp(cmd, (uint8_t*)"AT+RELOAD", 9) == 0) {
        result.cmd = RELOAD;
        result.offset = 9;
    } else {
    	result.cmd = UNKNOWN;
        return result;
    }

    if (cmd[result.offset] == '?') {
        result.query = eTRUE;
        return result;
    }
    result.query = eFALSE;
    return result;
}


    

void _processCmd(void)
{
    uint8_t *cmd = 0;
    cmd_parse_t parse;
    uint8_t i;
    uint8_t temp_buf[128] = {0};
    //uint8_t read_buf[36] = {0};
    uint32_t str_len;
    uint32_t temp;

    for(i=0; i<gateway_uart_rx_buffer.len; i++){
        if(gateway_uart_rx_buffer.data[i] == 'A' && gateway_uart_rx_buffer.data[i+1] == 'T'){
            cmd = (uint8_t*)&(gateway_uart_rx_buffer.data[i]);
            break;
        }
    }
    if (cmd == 0) {
        return;
    }

    parse = _parse_cmd(cmd, gateway_uart_rx_buffer.len);
    cmd += parse.offset;
    switch(parse.cmd)
    {
        case MODE:
            if(parse.query == eTRUE){
                //temp = sizeof(read_buf);
                //nvdm_read_data_item("common", "OpMode", read_buf, &temp);
                if(s_wifi_config.mode[0] == "1"){
                  str_len = (uint32_t)sprintf(temp_buf, "OP_mode: STA\n");  
                }else if(s_wifi_config.mode[0] == '2'){
                  str_len = (uint32_t)sprintf(temp_buf, "OP_mode: AP\n");  
                }
                gateway_uart_send_data(temp_buf, (uint32_t)str_len);
                return;
            }
            if(cmd[0] == '0'){        //unknow mode
                str_len = (uint32_t)sprintf(temp_buf, "OP_mode: 1 STA / 2 AP\n");
            }else if(cmd[0] == '1'){  //WIFI_MODE_STA_ONLY
                s_wifi_config.mode[0] = '1';
                str_len = (uint32_t)sprintf(temp_buf, "OP_mode: STA\n");
            }else if(cmd[0] == '2'){  //WIFI_MODE_AP_ONLY
                s_wifi_config.mode[0] = '2';
                str_len = (uint32_t)sprintf(temp_buf, "OP_mode: AP\n");
            }else{
                str_len = (uint32_t)sprintf(temp_buf, "OP_mode: 1 STA / 2 AP\n");  
            }
            gateway_uart_send_data(temp_buf, (uint32_t)str_len);
        break;

        case SSID:
            if(parse.query == eTRUE){
                //temp = sizeof(read_buf);
                //nvdm_read_data_item("STA", "Ssid", read_buf, &temp);
                str_len = (uint32_t)sprintf(temp_buf, "SSID:%s\n", s_wifi_config.SSID);
                gateway_uart_send_data(temp_buf, (uint32_t)str_len);
                return; 
            }
            memset(s_wifi_config.SSID, 0x00, WIFI_MAX_LENGTH_OF_SSID);
            str_len = _my_strlen(cmd);
            if(str_len != 0){
                memcpy(s_wifi_config.SSID, cmd, str_len);
                _my_itos(s_wifi_config.SSID_len, str_len);
                str_len = (uint32_t)sprintf(temp_buf, "SSID: %s\n", s_wifi_config.SSID);
            } else {
                str_len = (uint32_t)sprintf(temp_buf, "SSID can't NULL!\n"); 
            }
            gateway_uart_send_data(temp_buf, (uint32_t)str_len);
        break;

        case AUTH_MODE:
            if(cmd[0] == '0'){
                temp = 0;
                _my_itos(s_wifi_config.auth_mode, temp);
                str_len = (uint32_t)sprintf(temp_buf, "AUTH_MODE: Open\n");
            } else if(cmd[0] == '4'){
                temp = 4;
                _my_itos(s_wifi_config.auth_mode, temp);
                str_len = (uint32_t)sprintf(temp_buf, "AUTH_MODE: WPA_PSK\n");
            } else if(cmd[0] == '7'){
                temp = 7;
                _my_itos(s_wifi_config.auth_mode, temp);
                str_len = (uint32_t)sprintf(temp_buf, "AUTH_MODE: WPA2_PSK\n");
            } else if(cmd[0] == '9'){
                temp = 9;
                _my_itos(s_wifi_config.auth_mode, temp);
                str_len = (uint32_t)sprintf(temp_buf, "AUTH_MODE: WPA/WPA2 PSK\n");
            } else if((cmd[0] == '1') && (cmd[1] == '1')){
                temp = 11;
                _my_itos(s_wifi_config.auth_mode, temp);
                str_len = (uint32_t)sprintf(temp_buf, "AUTH_MODE: WPA3_PSK\n");
            } else {
                str_len = (uint32_t)sprintf(temp_buf, "AUTH_MODE: Not Support\n");
            }
            gateway_uart_send_data(temp_buf, (uint32_t)str_len);
        break;
        case ENC_TYPE:
            if(cmd[0] == '0'){
                temp = 0;
                _my_itos(s_wifi_config.encry_type, temp);
                str_len = (uint32_t)sprintf(temp_buf, "ENCRYPE_TYPE: WEP\n");
            } else if(cmd[0] == '4'){
                temp = 4;
                _my_itos(s_wifi_config.encry_type, temp);
                str_len = (uint32_t)sprintf(temp_buf, "ENCRYPE_TYPE: TKIP\n");
            } else if(cmd[0] == '6'){
                temp = 6;
                _my_itos(s_wifi_config.encry_type, temp);
                str_len = (uint32_t)sprintf(temp_buf, "ENCRYPE_TYPE: AES\n");
            } else if(cmd[0] == '8'){
                temp = 8;
                _my_itos(s_wifi_config.encry_type, temp);
                str_len = (uint32_t)sprintf(temp_buf, "ENCRYPE_TYPE: TKIP or AES\n");
            } else {
                str_len = (uint32_t)sprintf(temp_buf, "ENCRYPE_TYPE: Not Support\n");
            }
            gateway_uart_send_data(temp_buf, (uint32_t)str_len);
        break;

        case PW:
            if(parse.query == eTRUE){
                //temp = sizeof(read_buf);
                //nvdm_read_data_item("STA", "WpaPsk", read_buf, &temp);
                str_len = (uint32_t)sprintf(temp_buf, "Password:%s\n", s_wifi_config.PW);
                gateway_uart_send_data(temp_buf, (uint32_t)str_len);
                return; 
            }
            memset(s_wifi_config.PW, 0x00, WIFI_LENGTH_PASSPHRASE);
            str_len = _my_strlen(cmd);
            if(str_len != 0){
                memcpy(s_wifi_config.PW, cmd, str_len);
                _my_itos(s_wifi_config.PW_len, str_len);
                str_len = (uint32_t)sprintf(temp_buf, "Password: %s\n", s_wifi_config.PW);
            } else {
                str_len = (uint32_t)sprintf(temp_buf, "Password can't NULL!\n"); 
            }
            gateway_uart_send_data(temp_buf, (uint32_t)str_len);
        break;

        case RELOAD:
            if(!((s_wifi_config.mode[0] == '1') || (s_wifi_config.mode[0] == '2'))){
                LOGI("Mode fail\n");
                return;
            }
            if(strlen(s_wifi_config.SSID) == 0){
                LOGI("SSID is NULL\n");
                return;
            }
            if(strlen(s_wifi_config.PW) == 0){
                LOGI("Password is NULL\n");
                return;
            }
            if(strlen(s_wifi_config.auth_mode) == 0){
                LOGI("Auth Mode is 0\n");
                return;
            }
            if(strlen(s_wifi_config.encry_type) == 0){
                LOGI("Encry type is 0\n");
                return;
            }
            nvdm_write_data_item("common", "OpMode", NVDM_DATA_ITEM_TYPE_STRING, s_wifi_config.mode, 1);
            uint8_t str[4];
            if(s_wifi_config.mode[0] == '1'){        //STA
                strcpy(str, "STA");   
            }else if(s_wifi_config.mode[0] == '2'){
                strcpy(str, "AP");
            }
            temp = _my_strlen(s_wifi_config.SSID);
            nvdm_write_data_item(str, "Ssid", NVDM_DATA_ITEM_TYPE_STRING, s_wifi_config.SSID, temp);
            temp = _my_strlen(s_wifi_config.SSID_len);
            nvdm_write_data_item(str, "SsidLen", NVDM_DATA_ITEM_TYPE_STRING, s_wifi_config.SSID_len, temp);
            if(s_wifi_config.encry_type[0] != "0"){
                temp = _my_strlen(s_wifi_config.PW);
                nvdm_write_data_item(str, "WpaPsk", NVDM_DATA_ITEM_TYPE_STRING, s_wifi_config.PW, temp);
                temp = _my_strlen(s_wifi_config.PW_len);
                nvdm_write_data_item(str, "WpaPskLen", NVDM_DATA_ITEM_TYPE_STRING, s_wifi_config.PW_len, temp);
            }
            temp = _my_strlen(s_wifi_config.auth_mode);
            nvdm_write_data_item(str, "AuthMode", NVDM_DATA_ITEM_TYPE_STRING, s_wifi_config.auth_mode, temp);
            temp = _my_strlen(s_wifi_config.encry_type);
            nvdm_write_data_item(str, "EncrypType", NVDM_DATA_ITEM_TYPE_STRING, s_wifi_config.encry_type, temp);
            
            TickType_t local_tick = xTaskGetTickCount();
            LOGI("WiFi Update Reboot!\n");
            while((xTaskGetTickCount() - local_tick) < 2000);
            extern void exception_reboot(void);
            exception_reboot();
        break;

        case UNKNOWN:
            str_len = (uint32_t)sprintf(temp_buf, " AT+SSID=\n AT+MODE=\n AT+AUTH=\n AT+ENCRYPT=\n AT+PW=\n AT+RELOAD\n");
            gateway_uart_send_data(temp_buf, (uint32_t)str_len);
        break;
    }
}
void _gateway_uart_task(void)
{
    uint32_t rec_cnt;
    uint8_t i;
    uint8_t temp_buf[64];
    while(1)
    {
        
        while(_receive_notice)
        {
            rec_cnt = hal_uart_receive_dma(GATEWAY_BT_UART, temp_buf, 64);
            _receive_notice = 0;
            for(i=0; i<rec_cnt; i++){
                gateway_uart_rx_buffer.data[gateway_uart_rx_buffer.len++] = temp_buf[i];

                if(temp_buf[i]==0x00 || temp_buf[i]=='\n' || temp_buf[i]=='\r'){
                    _processCmd();
                    _reset_uart_buffer();
                }
            }
        }
        
    }
}

void gateway_bt_uart_init(void)
{
    //TimerHandle_t tHandle;
    //UART1_RX pin GPIO 42
    hal_gpio_init(HAL_GPIO_42);         //UART1_RX
    //set pin mux to UART1 RX
    hal_pinmux_set_function(HAL_GPIO_42, MT7933_PIN_42_FUNC_UART1_RX);      

    //UART1_TX pin GPIO 44
    hal_gpio_init(HAL_GPIO_44);         //UART1_TX
    //set pin mux to UART1 RX
    hal_pinmux_set_function(HAL_GPIO_44, MT7933_PIN_44_FUNC_UART1_TX); 

    //Config UART
    hal_uart_config_t uart_config;
    hal_uart_dma_config_t dma_config;
    //char buffer[64];
    //char *pbuf;
    //uint32_t left, snd_cnt, rcv_cnt;
    //char uart_prompt[] = "\nUART DMA mode begin\n";
    uart_config.baudrate = HAL_UART_BAUDRATE_115200;
    uart_config.parity = HAL_UART_PARITY_NONE;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    
    hal_uart_init(GATEWAY_BT_UART, &uart_config);

    //Config UART DMA
    dma_config.receive_vfifo_alert_size = GATEWAY_UART_RX_VFIFO_ALARM_SIZE;
    dma_config.receive_vfifo_buffer = rx_vfifo_buffer;
    dma_config.receive_vfifo_buffer_size = GATEWAY_UART_RX_VFIFO_BUFFER_SIZE;
    dma_config.receive_vfifo_threshold_size = GATEWAY_UART_RX_VFIFO_THRESHOLD_SIZE;
    dma_config.send_vfifo_buffer = tx_vfifo_buffer;
    dma_config.send_vfifo_buffer_size = GATEWAY_UART_TX_VFIFO_BUFFER_SIZE;
    dma_config.send_vfifo_threshold_size = GATEWAY_UART_TX_VFIFO_THRESHOLD_SIZE;
    hal_uart_set_dma(GATEWAY_BT_UART, &dma_config);
    hal_uart_register_callback(GATEWAY_BT_UART, _gateway_bt_uart_callback, NULL);
    //snd_cnt = hal_uart_send_dma(GATEWAY_BT_UART, uart_prompt, sizeof(uart_prompt));  
    _reset_uart_buffer();
    _s_wifi_config_init();
    xTaskCreate(_gateway_uart_task, GATEWAY_UART_TASK_NAME, GATEWAY_UART_TASK_STACK_SIZE / sizeof(portSTACK_TYPE), NULL, GATEWAY_UART_TASK_PRIO, NULL);

}
