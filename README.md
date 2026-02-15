# Soil & Climate Monitoring System

An Arduino-based soil and environmental monitoring system that measures soil moisture, temperature, and humidity, displaying the data on an OLED screen and the Serial Monitor.

---

## Project Overview

This project uses:

- Soil Moisture Sensor (Analog - A0)
- DHT22 Sensor (Temperature & Humidity - D3)
- SSD1306 128x64 OLED Display (I2C)

The system continuously reads environmental data and updates every 2 seconds.

---

## Features

- Detects Dry, Moist, or Wet soil conditions  
- Displays temperature in °C  
- Displays humidity in %  
- Shows all readings on:
  - OLED display
  - Serial Monitor  
- Auto refresh every 2 seconds  

---

## Hardware Required

- Arduino / ESP8266 (NodeMCU compatible)
- Soil moisture sensor
- DHT22 temperature & humidity sensor
- SSD1306 OLED display (128x64, I2C)
- Jumper wires

---

## Pin Configuration

| Component            | Pin |
|----------------------|-----|
| Soil Sensor (Analog) | A0  |
| DHT22 Data           | D3  |
| OLED (I2C)           | 0x3C (default address) |

---

## Soil Condition Logic

| Analog Value | Soil Status |
|--------------|------------|
| > 800        | Dry        |
| 500 – 800    | Moist      |
| < 500        | Wet        |

---

## Libraries Used

Make sure to install the following libraries in Arduino IDE:

- Adafruit_GFX
- Adafruit_SSD1306
- DHT sensor library
- Wire

---

## How It Works

1. The system reads soil moisture from the analog sensor.
2. It classifies the soil condition (Dry, Moist, Wet).
3. The DHT22 sensor measures temperature and humidity.
4. All values are printed to:
   - Serial Monitor
   - OLED display
5. The loop repeats every 2 seconds.

---

## Applications

- Smart gardening
- Plant health monitoring
- Greenhouse monitoring
- Educational IoT projects

---

## Future Improvements

- Add WiFi data logging
- Integrate mobile app monitoring
- Add automatic irrigation control
- Store data in cloud database

---

### Author
Your Name Here
