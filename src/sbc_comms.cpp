#include "sbc_comms.hpp"
#include "jr_bot_messages.hpp"
#include "robot_state_types.hpp"
#include <Arduino.h>

// Extern the global state to read sensor data and write motor commands
extern RobotState g_robot_state;

// Max payload size from our definitions (IMU data is largest at 12 bytes)
constexpr uint8_t MAX_PAYLOAD_BUFFER_SIZE = 32;
constexpr uint8_t MAX_TX_BUFFER_SIZE = 64;

// ----------------------------------------------------------------------------
// Private Helper
// ----------------------------------------------------------------------------

/**
 * @brief Packs a DataFrame into a raw byte array and blasts it out over the UART.
 */
static void comms_usb_tx(DataFrame_TypeDef* frame_to_transmit) {
    static uint8_t tx_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t packed_length = 0;

    // Pack the frame. If successful, push it directly to the hardware Serial buffer.
    if (botSpeak_packFrame(frame_to_transmit, tx_buffer, &packed_length) == 0) {
        Serial.write(tx_buffer, packed_length);
    }
}

// ----------------------------------------------------------------------------
// Public Parsers
// ----------------------------------------------------------------------------

int comms_parse_frame_id(DataFrame_TypeDef* frame) {
    int status = 0;

    switch (frame->frameID) {
        case 0x00 ... 0x1F:
            // Config frames (Skipped for now)
            break;

        case 0x20 ... 0x3F:
            status = comms_parse_request_frame(frame);
            break;

        case 0x40 ... 0x5F:
            // Response frames (SBC to ESP32 responses usually aren't needed here)
            break;

        case 0x60 ... 0x7F:
            status = comms_parse_command_frame(frame);
            break;

        case 0x80 ... 0x9F:
            // Emergency frames (Skipped for now)
            break;

        default:
            status = -1; // Invalid ID range
            break;
    }

    return status;
}

int comms_parse_request_frame(DataFrame_TypeDef* frame) {
    DataFrame_TypeDef tx_frame;
    uint8_t payload_bytes[MAX_PAYLOAD_BUFFER_SIZE];
    uint32_t serialized_length = 0;
    
    // Intermediate buffers for strict-type serialization
    float float32_buf[3];
    uint16_t uint16_buf[2];
    bool bool_buf[2];

    switch (frame->frameID) {
        case REQUEST_TOF_STATE:
            tx_frame.frameID = RESPONSE_TOF_STATE;
            tx_frame.timestamp = g_robot_state.tof.timestamp_ms;
            
            uint16_buf[0] = g_robot_state.tof.distance_mm;
            uint16_buf[1] = g_robot_state.tof.servo_angle_deg;
            
            botSpeak_serialize(uint16_buf, UNSERIALIZED_TOF_STATE_SIZE, sizeof(uint16_t), payload_bytes, &serialized_length);
            break;

        case REQUEST_IR_STATES:
            tx_frame.frameID = RESPONSE_IR_STATES;
            // Assuming both IRs update simultaneously, we use the timestamp of the left one
            tx_frame.timestamp = g_robot_state.irs[0].timestamp_ms; 
            
            bool_buf[0] = g_robot_state.irs[0].is_triggered;
            bool_buf[1] = g_robot_state.irs[1].is_triggered;
            
            botSpeak_serialize(bool_buf, UNSERIALIZED_IR_STATES_SIZE, sizeof(bool), payload_bytes, &serialized_length);
            break;

        case REQUEST_IMU_GYRO_RAW:
            tx_frame.frameID = RESPONSE_IMU_GYRO_RAW;
            tx_frame.timestamp = g_robot_state.imu.timestamp_ms;
            
            float32_buf[0] = g_robot_state.imu.gyro_x;
            float32_buf[1] = g_robot_state.imu.gyro_y;
            float32_buf[2] = g_robot_state.imu.gyro_z;
            
            botSpeak_serialize(float32_buf, UNSERIALIZED_IMU_GYRO_RAW_SIZE, sizeof(float), payload_bytes, &serialized_length);
            break;

        case REQUEST_IMU_ACCEL_RAW:
            tx_frame.frameID = RESPONSE_IMU_ACCEL_RAW;
            tx_frame.timestamp = g_robot_state.imu.timestamp_ms;
            
            float32_buf[0] = g_robot_state.imu.accel_x;
            float32_buf[1] = g_robot_state.imu.accel_y;
            float32_buf[2] = g_robot_state.imu.accel_z;
            
            botSpeak_serialize(float32_buf, UNSERIALIZED_IMU_ACCEL_RAW_SIZE, sizeof(float), payload_bytes, &serialized_length);
            break;

        default:
            return -1; // Unrecognized Request ID
    }

    // Bind the serialized payload to the frame and transmit
    tx_frame.data = payload_bytes;
    tx_frame.dataLength = serialized_length;
    comms_usb_tx(&tx_frame);

    return 0;
}

int comms_parse_command_frame(DataFrame_TypeDef* frame) {
    uint32_t deserialized_elements = 0;
    int16_t pwm_buf[2];

    switch (frame->frameID) {
        case MOTOR_DESIRED_PWM:
            // Extract the 4 payload bytes into two 16-bit integers
            botSpeak_deserialize(pwm_buf, &deserialized_elements, sizeof(int16_t), frame->data, frame->dataLength);
            
            // Map index 0 to Left (motors[0]) and index 1 to Right (motors[1])
            if (deserialized_elements == UNSERIALIZED_MOTOR_DESIRED_PWM_SIZE) {
                g_robot_state.motors[0].target_pwm = pwm_buf[0];
                g_robot_state.motors[1].target_pwm = pwm_buf[1];
            }
            break;

        default:
            return -1; // Unrecognized Command ID
    }

    return 0;
}