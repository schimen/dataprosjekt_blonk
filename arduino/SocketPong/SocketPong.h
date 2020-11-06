#ifndef SocketPong_h
#define SocketPong_h

#include "Arduino.h"
#include <WiFi.h>
#include <ArduinoWebsockets.h>
using namespace websockets;

class SocketPong {
  public:
    bool connect_to_wifi(String ssid_, String password_);
    bool connect_to_server(String server);
    void send(String message);
    void ping(String message);
    void poll();
    bool is_connected_wifi();
    bool is_connected_server();

  private:
    WebsocketsClient client;
};

#endif
