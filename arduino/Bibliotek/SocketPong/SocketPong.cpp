#include "Arduino.h"
#include "SocketPong.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>

SocketPong::SocketPong(String server) {
  websocketServer_ = server;
}

bool SocketPong::connectWiFi(const char* ssid, const char* password, int pause) {
  WiFi.begin(ssid, password);
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
    delay(pause);
  }
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}

void SocketPong::onMessageCallback(WebsocketsMessage message) {
  String msg = message.data();
  messageHandler(msg);
}

void SocketPong::onMsg() {
  client.onMessage(onMessageCallback);
}

void SocketPong::onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
      eventHandler("ConnectionOpened", data);
  }
  else if (event == WebsocketsEvent::ConnectionClosed) {
      eventHandler("ConnectionClosed", data);
  }
  else if (event == WebsocketsEvent::GotPong)  {
      eventHandler("GotPong", data);
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

void SocketPong::sendSerial() {
  String serialData;
  serialData = Serial.readString();
  client.send(serialData);
}

void SocketPong::update() {
  client.poll();
  if (Serial.available() > 0) {
    sendSerial();
  }
  if (!client.available()) {
    Serial.println("Disconnected from server retrying.");
    client.connect(websocketServer_);
  }
}
