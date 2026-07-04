#ifndef P_IMU_HPP
#define P_IMU_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/**
 * @brief Initializes the primary I2C bus (Wire) and the MPU6050 sensor.
 * @note This must be called in setup() before the FreeRTOS scheduler starts.
 */
void p_imu_init();

/**
 * @brief FreeRTOS task that continuously reads the IMU at the configured frequency,
 * applies the timestamp, and updates the global robot state.
 */
void Task_IMURead(void *pvParameters);

#endif // P_IMU_HPP