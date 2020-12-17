#include "Arduino.h"
#include "Arduino-idIOT.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>

idIOT::idIOT(String server) {
  websocketServer_ = server;
}

bool idIOT::connectWiFi(const char* ssid, const char* password, int pause) {
  WiFi.begin(ssid, password);  //nettverket og passordet 
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) { //Søker kontakt med nettverket
    delay(pause);                                                 //med 1 sekund pause  
  }
  if (WiFi.status() == WL_CONNECTED) { //Når det er kontakt med nettverket true
    return true;
  }
  else { //Er ikke kontakt med nettverket false
    return false;
  }
}

void idIOT::onMessageCallback(WebsocketsMessage message) {
  String msg = message.data(); //byte blir til String
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

void idIOT::sendSerial() {// Her skriver man ord som blir sendt fra klienten
  String serialData;     //skriver på serie monitoren. 
  serialData = Serial.readString();
  client.send(serialData);
}

void idIOT::update() {    //I denne koden vil se om det er mer data som skal leses.
  client.poll();          
  if (!client.available()) {  //Hvis det ikke er mer data
    Serial.println("Disconnected from server retrying.");
    client.connect(websocketServer_);
  }
}