#include "p_led.hpp"
#include "robot_config.hpp"
#include "robot_state_types.hpp"
#include <Arduino.h>

// Global variable that can be modified anywhere in the codebase to change the blink rate
volatile uint32_t g_status_led_delay_ms = LED_IDLE_DELAY; // Default to 500ms (idle)

extern RobotState g_robot_state;

void p_led_init() {
    for (int i = 0; i < 2; i++) {
        pinMode(ROBOT_PINOUT.leds[i].pin, OUTPUT);
        digitalWrite(ROBOT_PINOUT.leds[i].pin, LOW);
        
        g_robot_state.leds[i].is_on = false;
        g_robot_state.leds[i].timestamp_ms = 0;
    }
}

void Task_StatusLED(void *pvParameters) {
    uint8_t status_pin = ROBOT_PINOUT.leds[0].pin;
    bool toggle_state = false;

    for (;;) {
        if (g_status_led_delay_ms == 0) {
            // Turn on solid
            digitalWrite(status_pin, HIGH);
            g_robot_state.leds[0].is_on = true;
            g_robot_state.leds[0].timestamp_ms = millis();
            
            // Trap here, but yield slightly to appease the FreeRTOS watchdog
            while (g_status_led_delay_ms == 0) {
                vTaskDelay(pdMS_TO_TICKS(50));
            }
        } else {
            // Normal blinking operation
            toggle_state = !toggle_state;
            digitalWrite(status_pin, toggle_state ? HIGH : LOW);
            
            g_robot_state.leds[0].is_on = toggle_state;
            g_robot_state.leds[0].timestamp_ms = millis();
            
            vTaskDelay(pdMS_TO_TICKS(g_status_led_delay_ms));
        }
    }
}