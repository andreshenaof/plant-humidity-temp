# 🌱 Soil & Climate Monitoring System

An IoT-based soil and environmental monitoring system built with an **ESP8266**, designed to monitor plant conditions in real time.

The system collects **soil moisture, temperature, and humidity** data using sensors connected to the ESP8266. The readings are displayed locally on an **OLED screen** and sent over Wi-Fi to a **Node.js server**, where they can be stored and processed.

## Project Overview

The system is composed of three main parts:

* **ESP8266 / NodeMCU** — reads the sensors and communicates over Wi-Fi
* **Arduino code** — controls the sensors, OLED display, and data transmission
* **Node.js / Express server (`server.js`)** — receives and processes sensor data
* **MySQL database** — stores the collected measurements

### Data Flow

```text
Soil Sensor ─────┐
                 │
DHT22 ───────────┼──> ESP8266 ──Wi-Fi──> server.js ──> MySQL
                 │
OLED Display <───┘
```

The ESP8266 continuously collects sensor readings, displays them on the OLED screen, and sends the measurements to the backend server.

---

## Features

* 🌱 Soil moisture monitoring
* 🌡️ Temperature measurement
* 💧 Humidity measurement
* 📺 Real-time OLED display
* 📡 Wi-Fi communication
* 🖥️ Node.js / Express backend
* 🗄️ MySQL data storage
* 📊 Sensor data available for future dashboards and analysis
* 🔄 Automatic sensor updates

---

## Hardware

* ESP8266 / NodeMCU
* Capacitive soil moisture sensor
* DHT22 temperature & humidity sensor
* SSD1306 128×64 OLED display
* Jumper wires
* USB cable / power supply

## Pin Configuration

| Component            | ESP8266 Pin |
| -------------------- | ----------- |
| Soil Moisture Sensor | A0          |
| DHT22 Data           | D3          |
| OLED SDA             | D2          |
| OLED SCL             | D1          |
| OLED Address         | 0x3C        |

---

## Soil Condition Logic

The soil moisture sensor value is used to classify the soil condition:

| Analog Value | Soil Status |
| -----------: | ----------- |
|        > 800 | 🌵 Dry      |
|      500–800 | 🌱 Moist    |
|        < 500 | 💧 Wet      |

These thresholds can be adjusted depending on the sensor and the type of soil being monitored.

---

## Software

### ESP8266 / Arduino

The Arduino program is responsible for:

1. Connecting the ESP8266 to Wi-Fi.
2. Reading the soil moisture sensor.
3. Reading temperature and humidity from the DHT22.
4. Updating the OLED display.
5. Sending sensor data to the Node.js server.
6. Repeating the process at a defined interval.

### Node.js Server

The `server.js` application provides the backend API used by the ESP8266.

The server:

* Receives sensor measurements through HTTP requests.
* Processes incoming data.
* Connects to the MySQL database.
* Stores sensor measurements.
* Provides an API that can later be used by a web dashboard or other applications.

Example sensor data sent by the ESP8266:

```json
{
  "sensor_id": 1,
  "soil": 644,
  "temperature": 25.3,
  "humidity": 50.3
}
```

---

## Database

Sensor measurements are stored in a MySQL database.

A typical measurement contains:

| Field         | Description                  |
| ------------- | ---------------------------- |
| `id`          | Measurement ID               |
| `sensor_id`   | Sensor identifier            |
| `timestamp`   | Date and time of measurement |
| `soil`        | Soil moisture value          |
| `temperature` | Temperature in °C            |
| `humidity`    | Relative humidity in %       |

This makes it possible to keep a history of the plant's environmental conditions and analyze changes over time.

---

## Libraries

### Arduino

The ESP8266 project uses libraries such as:

* ESP8266WiFi
* ESP8266HTTPClient
* ArduinoJson
* Adafruit_GFX
* Adafruit_SSD1306
* DHT sensor library
* Wire

### Node.js

The backend uses:

* Express
* MySQL2
* dotenv
* Nodemailer

---

## Project Structure

```text
soil-climate-monitor/
│
├── arduino/
│   └── soil_climate_monitor.ino
│
├── server/
│   ├── server.js
│   └── .env
│
└── README.md
```

> The `.env` file contains private configuration such as database credentials, Wi-Fi/API settings, and should **not be committed to GitHub**.

---

## Future Improvements

* 📊 Web dashboard for real-time monitoring
* 📈 Historical graphs
* 🔔 Automatic plant-dryness alerts
* 📧 Email notifications
* 🌐 Remote monitoring
* 🚿 Automatic irrigation
* 📱 Mobile interface
* 🌱 Support for multiple sensors/plants

---

## Author

**Andres Henao**


