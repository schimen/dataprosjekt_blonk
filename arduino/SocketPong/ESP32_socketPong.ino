#include "SocketPong.h"

const char* ssid = "PongGang"; //Enter SSID
const char* password = "Pong1234"; //Enter Password

const int potPin = 34; 
int potValue = 0;

SocketPong socketPong("ws://192.168.137.174:8000");

void setup() {
  Serial.begin(115200);
  // Connect to wifi
  while (!socketPong.connectWiFi(ssid, password)){
    Serial.println("WiFi connection failed, retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");


  // Setup Callbacks
  socketPong.onMsg();
  socketPong.onEvnt();

  // Connect to server
  socketPong.connectServer();
  socketPong.send("lytt;potVerdi;START");
}

void loop() {
  socketPong.sendAndUpdate();
  potValue = socketPong.increaseCheck(potPin, potValue);
}
