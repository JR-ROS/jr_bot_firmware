# JR Bot Conventions

BotSpeak is used for communication between the ROS 2 host and the ESP32 microcontroller in the educational AMR platform.

## Usage

Include the conventions header along with BotSpeak headers.

```c
#include "bot_speak.h"
#include "jr_bot_messages.h"

```

## ID ranges

| ID start | ID end | param type |
| --- | --- | --- |
| 0x00 | 0x1F | config |
| 0x20 | 0x3F | request |
| 0x40 | 0x5F | response |
| 0x60 | 0x7F | commands |
| 0x80 | 0x9F | emergency |

## Exact fields

| ID | param type | param subtype | size | physical unit | default | depends on this being ON | SETTING? |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0x01 | config | imu on/off | 1 bool | - | ON | - | true |
| 0x02 | config | imu rate | 1 uint_32 | Hz | 50 | imu on/off | true |
| 0x03 | config | motor reverse directions | 2 bool | - | [false, false] | - | false |
| 0x04 | config | tof sweep on/off | 1 bool | - | ON | - | true |
| 0x21 | request | imu gyro raw | 0 (no data bytes) | - | - | imu on/off | false |
| 0x22 | request | imu accel raw | 0 (no data bytes) | - | - | imu on/off | false |
| 0x23 | request | tof state | 0 (no data bytes) | - | - | tof sweep on/off | false |
| 0x24 | request | ir states | 0 (no data bytes) | - | - | - | false |
| 0x41 | response | imu gyro raw | 3 float_32 | rad / s | - | imu on/off | false |
| 0x42 | response | imu accel raw | 3 float_32 | m / s^2 | - | imu on/off | false |
| 0x43 | response | tof state | 2 uint_16 | mm, deg | - | tof sweep on/off | false |
| 0x44 | response | ir states | 2 bool | - | - | - | false |
| 0x60 | commands | motor desired pwm | 2 int_16 | effort | - | - | false |
| 0x61 | commands | user defined LED | 1 bool | - | - | - | false |
| 0x81 | emergency | emergency stop | 1 bool | - | - | - | false |
