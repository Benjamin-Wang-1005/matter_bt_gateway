
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
// File functions - Gateway UART realtive functions header file
//      - use Top_uart1 as WiFi and BT communication port
//      - use UART DMA mode
//-------------------------------------------------------------------------------------------

#ifndef __LOT_UART_H__
#define __LOT_UART_H__

#include <stdint.h>
#include "FreeRTOS.h"
#include "lot_rtos.h"


#ifdef __cplusplus
extern "C" {
#endif

#define GATEWAY_BT_UART                                     HAL_UART_2
#define GATEWAY_UART_TASK_NAME                              "UART"
#define GATEWAY_UART_TASK_STACK_SIZE                        (512 * 8)
#define GATEWAY_UART_TASK_PRIO                              TASK_PRIORITY_NORMAL
#define GATEWAY_UART_RX_VFIFO_BUFFER_SIZE                   128
#define GATEWAY_UART_TX_VFIFO_BUFFER_SIZE                   128
#define GATEWAY_UART_RX_VFIFO_ALARM_SIZE                    32
#define GATEWAY_UART_RX_VFIFO_THRESHOLD_SIZE                64
#define GATEWAY_UART_TX_VFIFO_THRESHOLD_SIZE                64
#define MAX_CMD_PARAMS                                      5
#define UNUSED_VALUE                                        0xFF

//typedef enum
//{
//    bFALSE = 0,
//    bTRUE
//} bool_t;

typedef enum {
    UNKNOWN,
    MODE,
    SSID,
    AUTH_MODE,
    ENC_TYPE,
    PW,
    RELOAD
} paramater_t;

typedef struct
{
    paramater_t cmd;
    bool_t extended;
    bool_t query;
    uint8_t offset;
} cmd_parse_t;

typedef struct {
    uint32_t len;        // data max 252
    uint8_t data[GATEWAY_UART_RX_VFIFO_BUFFER_SIZE];
} uart_data_t;

void gateway_bt_uart_init(void);
void gateway_uart_send_data(uint8_t *buf, uint32_t len);


#ifdef __cplusplus
}
#endif

#endif      /* __LOT_UART_H__ */