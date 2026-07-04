#include "p_tof.hpp"
#include "robot_config.hpp"
#include "robot_state_types.hpp"

#include <Wire.h>
#include <VL53L0X.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Extern the global state variables so this task can update them
extern RobotState g_robot_state;
// We will define this in the servo peripheral file later
extern volatile uint16_t g_current_servo_angle; 

// Static instance of the Pololu sensor object
static VL53L0X tof_sensor;

void p_tof_init() {
    // Initialize the ESP32's secondary hardware I2C bus
    Wire1.begin(ROBOT_PINOUT.i2c[1].sda_pin, ROBOT_PINOUT.i2c[1].scl_pin);
    
    // Bind the Pololu library to Wire1 instead of the default Wire
    tof_sensor.setBus(&Wire1);
    
    // Set a timeout so the task doesn't hang indefinitely on a disconnected wire
    tof_sensor.setTimeout(100); 
    
    if (!tof_sensor.init()) {
        // Silently fail for the course kit, but real robots would log an error here
    }
    
    // Set timing budget to 20ms to ensure we can hit the 50Hz (20ms) task rate
    tof_sensor.setMeasurementTimingBudget(20000); 
}

void Task_ToFRead(void *pvParameters) {
    for (;;) {
        // This single-shot read is blocking. The Wire library will yield 
        // the CPU during I2C clock stretching, allowing other tasks to run.
        uint16_t distance = tof_sensor.readRangeSingleMillimeters();
        
        // Filter out bad data: timeouts or hardware noise beyond our stated max range
        if (tof_sensor.timeoutOccurred() || distance > TOF_MAX_DISTANCE_MM) {
            distance = TOF_MAX_DISTANCE_MM;
        }
        
        // Write to the global state
        g_robot_state.tof.distance_mm = distance;
        g_robot_state.tof.servo_angle_deg = g_current_servo_angle;

        // Block until it's time for the next 50Hz read
        vTaskDelay(pdMS_TO_TICKS(1000 / TASK_TOF_FREQ_HZ));
    }
}