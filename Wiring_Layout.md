
# 🔌 Smart Aqua Meter - Wiring Layout

This guide explains how to connect the hardware components used in the Smart Aqua Meter project.

---

## 📋 Pin Connections (Main ESP32)

| Component            | ESP32 Pin | Notes                             |
|----------------------|-----------|-----------------------------------|
| Flow Sensor 1        | GPIO 23   | Connected to flow sensor output  |
| Flow Sensor 2        | GPIO 22   | Connected to second flow sensor |
| DHT22 Sensor         | GPIO 21   | Data pin                          |
| Circuit Input Check  | GPIO 27   | Input pin to detect short circuit |
| LED Indicator        | GPIO 2    | Turns ON if short circuit detected |
| Relay Module         | GPIO 19   | Controls water valve or motor     |
| ESP-NOW Communication| Wi-Fi     | Works over internal Wi-Fi         |
| Blynk Communication  | Wi-Fi     | Requires 2.4 GHz Wi-Fi network    |

---

## 🔌 Power Supply

- Use a **regulated 5V power supply** for the ESP32 and sensors.
- Flow sensors and DHT22 typically operate at **3.3V or 5V** (check your model).
- Relay module may require **separate power source** (e.g., 5V) for higher current loads.

---

## 🌱 Moisture Nodes (Slave ESP32s via ESP-NOW)

Each moisture sensing ESP32 board should connect as follows:

| Component        | ESP32 Pin | Notes                          |
|------------------|-----------|--------------------------------|
| Moisture Sensor  | A0 or GPIO 34 | Analog input                 |
| Power            | 3.3V / 5V | Based on sensor requirement    |
| Ground           | GND       | Common ground with main board  |

These boards send data wirelessly to the main board using ESP-NOW protocol.

---

## ⚠️ Important Notes

- Ensure common ground between all ESP32 boards and sensors.
- Use pull-up resistors where needed (e.g., for DHT22 if unstable).
- Protect GPIOs with resistors (1kΩ–10kΩ) for sensitive signals.

---

## 🧰 Optional Components

- Water Level Sensor (Tank)
- GSM/LoRa module for remote connectivity
- OLED Display for local status

---

## 🖼️ Diagram (Recommended)

Consider drawing the full schematic using [Fritzing](https://fritzing.org/) or similar software for clarity.

---
