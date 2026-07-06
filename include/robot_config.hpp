#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include "robot_pinout_types.hpp"

// ==========================================
// SYSTEM & TIMING CONTEXT
// ==========================================
constexpr uint32_t USB_BAUD_RATE      = 2000000;

// FreeRTOS Task Frequencies (in Hz)
constexpr uint8_t TASK_MOTOR_FREQ_HZ  = 100;
constexpr uint8_t TASK_IMU_FREQ_HZ    = 50;
constexpr uint8_t TASK_IR_FREQ_HZ     = 20;
constexpr uint8_t TASK_TOF_FREQ_HZ    = 50;
constexpr uint8_t TASK_SERVO_FREQ_HZ  = 50;
constexpr uint8_t TASK_COMMS_FREQ_HZ  = 100;

// ==========================================
// HARDWARE CONSTRAINTS
// ==========================================
constexpr int16_t MOTOR_PWM_MAX       = 255;

constexpr uint16_t SERVO_MIN_ANGLE    = 0;
constexpr uint16_t SERVO_MAX_ANGLE    = 180;
constexpr uint16_t SERVO_SWEEP_DURATION_MS = 2000; // Time for a full sweep from min to max angle

constexpr uint16_t TOF_MAX_DISTANCE_MM = 8000; // Maximum distance the ToF sensor can measure

constexpr uint16_t TOF_OFFSET_MM = 61; // Offset to account for the physical placement of the ToF sensor

// ==========================================
// PINOUT ALLOCATIONS
// ==========================================

constexpr RobotPinout ROBOT_PINOUT = {
    .motors = {
        [0] = { .forward_pin = 13, .reverse_pin = 12, .pwm_pin = 16, .pwm_channel = 0 }, // Left Motor
        [1] = { .forward_pin = 14, .reverse_pin = 27, .pwm_pin = 17, .pwm_channel = 1 }  // Right Motor
    },

    .i2c = {
        [0] = { .sda_pin = 33, .scl_pin = 5 }, // IMU
        [1] = { .sda_pin = 21, .scl_pin = 22 }  // ToF
    },

    .tof_servo = { .pin = 18, .pwm_channel = 2 },

    .ir_sensor = {
        [0] = { .pin = 25 }, // Left IR
        [1] = { .pin = 26 }  // Right IR
    },

    .leds = {
        { .pin = 2 },  // Status LED (ESP32 Built-in)
        { .pin = 15 }  // User LED (ROS 2 controlled)
    }
};

#endif // ROBOT_CONFIG_H