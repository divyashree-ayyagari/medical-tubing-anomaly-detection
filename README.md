# Medical Tubing Liquid Flow Anomaly Detection System

An embedded system for real-time monitoring of liquid flow in medical tubing, designed to identify abnormal flow conditions and send remote alerts using IoT.

## Overview

This project implements a low-cost liquid flow monitoring prototype using an Arduino, ESP32, flow sensor, LCD, and LED indicators. It monitors flow behavior and identifies conditions such as normal flow, blockage, leakage, and air-bubble-related disturbances.

The system uses a moving-average filter to smooth sensor readings and displays the detected flow condition on an LCD. IoT connectivity enables notifications through the Blynk platform.

## Features

- Real-time liquid flow measurement using a flow sensor
- Moving-average filtering to reduce fluctuations in sensor readings
- Detection and indication of abnormal flow conditions
- LCD display for flow readings and system status
- LED indicators for visual alerts
- ESP32-based IoT connectivity
- Remote notifications through Blynk

## Hardware Components

- Arduino microcontroller
- ESP32 development board
- Liquid flow sensor
- 16×2 LCD
- LEDs and current-limiting resistors
- Breadboard and jumper wires
- Liquid tubing and supporting connections
- USB power and programming connections

## System Architecture

1. **Data acquisition:** The flow sensor generates pulses corresponding to liquid flow.
2. **Signal processing:** The Arduino counts sensor pulses and calculates flow readings.
3. **Noise reduction:** A moving-average filter smooths the readings.
4. **Condition detection:** The processed readings are used to identify and indicate flow conditions.
5. **Local alerts:** The LCD displays the readings and detected condition, while LEDs provide visual indications.
6. **IoT notifications:** The ESP32 enables remote notifications through Blynk.

## Demonstration

### Hardware Setup

![Hardware Setup]
<img width="960" height="1280" alt="WhatsApp Image 2026-10-20 at 8 10 08 PM" src="https://github.com/user-attachments/assets/592b1286-32d1-45a1-8e38-1fde297e4328" />

The prototype integrates the flow sensor, microcontrollers, LCD, and LED indicators.

## Software

- **Programming language:** Embedded C/C++ using the Arduino environment
- **Microcontroller platform:** Arduino
- **Wireless connectivity:** ESP32
- **IoT platform:** Blynk
- **Display:** Character LCD

## Repository Structure

```text
medical-tubing-anomaly-detection/
├── README.md
├── src/
│   └── flow_sensor_test.ino
└── images/
    ├── hardware-setup.jpg
    ├── normal-flow.jpg
    ├── air-bubble-detection.jpg
    ├── blockage-detection.jpg
    ├── leakage-detection.jpg
    └── blynk-notifications.jpg
```

## Potential Applications

The underlying concepts may be relevant to liquid-flow monitoring, embedded sensing, IoT-based alert systems, and fault detection. The current implementation is an academic prototype and is not validated or certified for clinical use.

## Future Improvements

- Quantitative evaluation of detection accuracy and false alarms
- More extensive testing under different flow conditions
- Improved calibration and sensor-noise handling
- Logging and visualization of flow measurements over time
- Further refinement of anomaly-detection logic

## Disclaimer

This project is intended for educational and experimental purposes only. It is not a medical device and must not be used to monitor or make decisions about actual patient treatment.

## Author

**Divyashree Ayyagari**

Electronics and Communication Engineering  
Vellore Institute of Technology, Vellore
