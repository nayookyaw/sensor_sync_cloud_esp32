#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// put function declarations here:
int myFunction(int, int);

const char* ssid = "your-ssid";
const char* password = "your-password";
const char* serverUrl = "http://your-api-url.com/api/sensor/1";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  Serial.println("heartbeat running");
  delay(1000);
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}