
#ifndef __LOT_RTOS_H__
#define __LOT_RTOS_H__

//#include <stdbool.h>
#include "FreeRTOS.h"
#include "stddef.h"


#ifdef __cplusplus
extern "C" {
#endif

#define RC_VERIFIED     3
#define RC_NO_RSP       2
#define RC_NO_CHANGE    1
#define RC_OK           0
#define RC_ERROR       -1
#define RC_FAIL        -2
#define RC_NOT_SUPPORT -3
#define RC_FULL        -4
#define RC_NOT_FOUND   -5

typedef enum {
    eFALSE = 0,
    eTRUE = 1
} bool_t;

#if defined(LOT_DEBUG)
#define LOT_ASSERT(x) configASSERT(x)
#else
#define LOT_ASSERT(x)
#endif

#if defined(LOT_LOGE)
#define LOGE(...) \
    { \
        printf("[E] "__VA_ARGS__); \
        fflush(stdout); \
    }
#else
#define LOGE(...)
#endif

#if defined(LOT_LOGW)
#define LOGW(...) \
    { \
        printf("[W] "__VA_ARGS__); \
        fflush(stdout); \
    }
#else
#define LOGW(...)
#endif

#if defined(LOT_LOGI)
#define LOGI(...) \
    { \
        printf("[I] "__VA_ARGS__); \
        fflush(stdout); \
    }
#else
#define LOGI(...)
#endif

#if defined(LOT_LOGD)
#define LOGD(...) \
    { \
        printf("[D] "__VA_ARGS__); \
        fflush(stdout); \
    }
#else
#define LOGD(...)
#endif

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* __LOT_RTOS_H__ */
