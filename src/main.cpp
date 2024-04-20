#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const char* ssid = "ssid";
const char* password = "password";
const char* serverUrl = "http://url:port/route";

const int trigPin = 16;
const int echoPin = 5;
long duration;
float distanceCm = 0;
float distanceInch;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void getDistanceFromUltraSonic () {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;

  distanceCm = distanceCm;
  if (distanceCm <= 0) {
    distanceCm = 0;
  }
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
}

// send data to cloud
void getDataFromCloud(const String ultraSonicDistance) {
  HTTPClient http;

  String jsonData = "{\"id\": 1, \"name\": \"Street 1 sensor\", \"details\": \"" + String(ultraSonicDistance) + "\"}";

  Serial.print("Sending HTTP GET request to ");
  Serial.println(serverUrl);

  if (http.begin(serverUrl)) {
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.PUT(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response:");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Failed to connect to server");
  }
}

void loop() {
  Serial.println("heartbeat running");

  String ultraSonicDistance = String(distanceCm);
  
  getDistanceFromUltraSonic();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(ultraSonicDistance);
    getDataFromCloud(ultraSonicDistance);
  }

  delay(2000);
  // put your main code here, to run repeatedly:
}