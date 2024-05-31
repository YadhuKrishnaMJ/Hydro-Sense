#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "BarackObama";
const char* password = "7395926901";

const char* pushbulletAccessToken = "o.fq6UFukjsfgG7Bd89THN3UG3eqILKB0a";

const int waterSensorPin = 36; 
const int ledPin = 26; 

void setup() {
  pinMode(waterSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  int waterLevel = digitalRead(waterSensorPin);

  if (waterLevel == HIGH) {
    Serial.println("Water detected!");
    digitalWrite(ledPin, HIGH); 
 
    sendNotification();
    delay(1000); 
  } else {
    digitalWrite(ledPin, LOW); 
  }
}

void sendNotification() {
 
  HTTPClient http;
  

  String notificationTitle = "Flood Detected in Bank Colony!";
  String notificationBody = "Bank Colony Flood Sensors has sensed Flooding! Please Plan and Relocate Accordingly";

  String requestBody = "{\"type\":\"note\",\"title\":\"" + notificationTitle + "\",\"body\":\"" + notificationBody + "\"}";


  http.begin("https://api.pushbullet.com/v2/pushes");
  http.addHeader("Authorization", "Bearer " + String(pushbulletAccessToken));
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(requestBody);


  if (httpResponseCode > 0) {
    Serial.print("Pushbullet notification sent! Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error sending Pushbullet notification. HTTP error code: ");
    Serial.println(httpResponseCode);
  }


  http.end();
}
