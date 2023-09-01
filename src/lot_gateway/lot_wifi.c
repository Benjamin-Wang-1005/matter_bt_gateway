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
// Date: 2023-08-10
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "hal.h"
#include "task.h"

#include "gl_init.h"
#include "wifi_api_ex.h"
//#include "ping.h"
#include "wifi_netif.h"
#include "lwip/sockets.h"

#include "lot_rtos.h"
#include "lot_wifi.h"
//#include "ip_addr.h"
//#include "gl_wifi_cli.h"

#define time_zone                       (8)
#define scan_frequency                  (5)



//-------------------------------------------------------------------------------------------
// File functions - WiFi initial done even handler
// 
//-------------------------------------------------------------------------------------------
int32_t lot_wifi_event_handler(wifi_event_t event,
                                    uint8_t *payload,
                                    uint32_t length)
{
    int wifi_event_handle = 0;
    uint16_t i = 0;
    switch(event){
        case WIFI_EVENT_IOT_CONNECTED:
            wifi_event_handle = 1;
            if ((length == WIFI_MAC_ADDRESS_LENGTH + 1) && (payload)){
                LOGI("[MTK Event Callback Sample]: LinkUp! CONNECTED MAC = %02x:%02x:%02x:%02x:%02x:%02x\n",
                         payload[0], payload[1], payload[2],
                         payload[3], payload[4], payload[5]);
            }else{
                LOGI("[MTK Event Callback Sample]: LinkUp!\n");
            }
            break;
        case WIFI_EVENT_IOT_SCAN_COMPLETE:
            wifi_event_handle = 1;
            if (!g_scan_list) {
                LOGI("scan list is null!\n");
                break;
            }
            for (i = 0; i < g_scan_list_size; i++) {
                LOGI("\n%-4s%-20s%-20s%-7s%-7s%-7s%-7s%-7s%-7s%-7s\n", "Ch",
                         "SSID", "BSSID", "Valid", "Hidden", "Auth", "Cipher", "RSSI",
                         "bcn", "capInfo");
                LOGI("%-4d", g_scan_list[i].channel);
                LOGI("%-20s", g_scan_list[i].ssid);
                LOGI("%02x:%02x:%02x:%02x:%02x:%02x	",
                         g_scan_list[i].bssid[0],
                         g_scan_list[i].bssid[1],
                         g_scan_list[i].bssid[2],
                         g_scan_list[i].bssid[3],
                         g_scan_list[i].bssid[4],
                         g_scan_list[i].bssid[5]);
                LOGI("%-7d", g_scan_list[i].is_valid);
                LOGI("%-7d", g_scan_list[i].is_hidden);
                LOGI("%-7d", g_scan_list[i].auth_mode);
                LOGI("%-7d", g_scan_list[i].encrypt_type);
                LOGI("%-7d", g_scan_list[i].rssi);
                /*printf("%-7d", g_scan_list[i].is_wps_supported);
                LOG_FUNC("%-7d", g_scan_list[i].wps_element.configuration_methods);
                LOG_FUNC("%-7d", g_scan_list[i].wps_element.device_password_id);
                LOG_FUNC("%-7d", g_scan_list[i].wps_element.selected_registrar);*/
                LOGI("%-7d", g_scan_list[i].beacon_interval);
                LOGI("%-7d", g_scan_list[i].capability_info);
                LOGI("\n");
            }
            LOG_FUNC("[MTK Event Callback Sample]: Scan Done!\n");
            break;
        default:
            wifi_event_handle = 0;
            LOG_FUNC("[MTK Event Callback Sample]: Unknown event(%d)\n", event);
            break;
    }

    return wifi_event_handle;
}

void _repeater_init(void)
{
    if((wifi_repeater_scan_timer_start()) == 0){
        LOGI("Repeater Initial Fail\n");
    }
}
//-------------------------------------------------------------------------------------------
// File functions - WiFi initial function
// 
//-------------------------------------------------------------------------------------------
void lot_wifi_init_task(void *para)
{
    struct wifi_cfg wifi_config = {0};
    if (0 != wifi_config_init(&wifi_config)) {
        LOG_E(common, "wifi config init fail");
    }

    wifi_config_t config = {0};
    wifi_config_ext_t config_ext = {0};
    memset(&config, 0, sizeof(config));
    memset(&config_ext, 0, sizeof(config_ext));

    config.opmode = wifi_config.opmode;

    kalMemCopy(config.sta_config.ssid, wifi_config.sta_ssid,
               WIFI_MAX_LENGTH_OF_SSID);
    config.sta_config.ssid_length = wifi_config.sta_ssid_len;
    config.sta_config.bssid_present = 0;
    config.sta_config.channel = wifi_config.sta_channel;
    kalMemCopy(config.sta_config.password, wifi_config.sta_wpa_psk,
               WIFI_LENGTH_PASSPHRASE);
    config.sta_config.password_length = wifi_config.sta_wpa_psk_len;
    if (wifi_config.sta_default_key_id == 255)
        config_ext.sta_wep_key_index_present = 0;
    else
        config_ext.sta_wep_key_index_present = 1;
    config_ext.sta_wep_key_index = wifi_config.sta_default_key_id;
    config_ext.sta_listen_interval_present = 1;
    config_ext.sta_listen_interval = wifi_config.sta_listen_interval;
    config_ext.sta_power_save_mode_present = 1;
    config_ext.sta_power_save_mode = wifi_config.sta_power_save_mode;

    kalMemCopy(config.ap_config.ssid, wifi_config.ap_ssid,
               WIFI_MAX_LENGTH_OF_SSID);
    config.ap_config.ssid_length = wifi_config.ap_ssid_len;
    kalMemCopy(config.ap_config.password, wifi_config.ap_wpa_psk,
               WIFI_LENGTH_PASSPHRASE);
    config.ap_config.password_length = wifi_config.ap_wpa_psk_len;
    config.ap_config.auth_mode =
        (wifi_auth_mode_t)wifi_config.ap_auth_mode;
    config.ap_config.encrypt_type =
        (wifi_encrypt_type_t)wifi_config.ap_encryp_type;
    config.ap_config.channel = wifi_config.ap_channel;
    config_ext.ap_wep_key_index_present = 1;
    config_ext.ap_wep_key_index = wifi_config.ap_default_key_id;
    config_ext.ap_dtim_interval_present = 1;
    config_ext.ap_dtim_interval = wifi_config.ap_dtim_interval;

    config_ext.sta_auto_connect_present = 1;
    config_ext.sta_auto_connect = 1;
    LOGI("\nSSID:%s\n",config.sta_config.ssid);
    LOGI("Password:%s\n",config.sta_config.password);
    wifi_init(&config, &config_ext);
     if(config.opmode == WIFI_MODE_AP_ONLY) {
        //timer_wifi_ap_scan(1);              // Scan router every 5 sec
        _repeater_init();
    }else if(config.opmode == WIFI_MODE_STA_ONLY){
        wifi_connection_register_event_handler(WIFI_EVENT_IOT_INIT_COMPLETE, (wifi_event_handler_t)lot_wifi_event_handler);
        wifi_connection_register_event_handler(WIFI_EVENT_IOT_SCAN_COMPLETE, (wifi_event_handler_t)lot_wifi_event_handler);
        wifi_connection_scan_init(g_ap_list, MTK_SCAN_LIST_SIZE);
    }else{
        LOGW("WiFi Mode not support:%d\n",config.opmode);
    }

    

    struct netif *sta_if = netif_find("st1");

    while (sta_if != NULL) {
        if ((pdTRUE == lwip_net_ready()) && (iot_IsIpReady(sta_if))) {
            /*ping_request(3, "8.8.8.8", PING_IP_ADDR_V4, 64, NULL);*/
            break;
        }
    }

    vTaskDelete(NULL);
}

static wifi_scan_list_item_t *scan_ap_list = NULL;
static int scan_ap_size = 0, ssid_list_size = 0;
static ssid_info_t *ssid_list = NULL;
//-------------------------------------------------------------------------------------------
// File functions - WiFi Event function
//      -Process scan complete process
//-------------------------------------------------------------------------------------------
wifi_event_handler_t _ap_mode_scan_event_handler(wifi_event_t event_id, unsigned char *payload, unsigned int len)
{
    int handled = 0;
    int i, total, size, j;
    bool found;

    switch (event_id) {
        case WIFI_EVENT_IOT_SCAN_COMPLETE:
            LOGD("WiFi event: WIFI_EVENT_IOT_SCAN_COMPLETE\n");
            handled = 1;
            // keep SSID list in local begin
            if (ssid_list == NULL) {
                ssid_list = malloc(sizeof(ssid_info_t) * 30);
                os_memset((char *)ssid_list, 0, sizeof(ssid_info_t) * 30);
            }
            // keep SSID list in local end

            for (i = 0, total = 0; i < scan_ap_size; i++) {
                if (scan_ap_list[i].ssid[0] == 0) {
                    continue;
                }
                total++;
                printf("\n%-4s%-33s%-20s%-8s%-8s%-8s%-8s%-8s%-8s%-8s\n", "Ch", "SSID", "BSSID", "Auth", "Cipher",
                       "RSSI", "WPS_EN", "CM", "DPID", "SR");
                printf("%-4d", scan_ap_list[i].channel);
                printf("%-33s", scan_ap_list[i].ssid);
                printf("%02x:%02x:%02x:%02x:%02x:%02x   ",
                       scan_ap_list[i].bssid[0],
                       scan_ap_list[i].bssid[1],
                       scan_ap_list[i].bssid[2],
                       scan_ap_list[i].bssid[3],
                       scan_ap_list[i].bssid[4],
                       scan_ap_list[i].bssid[5]);
                printf("%-8d", scan_ap_list[i].auth_mode);
                printf("%-8d", scan_ap_list[i].encrypt_type);
                printf("%-8d", scan_ap_list[i].rssi);
                printf("%-8d", scan_ap_list[i].is_wps_supported);
                printf("%-8d", scan_ap_list[i].wps_element.configuration_methods);
                printf("%-8d", scan_ap_list[i].wps_element.device_password_id);
                printf("%-8d", scan_ap_list[i].wps_element.selected_registrar);
                printf("\n");

                // keep SSID list in local begin
                if (ssid_list_size < 30) {
                    found = false;
                    for (j = 0; j < ssid_list_size; j++) {
                        if (strncmp(ssid_list[j].ssid, scan_ap_list[i].ssid, ssid_list[j].len) == 0) {
                            found = true;
                            break;
                        }
                    }
                    if (found == false) {
                        ssid_list[ssid_list_size].len = scan_ap_list[i].ssid_length;
                        strncpy(ssid_list[ssid_list_size].ssid,
                                scan_ap_list[i].ssid, scan_ap_list[i].ssid_length);
                        ssid_list_size++;
                    }
                }
                // keep SSID list in local end
            }

            wifi_connection_stop_scan();
            wifi_connection_scan_deinit();
            scan_ap_size = total;
            break;
    }
    return (wifi_event_handler_t)handled;
}

//-------------------------------------------------------------------------------------------
// File functions - Router Scan function
//      -AP Mode scan router SSID and parameter for APP use
//      -Every 5 sec execute this function one time
//-------------------------------------------------------------------------------------------
int32_t router_scan(uint8_t max_ssid_num)
{
    if(max_ssid_num > 32)
    {
        return -1;
    }
    scan_ap_size = max_ssid_num;
    if (scan_ap_list == NULL) {
        scan_ap_list = malloc(sizeof(wifi_scan_list_item_t) * scan_ap_size);
    }
    os_memset((char *)scan_ap_list, 0, sizeof(wifi_scan_list_item_t) * scan_ap_size);

    //wifi_connection_register_event_handler(WIFI_EVENT_IOT_SCAN_COMPLETE,
    //                                       (wifi_event_handler_t) _ap_mode_scan_event_handler);
    int32_t rc = wifi_connection_scan_init(scan_ap_list, max_ssid_num);
    if(rc < 0) {
        LOGW("router_scan(): wifi_connection_scan_init NG!(%ld)\n", rc);
    }
    rc = wifi_connection_start_scan(NULL, 32, NULL, 1, 1);
    if (rc < 0) {
        LOGW("router_scan(): wifi_connection_start_scan NG!(%ld)\n", rc);
        // Note by Charming begin
        // it seems lower layer keep running when call the function wifi_connection_start_scan() got failed
        wifi_connection_stop_scan();
        wifi_connection_scan_deinit();
        // free(scan_ap_list);
        // scan_ap_list = NULL;
        // Note by Charming end
        //timer_wifi_ap_scan(0);
    }
    return rc;
}
