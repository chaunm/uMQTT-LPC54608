#ifndef __THREADING_ALT_H__
#define __THREADING_ALT_H__
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct
{
    SemaphoreHandle_t mutex;
    char is_valid;
} mbedtls_threading_mutex_t;

void mutex_init_alt(mbedtls_threading_mutex_t* mutex);
void mutex_free_alt(mbedtls_threading_mutex_t* mutex);
int mutex_lock_alt(mbedtls_threading_mutex_t* mutex);
int mutex_unlock_alt(mbedtls_threading_mutex_t* mutex);

#endif