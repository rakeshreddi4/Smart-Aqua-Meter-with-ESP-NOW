#define BLYNK_TEMPLATE_ID "TMPL3l-FmibDs"
#define BLYNK_TEMPLATE_NAME "AQUA METER"
#define BLYNK_AUTH_TOKEN "Oc2q6JNIT2R2KtR__9TJJ96OumGM5Q2-"
#define BLYNK_PRINT serial

#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

// WiFi Credentials
#define BLYNK_WIFI_SSID "NOTHING"
#define BLYNK_WIFI_PASSWORD "0987654321"

// Pin Definitions
#define SENSOR1 23
#define SENSOR2 22
#define DHT_PIN 21
#define DHT_TYPE DHT22
const int inputPin = 27;
const int ledPin = 2;
const int relayPin = 19;

// DHT Sensor Setup
DHT dht(DHT_PIN, DHT_TYPE);

// Flow Sensor Variables
float calibrationFactor = 4.5;
volatile byte pulseCount1 = 0, pulseCount2 = 0;
float flowRate1 = 0.0, flowRate2 = 0.0;
long previousMillis = 0;
const int interval = 1600; // Interval for flow rate calculation (ms)

// ESP-NOW Data Structure
typedef struct struct_message {
  int id;           // Unique ID for sender
  int moisture;     // Moisture reading
  char text[50];    // Additional text data
} struct_message;

// Instances for ESP-NOW
struct_message readings;
struct_message boardsStruct[3];

// ESP-NOW Receive Callback
void OnDataRecv(const esp_now_recv_info* info, const uint8_t* incomingData, int len) {
  memcpy(&readings, incomingData, sizeof(readings));
  boardsStruct[readings.id - 1].moisture = readings.moisture;
  strncpy(boardsStruct[readings.id - 1].text, readings.text, sizeof(boardsStruct[readings.id - 1].text) - 1);
  boardsStruct[readings.id - 1].text[sizeof(boardsStruct[readings.id - 1].text) - 1] = '\0';

  Serial.printf("Board ID %d: Moisture = %d%%, Message = %s\n",
                readings.id, readings.moisture, boardsStruct[readings.id - 1].text);
}

// Flow Sensor Interrupt Handlers
void IRAM_ATTR pulseCounter1() { pulseCount1++; }
void IRAM_ATTR pulseCounter2() { pulseCount2++; }

// Function to send data to Blynk
void sendToBlynk() {
  // Read temperature and humidity from the DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    Blynk.virtualWrite(V0, temperature); // Send temperature
    Blynk.virtualWrite(V1, humidity);    // Send humidity
  } else {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    Blynk.virtualWrite(V8, "DHT Error"); // Notify error in Blynk
  }

  // Send flow rates
  Blynk.virtualWrite(V2, flowRate1); // Flow rate 1
  Blynk.virtualWrite(V3, flowRate2); // Flow rate 2

  // Send moisture levels from slave boards
  for (int i = 0; i < 3; i++) {
    int virtualPin = V4 + i; // Assign V4, V5, V6 for boards 1, 2, 3
    Blynk.virtualWrite(virtualPin, boardsStruct[i].moisture);
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Connect to WiFi
  WiFi.begin(BLYNK_WIFI_SSID, BLYNK_WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, BLYNK_WIFI_SSID, BLYNK_WIFI_PASSWORD);

  // Initialize ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // Setup flow sensors
  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SENSOR1), pulseCounter1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR2), pulseCounter2, FALLING);

  // Setup circuit input and output
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin, LOW);
}

void loop() {
  static unsigned long lastBlynkUpdate = 0;
  unsigned long currentMillis = millis();

  // Update Blynk data every 2 seconds
  if (currentMillis - lastBlynkUpdate > 2000) {
    sendToBlynk();
    lastBlynkUpdate = currentMillis;
  }

  // Flow rate calculations
  if (currentMillis - previousMillis > interval) {
    flowRate1 = (pulseCount1 * (1000.0 / interval)) / calibrationFactor;
    flowRate2 = (pulseCount2 * (1000.0 / interval)) / calibrationFactor;
    pulseCount1 = pulseCount2 = 0;
    previousMillis = currentMillis;

    Serial.printf("Flow rate 1: %.2f L/min, Flow rate 2: %.2f L/min\n", flowRate1, flowRate2);
    delay(1500);
    if (abs(flowRate1 - flowRate2) > 4) {
      Serial.println("Leakage detected!");
      delay(2000);
    }
  }

  // Read and display DHT sensor values
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
  } else {
    Serial.printf("Humidity: %.1f%%, Temperature: %.1f°C\n", humidity, temperature);
    delay(2000);
  }

  // Circuit state check
  int wireState = digitalRead(inputPin);
  if (wireState == LOW) {
    Serial.println("Short Circuit Detected!");
    digitalWrite(ledPin, HIGH);
    delay(2000);
  } else {
    Serial.println("Circuit Normal");
    digitalWrite(ledPin, LOW);
    delay(2000);
  }

  // Moisture condition evaluation
  bool moistureCondition = true;
  for (int i = 0; i < 3; i++) {
    if (boardsStruct[i].moisture <= 30) {
      moistureCondition = false;
      break;
    }
  }

  // Activate relay based on conditions
  if (moistureCondition || wireState == LOW || abs(flowRate1 - flowRate2) > 4) {
    digitalWrite(relayPin, HIGH);
    digitalWrite(V0, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
    digitalWrite(V0, LOW);
  }

  // Run Blynk
  Blynk.run();
}