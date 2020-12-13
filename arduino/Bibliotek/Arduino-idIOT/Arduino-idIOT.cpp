#include "Arduino.h"
#include "Arduino-idIOT.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>

idIOT::idIOT(String server) {
  websocketServer_ = server;
}

bool idIOT::connectWiFi(const char* ssid, const char* password, int pause) {
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

void idIOT::onMessageCallback(WebsocketsMessage message) {
  String msg = message.data();
  messageHandler(msg);
}

void idIOT::onMsg() {
  client.onMessage(onMessageCallback);
}

void idIOT::onEventsCallback(WebsocketsEvent event, String data) {
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

void idIOT::onEvnt() {
  client.onEvent(onEventsCallback);
}

void idIOT::connectServer() {
  client.connect(websocketServer_);
}

void idIOT::send(String sendData) {
  client.send(sendData);
}

void idIOT::sendSerial() {
  String serialData;
  serialData = Serial.readString();
  client.send(serialData);
}

void idIOT::update() {
  client.poll();
  if (!client.available()) {
    Serial.println("Disconnected from server retrying.");
    client.connect(websocketServer_);
  }
}
