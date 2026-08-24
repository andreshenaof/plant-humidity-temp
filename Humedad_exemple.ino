#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


// === WIFI SETUP ===
const char* ssid = "WIFI";
const char* password = "PASSWORD";


// Adresse de ton serveur Node.js
const char* serverURL = "http://adresse_serveur_local";


// === OLED SETUP ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// === DHT SETUP ===
#define DHTPIN D3
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);


// === Sensor ID ===
// Identifiant unique du capteur
int sensor_id = 1;



// === Setup ===
void setup() {

  Serial.begin(115200);

  dht.begin();


  // ---------------------
  // Connexion WiFi
  // ---------------------

  WiFi.begin(ssid, password);

  Serial.print("Connexion WiFi");

  

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connecté !");

  Serial.print("Adresse IP ESP8266 : ");
  Serial.println(WiFi.localIP());

  Serial.println("Test port 3000 vers PC");

  WiFiClient client;

 if (client.connect("YOUR_SERVER_IP", 3000))
    Serial.println("Connexion TCP OK !");
    client.stop();
  } 
  else {
    Serial.println("Connexion TCP ECHEC !");
  }

  // ---------------------
  // OLED init
  // ---------------------

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {

    Serial.println(F("SSD1306 OLED allocation failed"));

    for (;;);
  }


  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.println("Soil Monitor");
  display.println("WiFi Connected");

  display.display();

  delay(2000);

}



// === Loop ===
void loop() {


  // ---------------------
  // Lecture capteurs
  // ---------------------

  // ---------------------
// Soil moisture calibration
// ---------------------

int soilValue = analogRead(A0);

// Calibration SENSOR
const int SOIL_DRY = 837;   // Air = 0%
const int SOIL_WET = 757;   // Water = 100%

int soilHumidity = map(
    soilValue,
    SOIL_DRY,
    SOIL_WET,
    0,
    100
);

soilHumidity = constrain(soilHumidity, 0, 100);

  float humidity = dht.readHumidity();

  float temperature = dht.readTemperature();



  // ---------------------
  // Serial Monitor
    // ---------------------
  Serial.print("Soil raw: ");
  Serial.print(soilValue);

  Serial.print(" -> Soil humidity: ");
  Serial.print(soilHumidity);
  Serial.println(" %");



  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("DHT Error!");

  } 
  else {

    Serial.print("Humidity: ");
    Serial.print(humidity);

    Serial.print(" % | Temp: ");

    Serial.print(temperature);

    Serial.println(" C");

  }



  // ---------------------
  // OLED Display
  // ---------------------

  display.clearDisplay();

  display.setCursor(0,0);

  display.println("Soil Monitor");


  display.setCursor(0,16);

  display.print("Soil: ");
  display.print(soilHumidity);
  display.println(" %");



  if (!isnan(humidity) && !isnan(temperature)) {


    display.setCursor(0,32);

    display.print("Temp: ");
    display.print(temperature,1);
    display.println(" C");


    display.setCursor(0,48);

    display.print("Hum: ");
    display.print(humidity,1);
    display.println(" %");

  }


  display.display();



  // ---------------------
  // Envoi vers Node.js
  // ---------------------

  if (WiFi.status() == WL_CONNECTED &&
      !isnan(humidity) &&
      !isnan(temperature)) {


    WiFiClient client;

    HTTPClient http;


    http.begin(client, serverURL);

    http.addHeader(
      "Content-Type",
      "application/json"
    );


    String json = "{";

    json += "\"sensor_id\":";
    json += sensor_id;
    json += ",";


    json += "\"soil\":";
    json += soilHumidity;
    json += ",";


    json += "\"temperature\":";
    json += temperature;
    json += ",";


    json += "\"humidity\":";
    json += humidity;


    json += "}";


    Serial.println("Sending JSON:");
    Serial.println(json);



    Serial.println("Connexion au serveur...");

    int httpCode = http.POST(json);

    Serial.print("HTTP Response: ");
    Serial.println(httpCode);

    if (httpCode < 0) {
        Serial.println(http.errorToString(httpCode));
    }



    http.end();

  }


  delay(2000);

}