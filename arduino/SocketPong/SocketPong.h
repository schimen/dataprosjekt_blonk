#ifndef SocketPong_h
#define SocketPong_h

#include "Arduino.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>

class SocketPong {
  public:
    SocketPong(String server);
    bool connectWiFi(const char* ssid, const char* password);
    void onMsg();
    void onEvnt();
    void connectServer();
    void send(String sendData);
    void sendAndUpdate();    
    int increaseCheck(int pin, int prevPot);
  private:
    String websocketServer_;
};

#endif
