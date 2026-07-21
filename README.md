# JR Bot Firmware

ESP32-based robot firmware for an educational autonomous mobile robot (AMR) platform. It manages motor control, sensor I/O (IMU, ToF distance, IR collision), and bidirectional communication with a host ROS 2 system over USB serial using the BotSpeak protocol.

## Stack

- **Language(s):** C++
- **Framework / runtime:** Arduino (ESP32 + FreeRTOS)
- **Notable libraries:** FreeRTOS (task/timer management), Adafruit MPU6050 (IMU), VL53L0X (ToF distance sensor), BotSpeak (serialization protocol)

## Repository Structure

```
src/                    Implementations of peripheral drivers
  main.cpp              FreeRTOS setup: task/timer creation, hardware init
  p_motors.cpp          DC motor control via L293D H-bridge (PWM + direction)
  p_servo.cpp           Servo sweeping for ToF sensor
  p_imu.cpp             MPU6050 IMU (6-axis gyro + accel)
  p_tof.cpp             VL53L0X ToF distance sensing
  p_ir.cpp              IR cliff/collision detection
  p_uart.cpp            USB serial UART initialization
  p_led.cpp             Status and user-controlled LEDs
  sbc_comms.cpp         Message parsing: requests→sensor data, commands→motor/LED

include/                Header files and message/state definitions
  robot_config.hpp      Hardware pinout map, task frequencies, constraints
  robot_state_types.hpp Global state struct (motors, IMU, ToF, IR, LEDs)
  jr_bot_messages.hpp   BotSpeak frame IDs, message layouts
  p_*.hpp               Peripheral APIs

platformio.ini          PlatformIO build config (ESP32, Arduino framework)
docs/                   JR Bot communication conventions (BotSpeak message specs)
test/                   Unit test placeholder (PlatformIO)
```

## How It Works

The firmware is FreeRTOS-based. `main.cpp` spawns periodic tasks (ToF read, IMU read, servo calc, comms parse) and timers (motor write at 100 Hz, servo write at 50 Hz, IR at 20 Hz). A shared global `RobotState` struct holds all sensor readings and motor targets. The comms task parses incoming BotSpeak frames from the host: requests trigger sensor-data responses, and motor/LED commands update the shared state, which the timer callbacks then apply to hardware. Timestamps are tracked throughout for debugging and synchronization.

## Getting Started

### Prerequisites

- PlatformIO Core or PlatformIO IDE
- ESP32-DevKit board (or compatible)
- USB cable for flashing and serial communication

### Flash to ESP32

```bash
platformio run -e esp32dev --target upload
```

### Monitor Serial Output

```bash
platformio device monitor -b 2000000
```

The firmware communicates at 2 Mbps over USB serial using the BotSpeak protocol.

### Build Only (No Upload)

```bash
platformio run -e esp32dev
```

The `esp32dev` environment is pre-configured in `platformio.ini` with the ESP32-DevKit board, Arduino framework, and required libraries (BotSpeak, VL53L0X, Adafruit MPU6050).

## Communication Protocol

This firmware uses **BotSpeak**, a lightweight binary serialization protocol for communication between the ESP32 and a host ROS 2 system. Message types include:

- **Config frames** (0x00–0x1F): Configure sensor behavior and motor directions
- **Request frames** (0x20–0x3F): Request sensor data (IMU, ToF, IR)
- **Response frames** (0x40–0x5F): Sensor data responses
- **Command frames** (0x60–0x7F): Motor PWM and LED control
- **Emergency frames** (0x80–0x9F): Emergency stop

See `docs/jr-bot-conventions.md` for full message specifications.

## Hardware Configuration

Pin assignments and hardware constraints are defined in `include/robot_config.hpp`:

- **Motors:** Two DC motors with PWM control (L293D H-bridge)
- **IMU:** MPU6050 on I2C bus 0
- **ToF Sensor:** VL53L0X on I2C bus 1 with servo sweep
- **IR Sensors:** Two analog cliff/collision sensors
- **LEDs:** Status LED (built-in) and user-controlled LED
- **Serial:** USB UART at 2 Mbps

## Key Design Decisions

1. **FreeRTOS for real-time concurrency:** Tasks and timers ensure deterministic sensor reads and motor writes.
2. **Shared global state:** All subsystems access a thread-safe `RobotState` struct for synchronization.
3. **BotSpeak for host communication:** Efficient binary protocol minimizes latency and bandwidth.
4. **Modular peripheral drivers:** Each subsystem (motors, sensors, LEDs) is isolated in its own source file.

## Testing

Unit test infrastructure is provided by PlatformIO. Place tests in the `test/` directory and run with:

```bash
platformio test -e esp32dev
```

Currently, the test directory is a placeholder. Contributions are welcome!

## License

TBD

## Contributing

Contributions are welcome! Please follow the coding conventions defined in `docs/jr-bot-conventions.md`.
