#include "Arduino.h"
#include "SocketPong.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>

using namespace websockets;
WebsocketsClient client;

SocketPong::SocketPong(String server) {
  websocketServer_ = server;
}


bool SocketPong::connectWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
    delay(1000);
  }
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}

void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  Serial.println(message.data());
}

void SocketPong::onMsg() {
  client.onMessage(onMessageCallback);
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnected to Server");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Disconnected from Server");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}

void SocketPong::onEvnt() {
  client.onEvent(onEventsCallback);
}

void SocketPong::connectServer() {
  client.connect(websocketServer_);
}

void SocketPong::send(String sendData) {
  client.send(sendData);
}

void sendSerial() {
  String serialData;
  serialData = Serial.readString();
  client.send(serialData);
}

void SocketPong::sendAndUpdate() {
  client.poll();
  if (Serial.available() > 0) {
    sendSerial();
  }
  if(!client.available()){
    Serial.println("Disconnected from server retrying.");
    client.connect(websocketServer_);
  }
}

int SocketPong::increaseCheck(int pin, int prevPot){
  int currentPot = analogRead(pin);
  if(currentPot >= prevPot + 200 || currentPot <= prevPot - 200){
    client.send("send;potVerdi;" + String(currentPot));
    return currentPot;
  }
  else{
    return prevPot;
  }
}
