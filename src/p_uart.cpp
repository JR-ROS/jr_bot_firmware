#include "p_uart.hpp"
#include "bot_speak.h"
#include "sbc_comms.hpp"
#include <Arduino.h>

// Extern the queue handle defined in main.cpp
extern QueueHandle_t rx_packet_queue;

void p_uart_init() {
    // Timeout MUST be 0 so readBytes doesn't stall the FreeRTOS Timer Daemon
    Serial.setTimeout(0);
}

void CommsRxTimerCallback(TimerHandle_t xTimer) {
    size_t available_bytes = Serial.available();
    
    if (available_bytes > 0) {
        UartPacket packet;
        
        // Read whatever is currently in the RX FIFO instantly
        packet.length = Serial.readBytes(packet.data, MAX_PACKET_BUFFER_SIZE);
        
        if (packet.length > 0) {
            // Push to the queue without blocking the timer daemon
            xQueueSend(rx_packet_queue, &packet, 0);
        }
    }
}

void Task_CommsParse(void *pvParameters) {
    UartPacket raw_packet;
    DataFrame_TypeDef frame;
    
    // BotSpeak requires a pre-allocated buffer to copy the payload into during unpacking
    uint8_t payload_buffer[MAX_PACKET_BUFFER_SIZE];
    frame.data = payload_buffer;

    for (;;) {
        // Block indefinitely, freeing the CPU until the RX Timer puts a packet in the queue
        if (xQueueReceive(rx_packet_queue, &raw_packet, portMAX_DELAY) == pdTRUE) {
            
            // Attempt to unpack the raw bytes. 
            // botSpeak_unpackFrame returns 0 on success.
            if (botSpeak_unpackFrame(&frame, raw_packet.data, raw_packet.length) == 0) {
                
                // If the frame is valid, route it to the execution logic in sbc_comms.cpp
                comms_parse_frame_id(&frame);
            }
        }
    }
}