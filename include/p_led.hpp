#ifndef P_LED_HPP
#define P_LED_HPP

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Stable LED0 blink delay
#define LED_IDLE_DELAY 500
#define LED_ERROR_DELAY 0
#define LED_USB_ACTIVE_DELAY 50

extern volatile uint32_t g_status_led_delay_ms;

/**
 * @brief Initializes the GPIO pins for the status and user LEDs.
 */
void p_led_init();

/**
 * @brief FreeRTOS task that blinks the status LED. 
 * The blink rate is controlled by the global g_status_led_delay_ms.
 */
void Task_StatusLED(void *pvParameters);

#endif // P_LED_HPP