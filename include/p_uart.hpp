#ifndef P_UART_HPP
#define P_UART_HPP

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/queue.h>
#include <freertos/task.h>

constexpr uint8_t MAX_PACKET_BUFFER_SIZE = 64; 

// Structure to hold raw bytes from the Serial buffer
struct UartPacket {
    uint8_t data[MAX_PACKET_BUFFER_SIZE];
    uint32_t length;
};

/**
 * @brief Configures UART timeouts.
 */
void p_uart_init();

/**
 * @brief FreeRTOS Timer Callback to dump the hardware Serial buffer into a queue.
 */
void CommsRxTimerCallback(TimerHandle_t xTimer);

/**
 * @brief FreeRTOS task that blocks on the RX queue, unpacks BotSpeak frames, 
 * and passes them to the command/request parsers.
 */
void Task_CommsParse(void *pvParameters);

#endif // P_UART_HPP