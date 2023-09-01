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


#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "lot_timer.h"
#include "lot_wifi.h"
#include "lot_rtos.h"
#include "stddef.h"
#include "my_header.h"



//-------------------------------------------------------------------------------------------
// File functions - LOT timer call back functions
//      - LOT_TIMER_1S: repeat timer every 1 sec
//      - TIMER_ID_WIFI_SCAN: time delay for Wifi router scan
//-------------------------------------------------------------------------------------------
uint32_t g_lot_counter_1s = 0;

 void _lot_timer_expired_cb(TimerHandle_t xTimer)
 {
    uint32_t id = (uint32_t)pvTimerGetTimerID(xTimer);
    configASSERT( xTimer );
    switch(id)
    {
        case LOT_TIMER_1S:
            if((g_lot_counter_1s & 0x0000000F) == 0)
            {
                printf("\n1S Time Tick:%d\n",g_lot_counter_1s);
            }
            g_lot_counter_1s++;
            break;
        case TIMER_ID_WIFI_SCAN:
            router_scan(MTK_SCAN_LIST_SIZE);
            break;
    }
 }

//-------------------------------------------------------------------------------------------
// File functions - 1 sec timer function init 
//      - Set 1 sec timer enable
//      - Timer ID: LOT_TIMER_1S
//-------------------------------------------------------------------------------------------
 void timer_1s_init(void)
 {
    TimerHandle_t tHandle = xTimerCreate("1S_Test_Timer", 1000, pdTRUE, LOT_TIMER_1S, _lot_timer_expired_cb);
    if( xTimerStart(tHandle, 0 ) != pdPASS )
    {
        printf("\nThe timer could not be set into the Active state!\n");
    }
 }

//-------------------------------------------------------------------------------------------
// File functions - Timer delay use for WiFi Repeater mode scan  
//      - Timer ID: TIMER_ID_WIFI_SCAN
// Input 
//      - freq : timer repeat time sec base
// Return
//      - 1 timer start succeed / 0 timer start fail
//-------------------------------------------------------------------------------------------
/*void timer_wifi_ap_scan(uint8_t star)
{
    static TimerHandle_t tHandle;
    if(star){
        tHandle = xTimerCreate("timer_wifi_ap_scan", 5000, pdTRUE, TIMER_ID_WIFI_SCAN, _lot_timer_expired_cb);
        if( xTimerStart(tHandle, 0 ) != pdPASS )
        {
            LOGI("\nThe AP scan timer could not be set into the Active state!\n");
        }
    }else{
        if( xTimerStop(tHandle,0) != pdPASS )
        {
            LOGI("\nThe AP scan timer could not stop!\n");
        }
    }
}*/
TimerHandle_t repeater_scan_tHandle;
uint32_t wifi_repeater_scan_timer_start(uint32_t freq)
{
    repeater_scan_tHandle = xTimerCreate("timer_wifi_repeater_scan", freq * 1000, pdTRUE, TIMER_ID_WIFI_SCAN, _lot_timer_expired_cb);
    if( xTimerStart(repeater_scan_tHandle, 0) != pdPASS)
    {
        LOGI("\nThe AP scan timer could not be set into the Active state!\n");
        return 0;
    }
    return 1;
}

uint32_t wifi_repeater_scan_timer_stop(void)
{
    if( xTimerStop(repeater_scan_tHandle, 0) != pdPASS)
    {
        LOGI("\nThe repeater scan timer could not stop!\n");
        return 0;
    }
    return 1;
}
//-------------------------------------------------------------------------------------------
// File functions - Timer delay use for WiFi AP mode scan  
//      - Set 5 sec to samc one time
//      - Timer ID: TIMER_ID_WIFI_SCAN
//-------------------------------------------------------------------------------------------
/*bool_t clock_time_exceed(uint32_t tick, uint32_t space)
{
    if((g_lot_counter_1s - tick) > space){
        return eTRUE;
    }else{
        return eFALSE;
    }
}*/
