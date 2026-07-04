#ifndef ROBOT_STATES_H
#define ROBOT_STATES_H

#include <stdint.h>

// Represents the state of a single brushed DC motor
typedef struct {
    int16_t target_pwm;   // The commanded speed (-255 to 255)
    int16_t current_pwm;  // The actual applied speed (useful for ramping/smoothing)
    uint32_t timestamp_ms;
} MotorState;

// Represents the raw data from the MPU6050
typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    uint32_t timestamp_ms;
} IMUState;

// Represents a single point in the sweeping ToF scan
typedef struct {
    uint16_t distance_mm;
    uint16_t servo_angle_deg;
    uint32_t timestamp_ms;
} TOFState;

// Represents the state of a single IR cliff/collision sensor
typedef struct {
    bool is_triggered;
    uint32_t timestamp_ms;
} IRState;

// Represents the state of a single user-defined LED
typedef struct {
    bool is_on;
    uint32_t timestamp_ms; 
} LEDState;

// The global state of the robot, to be protected by a FreeRTOS Mutex
typedef struct {
    MotorState motors[2];
    IMUState imu;
    TOFState tof;
    IRState irs[2];
    LEDState leds[2];
    uint32_t timestamp_ms; // System time of the last full update
} RobotState;

#endif // ROBOT_STATES_H