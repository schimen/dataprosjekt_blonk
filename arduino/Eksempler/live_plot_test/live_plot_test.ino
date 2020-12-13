#include <Arduino-idIOT.h>

const char* ssid = "PongGang"; //Enter SSID
const char* password = "Pong1234"; //Enter Password

const int interval = 500;
const int potpin = 34;

idIOT connection("ws://192.168.137.95:8000");

void sendVal()
{
  String message = "send;potVal;";
  int potVal = analogRead(potpin);
  message += String(potVal);
  connection.send(message);
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);
    
  while (!connection.connectWiFi(ssid, password)) {
    Serial.println("WiFi connection failed, retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  
  connection.connectServer();
  delay(50);
  connection.send("lagre;potVal;STOP");
}

void loop()
{
  static int previousMillis = millis();
  if (millis() - previousMillis > interval) {
    sendVal();
    previousMillis = millis();
    connection.update();
  }
}
