#ifndef SocketPong_h
#define SocketPong_h

#include "Arduino.h"
#include <ArduinoWebsockets.h>

class SocketPong {
  public:
    SocketPong(String server);
    bool connectWiFi(const char* ssid, const char* password);
    void onMsg();
    void onEvnt();
    void connectServer();
    void send(String sendData);
    void update();    
    String playerCheck();
    void screenInit();
    void spill(String ply);
  private:
    String websocketServer_;
};

#endif
