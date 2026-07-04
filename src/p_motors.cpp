#include "p_motors.hpp"
#include "robot_config.hpp"
#include "robot_state_types.hpp"
#include <Arduino.h>

// Extern the global state so the timer callback can read the target PWMs
extern RobotState g_robot_state;

void p_motors_init() {
    for (int i = 0; i < 2; i++) {
        uint8_t fwd_pin = ROBOT_PINOUT.motors[i].forward_pin;
        uint8_t rev_pin = ROBOT_PINOUT.motors[i].reverse_pin;
        uint8_t pwm_pin = ROBOT_PINOUT.motors[i].pwm_pin;
        uint8_t channel = ROBOT_PINOUT.motors[i].pwm_channel;

        // Set L293D direction pins as outputs
        pinMode(fwd_pin, OUTPUT);
        pinMode(rev_pin, OUTPUT);
        
        // Ensure motors start stopped
        digitalWrite(fwd_pin, LOW);
        digitalWrite(rev_pin, LOW);

        // Configure LEDC PWM: 1kHz frequency, 8-bit resolution (0-255)
        // 1kHz is ideal for the slow Darlington transistors in the L293D to prevent overheating.
        ledcSetup(channel, 1000, 8);
        ledcAttachPin(pwm_pin, channel);
        
        // Initialize state to 0
        ledcWrite(channel, 0);
    }
}

void MotorWriteTimerCallback(TimerHandle_t xTimer) {
    for (int i = 0; i < 2; i++) {
        // Fetch target from global state
        int16_t target = g_robot_state.motors[i].target_pwm;

        // Safety clamp to prevent overflowing the 8-bit PWM limit
        if (target > MOTOR_PWM_MAX) target = MOTOR_PWM_MAX;
        if (target < -MOTOR_PWM_MAX) target = -MOTOR_PWM_MAX;

        uint8_t duty_cycle = abs(target);
        uint8_t fwd_pin = ROBOT_PINOUT.motors[i].forward_pin;
        uint8_t rev_pin = ROBOT_PINOUT.motors[i].reverse_pin;
        uint8_t channel = ROBOT_PINOUT.motors[i].pwm_channel;

        // Map sign to L293D direction logic
        if (target > 0) {
            digitalWrite(fwd_pin, HIGH);
            digitalWrite(rev_pin, LOW);
        } else if (target < 0) {
            digitalWrite(fwd_pin, LOW);
            digitalWrite(rev_pin, HIGH);
        } else {
            // Coast/Stop
            digitalWrite(fwd_pin, LOW);
            digitalWrite(rev_pin, LOW);
        }

        // Apply duty cycle to the specific hardware channel
        ledcWrite(channel, duty_cycle);

        // Update the current state to reflect what the hardware is actually doing
        g_robot_state.motors[i].current_pwm = target;
    }
}