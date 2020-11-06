#include "SocketPong.h"

const char* ssid = "LAN Solo";
const char* password = "Steroidegutta6";
const char* websockets_server = "ws://echo.websocket.org:80";

void onEventCall(WebsocketsEvent event, String data) {
   if(event == WebsocketsEvent::ConnectionOpened) {
      Serial.println("Connnection Opened");
  } else if(event == WebsocketsEvent::ConnectionClosed) {
      Serial.println("Connnection Closed");
  } else if(event == WebsocketsEvent::GotPing) {
      Serial.println("Got a Ping!");
  } else if(event == WebsocketsEvent::GotPong) {
      Serial.println("Got a Pong!");
  }
}

void onMsgCallback(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  Serial.println(message.data());
}

void setup() {
  Serial.begin(9600);
  while (!SocketPong.connect_to_wifi(ssid, password)) {
    Serial.println("Couldn't connect to wifi, retrying...");
    delay(1000);
  }
  Serial.println("Connected to wifi");


  // Setup Callbacks
  client.onMessage(onMsgCallback);
  client.onEvent(onEventCall);

  // Connect to server
  if (!SocketPong.connect_to_server(websockets_server)) {
    Serial.println("Couldn't connect to server");
  }
  else {
    Serial.println("Connected to server");
  }

  //Send message
  SocketPong.send("Testing 1");
  SocketPong.ping();
}

void loop() {
  SocketPong.poll();
}
