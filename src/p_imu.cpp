#include "p_imu.hpp"
#include "robot_config.hpp"
#include "robot_state_types.hpp"

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>

extern RobotState g_robot_state;
static Adafruit_MPU6050 mpu;

void p_imu_init() {
    Wire.begin(ROBOT_PINOUT.i2c[0].sda_pin, ROBOT_PINOUT.i2c[0].scl_pin);

    if (!mpu.begin(0x68, &Wire, 0)) {
        // Handle initialization failure
    } else {
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G); 
        mpu.setGyroRange(MPU6050_RANGE_500_DEG);      
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);   
    }
}

void Task_IMURead(void *pvParameters) {
    sensors_event_t a, g, temp;

    // Initialize the absolute timer variable with the current hardware tick count
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Calculate the exact tick interval for our frequency (e.g., 50Hz = 20ms)
    const TickType_t xFrequency = pdMS_TO_TICKS(1000 / TASK_IMU_FREQ_HZ);

    for (;;) {
        // This blocks the task, but NOT the FreeRTOS Timer Daemon
        if (mpu.getEvent(&a, &g, &temp)) {
            
            uint32_t acquired_time = millis();
            
            g_robot_state.imu.accel_x = a.acceleration.x;
            g_robot_state.imu.accel_y = a.acceleration.y;
            g_robot_state.imu.accel_z = a.acceleration.z;
            
            g_robot_state.imu.gyro_x = g.gyro.x;
            g_robot_state.imu.gyro_y = g.gyro.y;
            g_robot_state.imu.gyro_z = g.gyro.z;
            
            g_robot_state.imu.timestamp_ms = acquired_time;
        }

        // Block exactly until the next frequency boundary
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}