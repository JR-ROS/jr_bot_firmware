#ifndef P_UART_HPP
#define P_UART_HPP

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

constexpr uint8_t MAX_PACKET_BUFFER_SIZE = 64; 

void p_uart_init();
void Task_CommsParse(void *pvParameters);

#endif // P_UART_HPP