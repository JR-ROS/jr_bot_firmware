#ifndef P_MOTORS_HPP
#define P_MOTORS_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

/**
 * @brief Initializes the ESP32 LEDC PWM channels and GPIO direction pins for the L293D.
 * @note This must be called in setup() before the FreeRTOS scheduler starts.
 */
void p_motors_init();

/**
 * @brief FreeRTOS Timer Callback that pushes the target PWMs from the global state to the hardware.
 */
void MotorWriteTimerCallback(TimerHandle_t xTimer);

#endif // P_MOTORS_HPP