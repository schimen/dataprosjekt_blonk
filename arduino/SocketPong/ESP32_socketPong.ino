#include "SocketPong.h"

const char* ssid = "PongGang"; //Enter SSID
const char* password = "Pong1234"; //Enter Password
String player;

SocketPong socketPong("ws://192.168.137.244:8000");

void setup() {
  Serial.begin(115200);
  // Connect to wifi
  while (!socketPong.connectWiFi(ssid, password)) {
    Serial.println("WiFi connection failed, retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");

  // Setup Callbacks
  socketPong.onMsg();
  socketPong.onEvnt();

  // Connect to server
  socketPong.connectServer();
  player = socketPong.playerCheck();
  Serial.println(player);
  if(player == "screen"){
    socketPong.screenInit();
  }
}

void loop() {
  socketPong.update();
  socketPong.spill(player);
}
