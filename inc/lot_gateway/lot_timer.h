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
// Date: 2023-08-09
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// File functions - Gateway Timer realtive functions header file
//      - LOT define Timer
//      - Multi-timer functions use same timer use Timer ID to seprate
//-------------------------------------------------------------------------------------------

#ifndef __LOT_TIMER_H__
#define __LOT_TIMER_H__

#include <stdint.h>
#include "timers.h"
#include "lot_rtos.h"


#ifdef __cplusplus
extern "C" {
#endif

#define     LOT_TIMER_1S                            0x01
#define     TIMER_ID_WIFI_SCAN                      0x02
//typedef enum {
//    LOT_TIMER_1S = 0,
//    TIMER_ID_WIFI_SCAN
//} lot_timer_id_t;

//uint32_t g_lot_counter_1s;
void lot_timer_expired_cb(TimerHandle_t xTimer);
void timer_1s_init(void);
bool_t clock_time_exceed(uint32_t tick, uint32_t space);
uint32_t wifi_repeater_scan_timer_start(uint32_t freq);
uint32_t wifi_repeater_scan_timer_stop(void);



#ifdef __cplusplus
}
#endif

#endif      /* __BT_UART_H__ */