#ifndef P_SERVO_HPP
#define P_SERVO_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>

/**
 * @brief Initializes the ESP32 hardware PWM timers and attaches the servo.
 * @note This must be called in setup() before the FreeRTOS scheduler starts.
 */
void p_servo_init();

/**
 * @brief FreeRTOS Timer Callback to write the current target angle to the servo hardware.
 */
void ServoWriteTimerCallback(TimerHandle_t xTimer);

/**
 * @brief FreeRTOS task that continuously calculates the sweeping triangle wave 
 * to update the target servo angle over time.
 */
void Task_ServoCalc(void *pvParameters);

#endif // P_SERVO_HPP