#ifndef SBC_COMMS_HPP
#define SBC_COMMS_HPP

#include <stdint.h>
#include "bot_speak.h"

/**
 * @brief Uses the BotSpeak frame ID to route the packet to the appropriate parser.
 * @param frame Pointer to the unpacked BotSpeak DataFrame.
 * @return 0 on success, or a negative error code if the ID is unrecognized.
 */
int comms_parse_frame_id(DataFrame_TypeDef* frame);

/**
 * @brief Parses incoming data requests, fetches the data from the global state, 
 * packages it into a BotSpeak response frame, and transmits it over Serial.
 * @param frame Pointer to the unpacked BotSpeak DataFrame.
 * @return 0 on success, or a negative error code if the ID is unrecognized.
 */
int comms_parse_request_frame(DataFrame_TypeDef* frame);

/**
 * @brief Parses incoming commands (like target motor PWMs) and updates the global state.
 * @param frame Pointer to the unpacked BotSpeak DataFrame.
 * @return 0 on success, or a negative error code if the ID is unrecognized.
 */
int comms_parse_command_frame(DataFrame_TypeDef* frame);

#endif // SBC_COMMS_HPP