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

#ifndef __MY_HEADER_H__
#define __MY_HEADER_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IC_CONFIG
#define         IC_CONFIG                       mt7933
#endif

#ifndef BOARD_CONFIG
#define         BOARD_CONFIG                    mt7933_hdk
#endif

//debug level: none, error, warning, and info
#ifndef MTK_DEBUG_LEVEL
#define         MTK_DEBUG_LEVEL                 info
#endif

//3 options with psram/flash or not, only 1 option is y, the others should be n
#ifndef MTK_MEMORY_WITH_PSRAM_FLASH
#define         MTK_MEMORY_WITH_PSRAM_FLASH
#endif
//#define         MTK_MEMORY_WITHOUT_PSRAM
//#define         MTK_MEMORY_WITHOUT_PSRAM_FLASH

//System service debug feature for internal use
//#define         MTK_NON_INIT_HEAP
//#define         MTK_SUPPORT_HEAP_DEBUG
#ifndef MTK_OS_CPU_UTILIZATION_ENABLE
#define         MTK_OS_CPU_UTILIZATION_ENABLE
#endif
#ifndef MTK_XIP_ENABLE
#define         MTK_XIP_ENABLE
#endif

//NVDM
#ifndef MTK_NVDM_ENABLE
#define         MTK_NVDM_ENABLE
#endif
//#define         MTK_NVDM_NO_FLASH_ENABLE

//CONSYS
#ifndef MTK_MT7933_CONSYS_ENABLE
#define         MTK_MT7933_CONSYS_ENABLE
#endif
#ifdef  MTK_MT7933_CONSYS_ENABLE
#ifndef MTK_MT7933_WIFI_ENABLE
#define         MTK_MT7933_WIFI_ENABLE
#endif  
#endif

//CONNSYS WF
#ifndef MTK_MT7933_CONSYS_WIFI_ENABLE
#define         MTK_MT7933_CONSYS_WIFI_ENABLE
#endif

//WIFI features
#ifndef MTK_WIFI_AP_ENABLE
#define         MTK_WIFI_AP_ENABLE
#endif
#ifndef MTK_WIFI_ROUTER_ENABLE
#define         MTK_WIFI_ROUTER_ENABLE
#endif
#ifndef MTK_WIFI_PROFILE_ENABLE
#define         MTK_WIFI_PROFILE_ENABLE
#endif 
#ifndef MTK_WLAN_SERVICE_ENABLE
#define         MTK_WLAN_SERVICE_ENABLE
#endif 
#ifndef MTK_WIFI_TEST_TOOL_ENABLE
#define         MTK_WIFI_TEST_TOOL_ENABLE
#endif 
#ifndef MTK_WIFI_PSRAM_ENABLE
#define         MTK_WIFI_PSRAM_ENABLE
#endif 
#ifndef MTK_WIFI_EMI_IN_PSRAM
#define         MTK_WIFI_EMI_IN_PSRAM
#endif 
#ifndef MTK_WF_CLI_ENABLE
#define         MTK_WF_CLI_ENABLE
#endif
#ifndef MTK_WF_DBG_CLI_ENABLE
#define         MTK_WF_DBG_CLI_ENABLE
#endif

//enable mini-supplicant
#ifndef MTK_MINISUPP_ENABLE
#define         MTK_MINISUPP_ENABLE
#endif

//MTK system hang issue debug feauture option
//#define         MTK_SYSTEM_HANG_CHECK_ENABLE

//LWIP features
#ifndef MTK_LWIP_ENABLE
#define         MTK_LWIP_ENABLE
#endif
#ifndef MTK_IPERF_ENABLE
#define         MTK_IPERF_ENABLE
#endif
#ifndef MTK_PING_OUT_ENABLE
#define         MTK_PING_OUT_ENABLE
#endif
//#define         MTK_USER_FAST_TX_ENABLE

//CLI features
#ifndef MTK_MINICLI_ENABLE
#define         MTK_MINICLI_ENABLE
#endif
#ifndef MTK_CLI_TEST_MODE_ENABLE
#define         MTK_CLI_TEST_MODE_ENABLE
#endif
#ifndef MTK_LP_DVT_CLI_ENABLE
#define         MTK_LP_DVT_CLI_ENABLE
#endif
//#define         MTK_HAL_LOWPOWER_ENABLE
//#define         MTK_HIF_GDMA_ENABLE

#ifndef MTK_FREERTOS_VERSION
#define         MTK_FREERTOS_VERSION                V10
#endif
//#define         MTK_UT_ENABLE
#ifndef MTK_OS_CLI_ENABLE
#define         MTK_OS_CLI_ENABLE
#endif

//OS Heap Extend: n, heap5, multi
#ifndef MTK_OS_HEAP_EXTEND
#define         MTK_OS_HEAP_EXTEND                  multi
#endif

//BT Dual mode
//#define         MTK_BT_DUO_ENABLE

//BT driver
#ifndef MTK_MT7933_BT_ENABLE
#define         MTK_MT7933_BT_ENABLE
#endif
#ifndef MTK_BT_BUFFER_BIN_MODE
#define         MTK_BT_BUFFER_BIN_MODE
#endif
#ifndef MTK_BT_FW_BIN_TYPE
#define         MTK_BT_FW_BIN_TYPE                  le
#endif

//BT stack: Hummingbird
#ifndef MTK_BT_ENABLE
#define         MTK_BT_ENABLE
#endif
//MTK_BLE_ONLY_ENABLE                 = n
//MTK_BT_HFP_ENABLE                   = n
//MTK_BT_AVRCP_ENABLE                 = n
//MTK_BT_AVRCP_ENH_ENABLE             = n
//MTK_BT_A2DP_ENABLE                  = n
//MTK_BT_SPP_ENABLE                   = n
//MTK_BT_HID_ENABLE                   = n
//MTK_BT_PBAP_ENABLE                  = n
#ifndef MTK_BT_MESH_ENABLE
#define         MTK_BT_MESH_ENABLE
#endif
//MTK_BT_ACEMW_ENABLE                 = n
//MTK_BT_BAS_SERVICE_ENABLE           = n
#ifndef MTK_BLE_SMTCN_ENABLE
#define         MTK_BLE_SMTCN_ENABLE
#endif
//MTK_BT_SUPPORT_FW_ASSERT_RECOVERY   = n
//MTK_BT_TIMER_EXTERNAL_ENABLE        = n

//bluetooth connection manager feature support.
//MTK_BT_CM_SUPPORT                   = n

//BT Cli
#ifndef MTK_BLE_CLI_ENABLE
#define         MTK_BLE_CLI_ENABLE
#endif
#ifndef MTK_BT_MESH_CLI_ENABLE
#define         MTK_BT_MESH_CLI_ENABLE
#endif

//mesh example project: none, pts_device, test, pts_provisioner, switch, vendor_device, pts_lighting_client, pts_lighting_server
#ifndef MTK_BT_MESH_EXAMPLE_PROJECT
#define         MTK_BT_MESH_EXAMPLE_PROJECT             vendor_device
#endif

//BT tool
#ifndef MTK_BT_BOOTS_ENABLE
#define         MTK_BT_BOOTS_ENABLE
#endif
#ifndef MTK_BT_PICUS_ENABLE
#define         MTK_BT_PICUS_ENABLE
#endif

//TFM features
//MTK_TFM_ENABLE                       = n
//MTK_TFM_CLI_ENABLE                   = n
#ifndef MTK_AUXADCCLI_ENABLE
#define         MTK_AUXADCCLI_ENABLE
#endif
#ifndef MTK_HAL_SER_ENABLE
#define         MTK_HAL_SER_ENABLE
#endif
//MTK_HAL_SLA_LIB_ALL_IN_ONE           = n

//SSUSB
//MTK_SSUSB_GADGET_ENABLE              = n
//MTK_SSUSB_HOST_ENABLE                = n

//HIFIXDSP
//MTK_HIFI4DSP_ENABLE                  = n

//THERMAL_CLI
#ifndef MTK_THERMAL_CLI_ENABLE
#define         MTK_THERMAL_CLI_ENABLE
#endif

//POSIX
//MTK_POSIX_SUPPORT_ENABLE             = n

//Audio driver
//MTK_MT7933_AUDIO_DRIVER_ENABLE       = n
//MTK_MT7933_AUDIO_CODEC_ENABLE        = n

//FPGA Feature Disable
//MTK_FPGA_ENABLE                      = n
//MTK_1ST_LINK_SRAM_BOOT               = n

//select the build images
#ifndef MT7931AN_XIP_QFN_BW_ENABLE
#define         MT7931AN_XIP_QFN_BW_ENABLE
#endif

//Core Mini Dump
#ifndef MTK_MINI_DUMP_ENABLE
#define         MTK_MINI_DUMP_ENABLE
#endif

//compiler flag for DVT code
//MTK_DVT_CODE_ENABLE                  = n

//HTTPCLIENT SSL
#ifndef MTK_FOTA_V3_ENABLE
#define         MTK_FOTA_V3_ENABLE
#endif
#ifndef MTK_FOTA_V3_FREERTOS_ENABLE
#define         MTK_FOTA_V3_FREERTOS_ENABLE
#endif
#ifndef MTK_FOTA_V3_CLI_ENABLE
#define         MTK_FOTA_V3_CLI_ENABLE
#endif
#ifndef MTK_FOTA_V3_TFTP_ENABLE
#define         MTK_FOTA_V3_TFTP_ENABLE
#endif
#ifndef MTK_FOTA_V3_HTTP_ENABLE
#define         MTK_FOTA_V3_HTTP_ENABLE
#endif
//MTK_FOTA_V3_HTTPS_ENABLE             = n
#ifndef MTK_FOTA_V3_PACKET_ENCRYPTION_ALGORITHM
#define         MTK_FOTA_V3_PACKET_ENCRYPTION_ALGORITHM     aes128
#endif

//SWLA
#ifndef MTK_SWLA_ENABLE
#define         MTK_SWLA_ENABLE
#endif
#ifndef MTK_SWLA_USE_SYSRAM_BUFFER
#define         MTK_SWLA_USE_SYSRAM_BUFFER
#endif
//MTK_SWLA_WDT_RESET_TRACE             = n

//Syslog
#ifndef MTK_SAVE_LOG_AND_CONTEXT_DUMP_ENABLE
#define         MTK_SAVE_LOG_AND_CONTEXT_DUMP_ENABLE
#endif
#ifndef MTK_MEMORY_EXTRACTOR_ENABLE
#define         MTK_MEMORY_EXTRACTOR_ENABLE
#endif
#ifndef MTK_LIGHT_WEIGHT_PRINTF_ENABLED
#define         MTK_LIGHT_WEIGHT_PRINTF_ENABLED
#endif

//MTK_AUDIO_MP3_ENABLED                = n
//MTK_BT_CODEC_ENABLED                 = n

//Enable GCC LTO
#ifndef MTK_GCC_LTO_ENABLE
#define         MTK_GCC_LTO_ENABLE
#endif

//mbedtls config file
#ifndef MTK_MBEDTLS_CONFIG_FILE
#ifdef MTK_FOTA_V3_HTTPS_ENABLE
#define         MTK_MBEDTLS_CONFIG_FILE         config-mtk-fotav3.h
#else
#define         MTK_MBEDTLS_CONFIG_FILE         config-mtk-homekit.h
#endif
#endif


//F32K clock source software detection
//MTK_F32K_SW_DETECT_ENABLED           = n

//scan list size
#ifndef MTK_SCAN_LIST_SIZE
#define         MTK_SCAN_LIST_SIZE              32
#endif

//HAL Module Features
#ifndef MTK_HAL_ADC_MODULE_ENABLE
#define         MTK_HAL_ADC_MODULE_ENABLE
#endif
#ifndef MTK_HAL_AES_MODULE_ENABLE
#define         MTK_HAL_AES_MODULE_ENABLE
#endif
#ifndef MTK_HAL_CACHE_MODULE_ENABLE
#define         MTK_HAL_CACHE_MODULE_ENABLE
#endif
#ifndef MTK_HAL_CLOCK_MODULE_ENABLE
#define         MTK_HAL_CLOCK_MODULE_ENABLE
#endif
#ifndef MTK_HAL_DES_MODULE_ENABLE
#define         MTK_HAL_DES_MODULE_ENABLE
#endif
#ifndef MTK_HAL_EFUSE_MODULE_ENABLE
#define         MTK_HAL_EFUSE_MODULE_ENABLE
#endif
#ifndef MTK_HAL_EINT_MODULE_ENABLE
#define         MTK_HAL_EINT_MODULE_ENABLE
#endif
#ifndef MTK_HAL_FLASH_MODULE_ENABLE
#define         MTK_HAL_FLASH_MODULE_ENABLE
#endif
#ifndef MTK_HAL_GDMA_MODULE_ENABLE
#define         MTK_HAL_GDMA_MODULE_ENABLE
#endif

#ifndef MTK_HAL_GPIO_MODULE_ENABLE
#define         MTK_HAL_GPIO_MODULE_ENABLE
#endif
#ifdef MTK_HAL_GPIO_MODULE_ENABLE
#ifndef HAL_GPIO_MODULE_ENABLED
#define         HAL_GPIO_MODULE_ENABLED
#endif
#endif

#ifndef MTK_HAL_GPT_MODULE_ENABLE
#define         MTK_HAL_GPT_MODULE_ENABLE
#endif

#ifndef MTK_HAL_GCPU_MODULE_ENABLE
#define         MTK_HAL_GCPU_MODULE_ENABLE
#endif
#ifdef MTK_HAL_GCPU_MODULE_ENABLE
#ifndef HAL_GCPU_MODULE_ENABLED
#define         HAL_GCPU_MODULE_ENABLED
#endif
#endif

#ifndef MTK_HAL_I2C_MASTER_MODULE_ENABLE
#define         MTK_HAL_I2C_MASTER_MODULE_ENABLE
#endif
#ifndef MTK_HAL_NVIC_MODULE_ENABLE
#define         MTK_HAL_NVIC_MODULE_ENABLE
#endif

//MTK_HAL_IRRX_MODULE_ENABLE           = n
//MTK_HAL_KEYPAD_MODULE_ENABLE         = n
#ifndef MTK_HAL_MD5_MODULE_ENABLE
#define         MTK_HAL_MD5_MODULE_ENABLE
#endif
#ifndef MTK_HAL_MPU_MODULE_ENABLE
#define         MTK_HAL_MPU_MODULE_ENABLE
#endif
#ifndef MTK_HAL_ASIC_MPU_MODULE_ENABLE
#define         MTK_HAL_ASIC_MPU_MODULE_ENABLE
#endif
#ifndef MTK_HAL_PMU_MODULE_ENABLE
#define         MTK_HAL_PMU_MODULE_ENABLE
#endif

//MTK_HAL_PSRAM_UHS_ENABLE             = n
#ifndef MTK_HAL_PSRAM_MODULE_ENABLE
#define         MTK_HAL_PSRAM_MODULE_ENABLE
#endif
#ifndef MTK_PSRAM_TYPE_VALUE
#define         MTK_PSRAM_TYPE_VALUE
#endif
#ifndef MTK_HAL_PWM_MODULE_ENABLE
#define         MTK_HAL_PWM_MODULE_ENABLE
#endif
#ifndef MTK_HAL_RTC_MODULE_ENABLE
#define         MTK_HAL_RTC_MODULE_ENABLE
#endif
#ifndef MTK_HAL_SPI_MASTER_MODULE_ENABLE
#define         MTK_HAL_SPI_MASTER_MODULE_ENABLE
#endif
#ifndef MTK_HAL_SPI_SLAVE_MODULE_ENABLE
#define         MTK_HAL_SPI_SLAVE_MODULE_ENABLE
#endif
#ifndef MTK_HAL_SDIO_MODULE_ENABLE
#define         MTK_HAL_SDIO_MODULE_ENABLE
#endif
#ifndef MTK_HAL_SDIO_SLAVE_MODULE_ENABLE
#define         MTK_HAL_SDIO_SLAVE_MODULE_ENABLE
#endif
#ifndef MTK_HAL_SHA_MODULE_ENABLE
#define         MTK_HAL_SHA_MODULE_ENABLE
#endif
#ifndef MTK_HAL_SLEEP_MANAGER_MODULE_ENABLE
#define         MTK_HAL_SLEEP_MANAGER_MODULE_ENABLE
#endif
#ifndef MTK_HAL_TRNG_MODULE_ENABLE
#define         MTK_HAL_TRNG_MODULE_ENABLE
#endif
#ifndef MTK_HAL_UART_MODULE_ENABLE
#define         MTK_HAL_UART_MODULE_ENABLE
#endif

//MTK_HAL_USB_MODULE_ENABLE            = n
#ifndef MTK_HAL_WDT_MODULE_ENABLE
#define         MTK_HAL_WDT_MODULE_ENABLE
#endif
#ifdef MTK_HAL_WDT_MODULE_ENABLE
#ifndef HAL_WDT_MODULE_ENABLED
#define         HAL_WDT_MODULE_ENABLED
#endif
#endif

#ifndef MTK_HAL_DWT_MODULE_ENABLE
#define         MTK_HAL_DWT_MODULE_ENABLE
#endif
#ifndef MTK_HAL_CLK_CTP_SUPPORT
#define         MTK_HAL_CLK_CTP_SUPPORT
#endif
#ifndef MTK_HAL_SPM_LOW_POWER_SUPPORT
#define         MTK_HAL_SPM_LOW_POWER_SUPPORT
#endif
#ifndef MTK_HAL_DEVAPC_MODULE_ENABLE
#define         MTK_HAL_DEVAPC_MODULE_ENABLE
#endif

#ifndef MTK_HAL_I2S_MODULE_ENABLE
#define         MTK_HAL_I2S_MODULE_ENABLE
#endif
#ifndef MTK_HAL_SD_MODULE_ENABLE
#define         MTK_HAL_SD_MODULE_ENABLE
#endif
#ifndef MTK_HAL_DCXO_MODULE_ENABLE
#define         MTK_HAL_DCXO_MODULE_ENABLE
#endif


//MTK_HAL_PMU_FORCE_VCORE_0P8V         = n
#ifndef MTK_HAL_SECURITY_MODULE_ENABLE
#define         MTK_HAL_SECURITY_MODULE_ENABLE
#endif
#ifndef MTK_HAL_TOP_THERMAL_MODULE_ENABLE
#define         MTK_HAL_TOP_THERMAL_MODULE_ENABLE
#endif


//LOT Begin
#ifndef LOT_MODULE_ENABLE
#define         LOT_MODULE_ENABLE
#endif
#ifndef LOT_GATEWAY
#define         LOT_GATEWAY
#endif

#ifdef __cplusplus
}
#endif

#endif      /* __MY_HEADER_H__ */