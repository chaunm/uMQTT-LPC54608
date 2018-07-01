#include <threading_alt.h>
#include "mbedtls/threading.h"

void mutex_init_alt(mbedtls_threading_mutex_t* mutex)
{
    if( mutex == NULL )
        return;
    mutex->mutex = xSemaphoreCreateMutex();
    mutex->is_valid = 1;
}

void mutex_free_alt(mbedtls_threading_mutex_t* mutex)
{
    if( mutex == NULL || !mutex->is_valid )
        return;
    vSemaphoreDelete(mutex->mutex);
    mutex->is_valid = 0;
}
int mutex_lock_alt(mbedtls_threading_mutex_t* mutex)
{
    if( mutex == NULL || ! mutex->is_valid )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( xSemaphoreTake( mutex->mutex, portMAX_DELAY ) != pdTRUE )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );
    return (0);
}

int mutex_unlock_alt(mbedtls_threading_mutex_t* mutex)
{
    if( mutex == NULL || ! mutex->is_valid )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( xSemaphoreGive( mutex->mutex ) != pdTRUE )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );
    return 0;
}