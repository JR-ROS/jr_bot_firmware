#include "p_servo.hpp"
#include "robot_config.hpp"
#include <Arduino.h>

// Explicitly define a channel that DOES NOT overlap with motors (0 and 1)
#define SERVO_FREQ 50          // 50Hz for standard servos
#define SERVO_RESOLUTION 16    // 16-bit precision

// Duty cycle limits for 50Hz (20ms period)
// 1ms pulse (0 deg)  = (1ms / 20ms) * 65536 = 3277
// 2ms pulse (180 deg)= (2ms / 20ms) * 65536 = 6554
#define DUTY_MIN 3277
#define DUTY_MAX 6554

// The global target angle
volatile uint16_t g_current_servo_angle = 90; // Default to center

void p_servo_init() {
    // Configure LEDC: 50Hz, 16-bit resolution
    ledcSetup(ROBOT_PINOUT.tof_servo.pwm_channel, SERVO_FREQ, SERVO_RESOLUTION);
    ledcAttachPin(ROBOT_PINOUT.tof_servo.pin, ROBOT_PINOUT.tof_servo.pwm_channel);
}

void ServoWriteTimerCallback(TimerHandle_t xTimer) {
    // Convert angle (0-180) to duty cycle (3277-6554)
    uint32_t duty = map(g_current_servo_angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, DUTY_MIN, DUTY_MAX);
    ledcWrite(ROBOT_PINOUT.tof_servo.pwm_channel, duty);
}

void Task_ServoCalc(void *pvParameters) {
    int8_t sweep_direction = 1;
    const TickType_t delay_per_degree = pdMS_TO_TICKS(SERVO_SWEEP_DURATION_MS / SERVO_MAX_ANGLE);

    for (;;) {
        if (sweep_direction == 1) {
            if (g_current_servo_angle < SERVO_MAX_ANGLE) g_current_servo_angle++;
            else sweep_direction = -1;
        } else {
            if (g_current_servo_angle > SERVO_MIN_ANGLE) g_current_servo_angle--;
            else sweep_direction = 1;
        }
        vTaskDelay(delay_per_degree);
    }
}