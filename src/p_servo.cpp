#include "p_servo.hpp"
#include "robot_config.hpp"
#include <ESP32Servo.h>

// The global target angle. 
// Task_ServoCalc updates this, and Task_ToFRead bundles it with distance measurements.
volatile uint16_t g_current_servo_angle = SERVO_MAX_ANGLE / 2;

// Static instance of the servo object
static Servo tof_servo;

void p_servo_init() {
    // Allocate hardware timers to prevent conflicts
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    
    // Standard analog servos expect a 50Hz update rate
    tof_servo.setPeriodHertz(50); 
    
    // Attach the servo using the config definitions for pin and pulse widths
    tof_servo.attach(ROBOT_PINOUT.tof_servo.pin);
}

void ServoWriteTimerCallback(TimerHandle_t xTimer) {
    // Pushes the latest calculated angle to the hardware at a fixed 50Hz
    tof_servo.write(g_current_servo_angle);
}

void Task_ServoCalc(void *pvParameters) {
    int8_t sweep_direction = 1; // 1 for increasing, -1 for decreasing
    
    // Calculate the delay needed per degree to match SERVO_SWEEP_DURATION_MS
    // e.g., 2000ms / 180 degrees = ~11ms per degree
    const TickType_t delay_per_degree = pdMS_TO_TICKS(SERVO_SWEEP_DURATION_MS / SERVO_MAX_ANGLE);

    for (;;) {
        // Increment or decrement the angle
        if (sweep_direction == 1) {
            g_current_servo_angle++;
            if (g_current_servo_angle >= SERVO_MAX_ANGLE) {
                sweep_direction = -1;
            }
        } else {
            g_current_servo_angle--;
            if (g_current_servo_angle <= SERVO_MIN_ANGLE) {
                sweep_direction = 1;
            }
        }

        // Yield the CPU until it's time to increment the next degree
        vTaskDelay(delay_per_degree);
    }
}