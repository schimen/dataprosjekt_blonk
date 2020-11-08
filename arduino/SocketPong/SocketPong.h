#ifndef SocketPong_h
#define SocketPong_h

#include "Arduino.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>

class SocketPong {
  public:
    SocketPong(String test);
    bool connectWiFi(const char* ssid, const char* password);
    void onMsg();
    void onEvnt();
    void connectServer(const char* websocketServer);
    void send(String sendData);
    void sendAndUpdate();    
  private:
    String test_;
};

#endif
