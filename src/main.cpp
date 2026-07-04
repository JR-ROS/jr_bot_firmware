#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"

#include "robot_config.hpp"
#include "robot_state_types.hpp"
#include "robot_pinout_types.hpp"

#include "p_tof.hpp"
#include "p_servo.hpp"
#include "p_motors.hpp"
#include "p_ir.hpp"
#include "p_uart.hpp"
#include "p_led.hpp"
#include "p_imu.hpp"

#include "sbc_comms.hpp"

// ==========================================
// GLOBALS & RTOS HANDLES
// ==========================================

RobotState g_robot_state = {0};

// Queues
QueueHandle_t rx_packet_queue;

// Timers
TimerHandle_t MotorWriteTimer;
TimerHandle_t ServoWriteTimer;
TimerHandle_t CommsRxTimer;
TimerHandle_t IRSensorTimer;

// Tasks
TaskHandle_t ServoCalcTask;
TaskHandle_t TofReadTask;
TaskHandle_t CommsParseTask;
TaskHandle_t StatusLEDTask;
TaskHandle_t ImuReadTask;

// ==========================================
// ARDUINO SETUP & LOOP
// ==========================================

void setup() {
    // 1. Hardware Initialization
    p_motors_init();
    p_servo_init();
    p_tof_init();
    p_ir_init();
    p_uart_init();
    p_led_init();
    p_imu_init();

    // 2. Queue Creation
    rx_packet_queue = xQueueCreate(5, MAX_PACKET_BUFFER_SIZE);

    // 3. Timer Creation
    MotorWriteTimer = xTimerCreate("MotorTmr", pdMS_TO_TICKS(1000 / TASK_MOTOR_FREQ_HZ), pdTRUE, (void *)0, MotorWriteTimerCallback);
    ServoWriteTimer = xTimerCreate("ServoWrTmr", pdMS_TO_TICKS(1000 / TASK_SERVO_FREQ_HZ), pdTRUE, (void *)0, ServoWriteTimerCallback);
    IRSensorTimer = xTimerCreate("IRTmr", pdMS_TO_TICKS(1000 / TASK_IR_FREQ_HZ), pdTRUE, (void *)0, IRSensorTimerCallback);
    CommsRxTimer = xTimerCreate("CommsRxTmr", pdMS_TO_TICKS(1000 / TASK_COMMS_FREQ_HZ), pdTRUE, (void *)0, CommsRxTimerCallback);

    // 4. Task Creation
    xTaskCreate(Task_ServoCalc, "ServoCalc", 2048, NULL, 1, &ServoCalcTask);
    xTaskCreate(Task_ToFRead, "TofRead", 2048, NULL, 1, &TofReadTask);
    xTaskCreate(Task_StatusLED, "StatusLED", 1024, NULL, 1, &StatusLEDTask); // No need to keep a handle for the status LED task
    
    xTaskCreate(Task_CommsParse, "CommsParse", 4096, NULL, 2, &CommsParseTask); 
    xTaskCreate(Task_IMURead, "ImuRead", 4096, NULL, 2, &ImuReadTask);

    // 5. Start Execution
    xTimerStart(MotorWriteTimer, 0);
    xTimerStart(ServoWriteTimer, 0);
    xTimerStart(IRSensorTimer, 0);
    xTimerStart(CommsRxTimer, 0);
    
    // The FreeRTOS scheduler starts automatically after setup() in the ESP32 Arduino Core
}

void loop() {
    // Delete the Arduino loop task to free up memory/CPU since everything is RTOS-driven
    vTaskDelete(NULL); 
}