#include "Arduino.h"
#include "SocketPong.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>

bool SocketPong::connect_to_wifi(String ssid_, String password_)
{
  WiFi.begin(ssid_, password_);
  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
     Serial.print(".");
     delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}

bool SocketPong::connect_to_server(String server) {
  if (!client.connect(server)) {
    return false;
  }
  client.connect(server)
  return true;
}

/*
void SocketPong::onEvent(){
   client.onEvent(eventCall);
}
*/

void SocketPong::send(String message) {
  client.send(message);
}

void SocketPong::ping(String message) {
  client.ping(message);
}

/*
void SocketPong::onMsg() {
  client.onMessage(onMsgCallback);
}
*/

void SocketPong::poll() {
  client.poll();
}



bool SocketPong::is_connected_wifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}

bool SocketPong::is_connected_server() {
  if (client.connected()) {
    return true;
  }
  else {
    return false;
  }
}
