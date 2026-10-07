# Smart Access & Environmental Monitoring System

**Arduino Mega · Embedded Systems · 2024**

## Project Overview

The Smart Access & Environmental Monitoring System is an Arduino Mega-based embedded system that combines secure access control with environmental and security monitoring.

The system uses multiple authentication and sensing mechanisms to control access, detect potential intrusions, automatically trigger the door when an object is detected nearby, and monitor environmental conditions.

## Key Features

* RFID-based access authentication
* PIN-based authentication using a 4×4 keypad
* Servo-controlled door locking and unlocking
* Automatic door locking after successful authentication
* PIR-based motion and intrusion detection
* Ultrasonic sensor-based automatic door triggering
* Temperature and humidity monitoring using a DHT11
* Light-level monitoring
* Water-level monitoring
* LCD system status and environmental display
* LED indicators for access status
* Buzzer alerts for authentication and security events

## System Components

| Component           | Purpose                                |
| ------------------- | -------------------------------------- |
| Arduino Mega        | Main system controller                 |
| MFRC522 RFID Reader | RFID authentication                    |
| 4×4 Keypad          | PIN authentication                     |
| Servo Motor         | Door locking mechanism                 |
| 16×2 LCD            | User interface and system information  |
| PIR Sensor          | Motion/intrusion detection             |
| Ultrasonic Sensor   | Proximity-based automatic door trigger |
| DHT11               | Temperature and humidity monitoring    |
| Light Sensor        | Light-level monitoring                 |
| Water-Level Sensor  | Water-level monitoring                 |
| LEDs                | Access and alert indicators            |
| Buzzer              | Audio alerts                           |

## How the System Works

### 1. Authentication

The system provides two authentication methods:

* **RFID authentication** — an RFID card is checked against an authorized UID.
* **PIN authentication** — a user enters a PIN through the keypad.

If authentication succeeds, the servo moves to the unlocked position and the green LED indicates successful access.

After approximately three seconds, the system automatically returns the door to the locked position.

### 2. Security Monitoring

The PIR sensor continuously monitors for motion.

When motion is detected, the system:

1. Displays an intrusion alert on the LCD.
2. Activates the red LED.
3. Activates the buzzer.
4. Produces repeated warning signals.
5. Returns the display to the system-ready state.

### 3. Automatic Door Trigger

The ultrasonic sensor measures the distance of nearby objects.

When an object is detected within approximately 20 cm, the system can automatically unlock the door for a short period before locking it again.

### 4. Environmental Monitoring

The system periodically collects environmental readings from the connected sensors.

It monitors:

* Temperature
* Humidity
* Light level
* Water level

The measurements are displayed on the LCD.

## Authentication and Access Control

The system combines electronic authentication with physical access control.

```text
RFID Card ──────┐
                ├──> Arduino Mega ───> Servo Door Lock
PIN Keypad ─────┘          │
                           ├──> LCD
                           ├──> LEDs
                           └──> Buzzer
```

## Security and Monitoring

```text
PIR Sensor ────────> Motion Detection ────> Alert

Ultrasonic Sensor ─> Proximity Detection ─> Auto Door

DHT11 ─────────────> Temperature/Humidity
Light Sensor ──────> Light Level
Water Sensor ──────> Water Level
```

## Technologies

* Arduino Mega
* C/C++
* Arduino SPI
* MFRC522 RFID
* Keypad library
* Servo library
* LiquidCrystal
* DHT sensor library
* Embedded systems
* Sensor integration
* Hardware interfacing

## Project Structure

```text
Smart-Access-Environmental-Monitoring/
│
├── README.md
│
└── arduino/
    └── smart_access_monitoring.ino
```

## Project Status

**Completed academic embedded-systems project — 2025**

The project was developed as an Arduino Mega-based embedded systems implementation. The repository preserves the Arduino source code and technical documentation for portfolio and demonstration purposes.

## Learning Outcomes

This project provided practical experience with:

* Microcontroller programming
* Hardware and sensor integration
* Embedded C/C++ programming
* RFID authentication
* Keypad interfaces
* Servo motor control
* Digital and analog sensor input
* LCD interfaces
* Security alert mechanisms
* Real-time sensor monitoring
* Embedded system design

## Author

**Naivete Thandiwe Makobe**

BSc Computer Engineering · 2026
