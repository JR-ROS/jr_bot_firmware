#include "p_uart.hpp"
#include "p_led.hpp"
#include "bot_speak.h"
#include "sbc_comms.hpp"
#include "robot_config.hpp"
#include <Arduino.h>


void p_uart_init() {
    // 0 timeout because we manage the buffering manually now
    Serial.begin(USB_BAUD_RATE);
    Serial.setTimeout(0); 
}

void Task_CommsParse(void *pvParameters) {
    DataFrame_TypeDef frame;
    uint8_t payload_buffer[MAX_PACKET_BUFFER_SIZE];
    frame.data = payload_buffer;

    static uint8_t rx_buffer[MAX_PACKET_BUFFER_SIZE];
    static uint32_t rx_index = 0;
    
    // Track activity
    uint32_t last_packet_time = millis();

    for (;;) {
        // Check for inactivity timeout
        if (millis() - last_packet_time > LED_IDLE_DELAY) {
            g_status_led_delay_ms = LED_IDLE_DELAY;
        }

        while (Serial.available() > 0) {
            uint8_t c = Serial.read();

            // Activity detected: Reset timer and set active blink rate
            last_packet_time = millis();
            g_status_led_delay_ms = LED_USB_ACTIVE_DELAY; 

            // --- State Machine logic ---
            if (rx_index == 0 && c != START_BYTE) continue;

            if (rx_index < MAX_PACKET_BUFFER_SIZE) {
                rx_buffer[rx_index++] = c;
            } else {
                rx_index = 0;
                continue;
            }

            if (rx_index >= 13) {
                uint32_t expected_data_len = 0;
                memcpy(&expected_data_len, &rx_buffer[5], 4);
                uint32_t total_expected_len = 13 + expected_data_len + 1;

                if (rx_index == total_expected_len) {
                    if (rx_buffer[rx_index - 1] == END_BYTE) {
                        if (botSpeak_unpackFrame(&frame, rx_buffer, rx_index) == 0) {
                            comms_parse_frame_id(&frame);
                        }
                    }
                    rx_index = 0; 
                } else if (rx_index > total_expected_len) {
                    rx_index = 0; 
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}