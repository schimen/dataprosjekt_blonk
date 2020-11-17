#include "SocketPong.h"

const char* ssid = "LAN Solo"; //Enter SSID
const char* password = "Steroidegutta6"; //Enter Password
const char* websockets_server = "ws://192.168.10.104:8000"; //server adress and port

SocketPong socketPong("abc");

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
  socketPong.connectServer(websockets_server);
}

void loop() {
  socketPong.sendAndUpdate();
}
