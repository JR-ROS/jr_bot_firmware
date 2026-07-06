#include "p_tof.hpp"
#include "robot_config.hpp"
#include "robot_state_types.hpp"
#include "p_led.hpp"

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
        g_status_led_delay_ms = LED_ERROR_DELAY;
    }

    tof_sensor.startContinuous();
    
    // Set timing budget to 20ms to ensure we can hit the 50Hz (20ms) task rate
    // tof_sensor.setMeasurementTimingBudget(20000); 
}

void Task_ToFRead(void *pvParameters) {
    for (;;) {
        // 1. Block for measurement
        uint16_t distance = tof_sensor.readRangeContinuousMillimeters() - TOF_OFFSET_MM;
        
        // 2. Capture the exact acquisition time and angle
        uint32_t acquired_time = millis(); 
        uint16_t acquired_angle = g_current_servo_angle;
        
        if (tof_sensor.timeoutOccurred() || distance > TOF_MAX_DISTANCE_MM || distance < 0) {
            distance = TOF_MAX_DISTANCE_MM;
        }

        // 3. Write all synchronized data to the global state
        g_robot_state.tof.distance_mm = distance;
        g_robot_state.tof.servo_angle_deg = acquired_angle;
        g_robot_state.tof.timestamp_ms = acquired_time;
        

        // Block until it's time for the next 50Hz read
        vTaskDelay(pdMS_TO_TICKS(1000 / TASK_TOF_FREQ_HZ));
    }
}