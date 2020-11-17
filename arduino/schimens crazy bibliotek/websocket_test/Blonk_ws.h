#ifndef Blonk_ws_h
#define Blonk_ws_h

#include "Arduino.h"
#include <WiFi.h>
#include "WebSocketClient.h"

class Blonk
{
  public:
    Blonk(String input_token);
    bool connect_to_wifi(String ssid, String password);
    bool connect_to_server(String host, String path, int port);
    bool handshake();
    void send(String message);
    String read();
    bool is_connected_wifi();
    bool is_connected_server();

  private:
    String token;
    WiFiClient client;
    WebSocketClient webSocketClient;
};

#endif
