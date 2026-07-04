#ifndef P_UART_HPP
#define P_UART_HPP

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/queue.h>

// Moved from main.cpp to here
constexpr uint8_t MAX_PACKET_BUFFER_SIZE = 64; 

// Structure to match your old USBData_TypeDef
struct UartPacket {
    uint8_t data[MAX_PACKET_BUFFER_SIZE];
    uint32_t length;
};

/**
 * @brief Initialize the hardware Serial port for USB communication.
 */
void p_uart_init();

/**
 * @brief FreeRTOS Timer Callback to dump the hardware Serial buffer into a queue.
 */
void CommsRxTimerCallback(TimerHandle_t xTimer);

#endif // P_UART_HPP