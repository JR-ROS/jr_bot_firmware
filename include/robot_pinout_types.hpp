#ifndef ROBOT_PINOUT_H
#define ROBOT_PINOUT_H

#include <stdint.h>

// Assuming a standard H-Bridge like L298N or TB6612FNG
typedef struct {
    uint8_t forward_pin;
    uint8_t reverse_pin;
    uint8_t pwm_pin;
    uint8_t pwm_channel; // ESP32 LEDC hardware channel
} MotorConfig;

// I2C bus configuration for IMU and ToF
typedef struct {
    uint8_t sda_pin;
    uint8_t scl_pin;
} I2CConfig;

// Servo driving the ToF sensor
typedef struct {
    uint8_t pin;
    uint8_t pwm_channel; // ESP32 LEDC hardware channel
} ServoConfig;

// Simple digital IR sensor
typedef struct {
    uint8_t pin;
} IRConfig;

typedef struct {
    MotorConfig motors[2];  // 0 is left, 1 is right
    I2CConfig i2c[2];  // 0 is IMU, 1 is ToF
    ServoConfig tof_servo;
    IRConfig ir_sensor[2];  // 0 is left, 1 is right
} RobotPinout;

#endif // ROBOT_PINOUT_H