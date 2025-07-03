# Smart-Aqua-Meter-with-ESP-NOW
Smart Aqua Meter is an IoT-based water monitoring system using ESP32 and ESP-NOW for wireless communication. It detects pipe leakages, monitors flow, and sends alerts to the Blynk app. Ideal for homes and farms to prevent water wastage.

# 💧 Smart Aqua Meter

The **Smart Aqua Meter** is a smart IoT-based water management system built using **ESP32**, **ESP-NOW**, and **Blynk IoT**. It enables **real-time monitoring of water flow**, **pipe leakage detection**, **soil moisture sensing**, and **short-circuit alerts**, making it ideal for smart farming, home automation, and water conservation projects.

---

## 🚀 Features

- 🔄 **ESP-NOW Communication** for fast, low-power wireless data from slave moisture nodes
- 🌡️ **DHT22** sensor for ambient temperature and humidity monitoring
- 💧 **Dual Flow Sensors** for water usage tracking and leakage detection
- ⚠️ **Short Circuit Detection** using GPIO input
- 🔌 **Relay Control** based on sensor data (e.g., leakage or dry soil)
- 📱 **Blynk App Integration** for live monitoring and alerts on mobile

---

## 📶 Architecture Overview

```
[Slave Node 1] --->|
[Slave Node 2] --->|   (ESP-NOW)
[Slave Node 3] --->|        |
                   v        v
                [Main ESP32 Node] -----> Blynk Cloud -----> Mobile App
                 |      |       |
            Flow1   Flow2    DHT22
                          |
                      Relay & LED
```

---

## 🛠️ Hardware Requirements

- ✅ ESP32 (main board)
- ✅ ESP32 (up to 3 slave boards for moisture)
- ✅ DHT22 temperature & humidity sensor
- ✅ 2x Flow sensors (YF-S201 or similar)
- ✅ Relay module
- ✅ LED for circuit fault indication
- ✅ Power supply and wiring

---

## 📲 Blynk Setup

1. Create a new Blynk project using **Blynk IoT (new platform)**.
2. Use the following credentials:
   - **Template ID**: `TMPL3l-FmibDs`
   - **Template Name**: `AQUA METER`
   - **Auth Token**: *(Replace with your token if needed)*

3. Set up the following virtual pins:
   - `V0`: Temperature
   - `V1`: Humidity
   - `V2`: Flow Rate 1
   - `V3`: Flow Rate 2
   - `V4`: Moisture Sensor 1
   - `V5`: Moisture Sensor 2
   - `V6`: Moisture Sensor 3
   - `V8`: DHT sensor error

---

## 📦 Code Summary

- `ESP-NOW`: Handles incoming moisture data from slave boards.
- `DHT22`: Captures temperature and humidity.
- `Flow Sensors`: Monitors water flow and calculates leakages.
- `Relay Logic`: Activates relay if:
  - All moisture levels > 30%
  - Flow rate difference > 4 L/min (leakage)
  - Short circuit detected
- `Blynk`: Sends all sensor data to cloud and mobile interface.

---

## 🔄 How It Works

1. ESP32 connects to Wi-Fi and Blynk.
2. ESP-NOW slaves send moisture readings.
3. Flow sensors count pulses to measure flow rate.
4. DHT22 reads humidity and temperature.
5. All values are pushed to Blynk dashboard.
6. If any hazard (leak, dry soil, short) is detected:
   - Relay is triggered
   - Notification is sent via Blynk

---

## 🧠 Future Improvements

- Add water level sensor in tank
- Automate pump control using time slots
- Add OTA updates for ESP32 firmware

---

## 🔐 Credentials (Update Before Uploading)

```cpp
#define BLYNK_TEMPLATE_ID "TMPL3l-FmibDs"
#define BLYNK_TEMPLATE_NAME "AQUA METER"
#define BLYNK_AUTH_TOKEN "Oc2q6JNIT2R2KtR__9TJJ96OumGM5Q2-"

#define BLYNK_WIFI_SSID "YOUR_WIFI_SSID"
#define BLYNK_WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

---

## 🧪 Demo (Optional)

Include a screenshot of the Blynk app or a short GIF showing live sensor data and relay activation.

---

## 📄 License

MIT License — feel free to use and modify for educational and non-commercial purposes.

---
