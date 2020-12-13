#include <SocketPong.h>

const char* ssid = "PongGang"; //Enter SSID
const char* password = "Pong1234"; //Enter Password

const int ledpin = 2;
bool ledOn = false;

String sourceMessageOn = "melding;led;on\n";
String sourceMessageOff = "melding;led;off\n";

void SocketPong::messageHandler(String message) {
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

void SocketPong::eventHandler(String event, String data)  {
  Serial.println(event);
}

SocketPong socketPong("ws://192.168.137.95:8000");

void setup() {
  pinMode(ledpin, OUTPUT);
  Serial.begin(115200);
  
  // Connect to wifi
  while (!socketPong.connectWiFi(ssid, password)) {
    Serial.println("WiFi connection failed, retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");

  // Setup Callbacks
  socketPong.onMsg();
  socketPong.onEvnt();

  // Connect to server
  socketPong.connectServer();

  socketPong.send("hent;led\n");
  socketPong.send("lytt;led;START\n");
}

void loop() {
    socketPong.update();
}
