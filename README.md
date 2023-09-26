# ESP32S3 Smart Desk

The ESP32S3 Smart Desk is a project that aims to provide smart control over a desk setup using an ESP32S3 microcontroller. This project integrates with MQTT to control various devices such as relays and motors, allowing for remote control over Wi-Fi.

## 🌟 Features

- **Relay Control**: Control relays for devices like monitors, laptops, fans, and more.
- **Motor Control**: Control a motor to adjust the height of the desk.
- **Runtime Configuration**: Set specific runtimes for the motor to move the table up or down.
- **MQTT Feedback**: Receive feedback and error messages via MQTT.
- **Wi-Fi Integration**: Easily connect to Wi-Fi and MQTT server.

## 🛠 Setup

### Hardware Requirements

- ESP32S3 microcontroller
- Relay module (with at least 6 relays)
- Motor for adjusting the desk height
- Shift register (74HC595)
- Necessary jumper wires and breadboard

### Software Requirements

- Arduino IDE or PlatformIO
- PubSubClient library for MQTT
- ShiftRegister74HC595 library for controlling the shift register

### Wiring

- `SER_Pin`: 7
- `RCLK_Pin`: 5
- `SRCLK_Pin`: 6

Connect the shift register pins to the ESP32S3 as mentioned above. Connect the outputs of the shift register to the relay module.

### Configuration

1. Set your Wi-Fi SSID and password in the main code.
2. Set the MQTT server IP and port.
3. Adjust the relay and motor control pins if necessary.

## 🚀 Usage

Once the setup is complete, power on the ESP32S3. It will automatically connect to the specified Wi-Fi network and the MQTT server.

You can send MQTT messages to control the relays and the motor. Here are the available topics and payloads:

- `/relay/monitor`: "on" or "off"
- `/relay/laptop`: "on" or "off"
- `/relay/fan`: "on" or "off"
- `/relay/reserve2`: "on" or "off"
- `/motor/table`: "up", "down", or "stop"
- `/motor/runtimeup`: (integer value for runtime in milliseconds)
- `/motor/runtimedown`: (integer value for runtime in milliseconds)

For example, to turn on the monitor relay, send "on" to the `/relay/monitor` topic.

## 🛠 Troubleshooting

If you encounter any issues:

1. Ensure that the Wi-Fi and MQTT credentials are correct.
2. Check the wiring connections.
3. Monitor the serial output for any error messages or feedback.
4. Ensure that the MQTT server is running and reachable from the ESP32S3.

## 🤝 Contributing

Feel free to fork this repository and submit pull requests. Any contributions to improve the code or add new features are welcome!

