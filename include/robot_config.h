#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include "robot_pinout_types.h"

// ==========================================
// SYSTEM & TIMING CONTEXT
// ==========================================
constexpr uint32_t USB_BAUD_RATE      = 115200;

// FreeRTOS Task Frequencies (in Hz)
constexpr uint8_t TASK_MOTOR_FREQ_HZ  = 100;
constexpr uint8_t TASK_IMU_FREQ_HZ    = 50;
constexpr uint8_t TASK_IR_FREQ_HZ     = 20;
constexpr uint8_t TASK_TOF_FREQ_HZ    = 50;

// ==========================================
// HARDWARE CONSTRAINTS
// ==========================================
constexpr int16_t MOTOR_PWM_MAX       = 255;

constexpr uint16_t SERVO_MAX_ANGLE    = 180;
constexpr uint16_t SERVO_SWEEP_DURATION_MS = 2000; // Time for a full sweep from min to max angle

constexpr uint16_t TOF_MAX_DISTANCE_MM = 1200; // Maximum distance the ToF sensor can measure

// ==========================================
// PINOUT ALLOCATIONS
// ==========================================

constexpr RobotPinout ROBOT_PINOUT = {
    .motors = {
        [0] = { .forward_pin = 13, .reverse_pin = 12, .pwm_pin = 0, .pwm_channel = 0 }, // Left Motor
        [1] = { .forward_pin = 14, .reverse_pin = 27, .pwm_pin = 0, .pwm_channel = 1 }  // Right Motor
    },

    .i2c = {
        [0] = { .sda_pin = 25, .scl_pin = 4 }, // IMU
        [1] = { .sda_pin = 21, .scl_pin = 22 }  // ToF
    },

    .tof_servo = { .pin = 18, .pwm_channel = 2 },

    .ir_sensor = {
        [0] = { .pin = 34 }, // Left IR
        [1] = { .pin = 35 }  // Right IR
    },
};

#endif // ROBOT_CONFIG_H