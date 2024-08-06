#include "app.h"


osMutexId_t log_mutex_id;

/**
 * @brief board's hardware init before os init, such as uart, i2c, etc.
 * 
 */
void board_init(void) {

}

/**
 * @brief app before after os start, add threads, queues, etc.
 * 
 */
void app_init(void) {
    log_mutex_id = osMutexNew(NULL);
    if (log_mutex_id == NULL) {

    }
}

void Main() {
    // 串口发送，阻塞发送。因为中断和DMA都通过
    HAL_UART_Transmit(&log_huart, (uint8_t *)"STM32 serial test\n", sizeof("STM32 serial test\n"), 1000);
    osMutexAcquire(log_mutex_id, osWaitForever);
    // 中断下为异步发送，在发送完成后会调用HAL_UART_TxCpltCallback
    HAL_UART_Transmit_IT(&log_huart, (uint8_t *)"STM32 serial interrupt test\n", sizeof("STM32 serial interrupt test\n"));
    osMutexAcquire(log_mutex_id, osWaitForever);
    HAL_UART_Transmit_DMA(&log_huart, (uint8_t *)"STM32 serial DMA test\n", sizeof("STM32 serial DMA test\n"));
    osMutexAcquire(log_mutex_id, osWaitForever);

    /* Infinite loop */
    for(;;)
    {
        osDelay(1000);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == log_huart.Instance) {
        osSemaphoreRelease(log_lockHandle);
    }
}