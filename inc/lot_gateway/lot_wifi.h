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

#ifndef __LOT_WIFI_H__
#define __LOT_WIFI_H__

#include <stdio.h>
#include "wifi_api.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    char ssid[WIFI_MAX_LENGTH_OF_SSID];
    uint8_t len;
} ssid_info_t;

void lot_wifi_init_task(void *para);
void sntp_app_enter(void);

#ifdef __cplusplus
}
#endif

#endif      /* __LOT_WIFI_H__ */