#ifndef idIOT_h
#define idIOT_h

#include "Arduino.h"
#include <ArduinoWebsockets.h>

using namespace websockets;

class idIOT {
  public:
    idIOT(String server);
    bool connectWiFi(const char* ssid, const char* password, int pause = 1000);
    static void messageHandler(String message);
    static void eventHandler(String event, String data);
    void onMsg();
    void onEvnt();
    void connectServer();
    void send(String sendData);
    void sendSerial();
    void update();

  private:
    static void onMessageCallback(WebsocketsMessage message);
    static void onEventsCallback(WebsocketsEvent event, String data);
    String websocketServer_;
    WebsocketsClient client;
};

#endif
