#include "p_uart.hpp"
#include "robot_config.hpp"

#include <Arduino.h>

// Extern the queue handle defined in main.cpp
extern QueueHandle_t rx_packet_queue;

void p_uart_init() {
    Serial.begin(USB_BAUD_RATE);

    // Arduino's default readBytes timeout is 1000ms. 
    // We MUST set it to 0, otherwise readBytes will stall the entire 
    // FreeRTOS Timer Daemon waiting for data that hasn't arrived yet.
    Serial.setTimeout(0);
}

void CommsRxTimerCallback(TimerHandle_t xTimer) {
    size_t available_bytes = Serial.available();
    
    if (available_bytes > 0) {
        UartPacket packet;
        
        // Read up to MAX_PACKET_BUFFER_SIZE. 
        // Because timeout is 0, this instantly returns whatever is currently in the RX FIFO.
        packet.length = Serial.readBytes(packet.data, MAX_PACKET_BUFFER_SIZE);
        
        if (packet.length > 0) {
            // Push to the queue with 0 block time. 
            // If the parser task gets backed up and the queue is full, we drop the frame.
            xQueueSend(rx_packet_queue, &packet, 0);
        }
    }
}