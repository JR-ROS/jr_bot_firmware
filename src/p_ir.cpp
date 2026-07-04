#include "p_ir.hpp"
#include "robot_config.hpp"
#include "robot_state_types.hpp"
#include <Arduino.h>

// Extern the global state so the timer callback can write the triggered status
extern RobotState g_robot_state;

void p_ir_init() {
    for (int i = 0; i < 2; i++) {
        // Set the assigned IR pins as inputs. 
        // Using INPUT instead of INPUT_PULLUP as most cheap IR modules have their own pull-ups.
        pinMode(ROBOT_PINOUT.ir_sensor[i].pin, INPUT);
    }
}

void IRSensorTimerCallback(TimerHandle_t xTimer) {
    uint32_t current_time = millis(); // Time of poll
    
    for (int i = 0; i < 2; i++) {
        uint8_t pin = ROBOT_PINOUT.ir_sensor[i].pin;
        int pin_state = digitalRead(pin);

        g_robot_state.irs[i].is_triggered = (pin_state == LOW);
        g_robot_state.irs[i].timestamp_ms = current_time;
    }
}