#ifndef P_TOF_HPP
#define P_TOF_HPP

/**
 * @brief Initializes the VL53L0X sensor on the secondary I2C bus (Wire1).
 * @note This must be called in setup() before the FreeRTOS scheduler starts.
 */
void p_tof_init();

/**
 * @brief FreeRTOS task that continuously reads the ToF sensor and updates the global state.
 */
void Task_ToFRead(void *pvParameters);

#endif // P_TOF_HPP