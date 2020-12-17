#include <Arduino-idIOT.h>

const char* ssid = "BjornarNett"; //Enter SSID
const char* password = "Hello123"; //Enter Password

const int ledpin = 2;
bool ledOn = false;

String sourceMessageOn = "melding;led;on\n";
String sourceMessageOff = "melding;led;off\n";

void idIOT::messageHandler(String message) {
  if (message == sourceMessageOn && !ledOn)
  {
    digitalWrite(ledpin, HIGH);
    Serial.println("Led turned on.");
    ledOn = true;
  }
  else if (message == sourceMessageOff && ledOn)
  {
    digitalWrite(ledpin, LOW);
    Serial.println("Led turned off.");
    ledOn = false;
  }
}

void idIOT::eventHandler(String event, String data)  {
  Serial.println(event);
}

idIOT connection("ws://192.168.137.95:8000");

void setup() {
  pinMode(ledpin, OUTPUT);
  Serial.begin(115200);
  
  // Connect to wifi
  while (!connection.connectWiFi(ssid, password)) {
    Serial.println("WiFi connection failed, retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");

  // Setup Callbacks
  connection.onMsg();
  connection.onEvnt();

  // Connect to server
  connection.connectServer();

  connection.send("hent;led\n");
  connection.send("lytt;led;START\n");
}

void loop() {
    connection.update();
}
