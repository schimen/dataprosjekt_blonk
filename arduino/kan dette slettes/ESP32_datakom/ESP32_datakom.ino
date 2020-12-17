#include <analogWrite.h> //Import the analogWrite library for ESP32 so that analogWrite works properly
#include <WiFi.h>//Imports the needed WiFi libraries
#include <WiFiMulti.h> //We need a second one for the ESP32 (these are included when you have the ESP32 libraries)
#include <SocketIoClient.h> //Import the Socket.io library, this also imports all the websockets

WiFiMulti WiFiMulti; //Declare an instane of the WiFiMulti library
SocketIoClient webSocket; //Decalre an instance of the Socket.io library

void event(const char * payload, size_t length) { //Default event, what happens when you connect
  Serial.printf("got message: %s\n", payload);
}

void socketConnected(const char * payload, size_t length) {
  Serial.println("Connected!");
}

void messageHandle(const char * message, size_t length) {
  Serial.printf("Message: %s\n", message);
}


void setup() {
  Serial.begin(9600); //Start the serial monitor
  Serial.setDebugOutput(true); //Set debug to true (during ESP32 booting)

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 2; t > 0; t--) { //More debugging
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP("LAN Solo", "Steroidegutta6"); //Add a WiFi hotspot (addAP = add AccessPoint) (put your own network name and password in here)

  while (WiFiMulti.run() != WL_CONNECTED) { //Here we wait for a successfull WiFi connection untill we do anything else
    Serial.println("Not connected to wifi...");
    delay(100);
  }
  Serial.println("Connected to WiFi successfully!"); //When we have connected to a WiFi hotspot

  webSocket.on("handleConnected", socketConnected);
  webSocket.on("handleMessage", messageHandle);
  webSocket.begin("192.168.10.104", 2520); //This starts the connection to the server with the ip-address/domainname and a port (unencrypted)

}


void serialChecker() {
  if (Serial.available() > 0) {
    String tempSerialString = "\"" + Serial.readString() + "\"";
    const char* testEn = tempSerialString.c_str();
    webSocket.emit("handleMessage", testEn);
  }
}

void loop() {
  webSocket.loop();
  serialChecker();
  //DO NOT USE DELAY HERE, IT WILL INTERFER WITH WEBSOCKET OPERATIONS
  //TO MAKE TIMED EVENTS HERE USE THE millis() FUNCTION OR PUT TIMERS ON THE SERVER IN JAVASCRIPT
}
