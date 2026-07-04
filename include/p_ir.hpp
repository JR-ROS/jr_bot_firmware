#ifndef P_IR_HPP
#define P_IR_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

/**
 * @brief Initializes the GPIO pins for the IR sensors as inputs.
 * @note This must be called in setup() before the FreeRTOS scheduler starts.
 */
void p_ir_init();

/**
 * @brief FreeRTOS Timer Callback that polls the IR pins and updates the global state.
 */
void IRSensorTimerCallback(TimerHandle_t xTimer);

#endif // P_IR_HPP