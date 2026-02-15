#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// === OLED SETUP ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C  // Change to 0x3D if needed

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === DHT SETUP ===
#define DHTPIN D3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// === Setup ===
void setup() {
  Serial.begin(115200);
  dht.begin();

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 OLED allocation failed"));
    for (;;); // Halt
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Soil Monitor"));
  display.display();
  delay(2000);
}

// === Loop ===
void loop() {
  // Read soil moisture
  int soilValue = analogRead(A0);

  String soilStatus;
  if (soilValue > 800) {
    soilStatus = "Dry🌵";
  } else if (soilValue > 500) {
    soilStatus = "Moist🌱";
  } else {
    soilStatus = "Wet💧";
  }

  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Display on Serial
  Serial.print("Soil: "); Serial.print(soilValue); Serial.print(" -> "); Serial.println(soilStatus);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: "); Serial.print(humidity); Serial.print(" % | ");
    Serial.print("Temp: "); Serial.print(temperature); Serial.println(" °C");
  }

  // Display on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Soil Monitor");

  display.setCursor(0, 16);
  display.print("Soil: ");
  display.print(soilValue);
  display.print(" ");
  display.println(soilStatus);

  if (isnan(humidity) || isnan(temperature)) {
    display.setCursor(0, 32);
    display.println("DHT Error!");
  } else {
    display.setCursor(0, 32);
    display.print("Temp: ");
    display.print(temperature, 1);
    display.println(" C");

    display.setCursor(0, 48);
    display.print("Hum:  ");
    display.print(humidity, 1);
    display.println(" %");
  }

  display.display();
  delay(2000);
}
