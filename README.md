# ESP32-CAM-ROVER
The ESP32 Smart Rover is a wireless, microcontroller-based robotic vehicle designed for remote monitoring, control, and automation applications. The project uses an ESP32 development board as the main controller because of its built-in Wi-Fi and Bluetooth capabilities, making it suitable for IoT-based robotic systems.


### ESP32 Smart Rover

The **ESP32 Smart Rover** is a wireless, microcontroller-based robotic vehicle designed for remote monitoring, control, and automation applications. The project uses an **ESP32 development board** as the main controller because of its built-in Wi-Fi and Bluetooth capabilities, making it suitable for IoT-based robotic systems.

The rover consists of **DC geared motors, motor driver, wheels, battery supply, and sensors** integrated with the ESP32. The motor driver allows the ESP32 to control the direction and speed of the motors, enabling the rover to move **forward, backward, left, and right**. Wireless connectivity allows the rover to be controlled remotely through a smartphone or web-based interface.

The system can be further equipped with sensors such as **ultrasonic, infrared, temperature, or obstacle-detection sensors** to improve its autonomous capabilities. An ultrasonic sensor can be used to detect obstacles and help the rover avoid collisions while navigating its surroundings.

The ESP32 processes commands received through the wireless network and generates appropriate control signals for the motor driver. This demonstrates practical implementation of **embedded programming, GPIO control, PWM motor control, wireless communication, sensor interfacing, and IoT concepts**.

The rover can be extended with additional features such as **live camera streaming, autonomous navigation, obstacle avoidance, GPS tracking, voice control, and IoT-based monitoring**. Due to its flexible architecture, it can be used for educational purposes, surveillance prototypes, industrial inspection, and smart robotic applications.

This project provides hands-on experience in designing and developing an embedded robotic system while combining **electronics, programming, communication, and automation technologies** into a single practical application.


# PARTS-LIST

| S. No. | Component                   |  Qty. | Purpose                            |
| ------ | --------------------------- | ----: | ---------------------------------- |
| 1      | ESP32 Development Board     |     1 | Main controller                    |
| 2      | DC Geared Motors            |     4 | Drive the four wheels              |
| 3      | Robot Wheels                |     4 | Movement                           |
| 4      | L298N Motor Driver          |     1 | Controls motor direction and speed |
| 5      | Robot Chassis               |     1 | Mechanical body/frame              |
| 6      | Ultrasonic Sensor/ optional |     1 | Obstacle detection                 |
| 7      | Li-ion/LiPo Battery Pack    |     1 | Power supply                       |
| 8      | Battery Holder              |     1 | Holds batteries                    |
| 9      | 5V Buck Converter           |     1 | Provides regulated voltage         |
| 10     | Power ON/OFF Switch         |     1 | Main power control                 |
| 11     | Jumper Wires                | 1 set | Electrical connections             |
| 12     | PCB                         |     1 | Circuit mounting                   |
| 13     | Screws, Nuts & Spacers      | 1 set | Mechanical assembly                |
| 14     | USB Cable                   |     1 | ESP32 programming/power            |
| 15     | Buzzer / optional           |     1 | Warning/status indication          |
| 16     | LEDs                        |   2–4 | Status indication                  |


# 💻 Software Stack
Arduino IDE firmware
WebSocket / HTTP server running on ESP32 for streaming & control
HTML web interface

# ✨ Features
🎥 Live video streaming straight from the ESP32-CAM camera
🌐 Control & monitor your car via any web browser — no extra apps needed
⚙️ Simple, lightweight design — no pan-tilt, just raw power and smooth streaming
🔋 Battery-powered for full mobility
