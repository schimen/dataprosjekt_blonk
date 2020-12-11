#include <SocketPong.h>

const char* ssid = "Get-2G-D61211"; //Enter SSID
const char* password = "YYEJ47GN8U"; //Enter Password

const int interval = 5000;
const int tempsensor = 34;
int temperatur;

SocketPong socketPong("ws://192.168.0.109:8000");

void sendTemp()
{
  String message = "send;temp;";
  float in_min = 0; float in_max = 4096; float out_min = -40; float out_max = 125;
  int x = analogRead(tempsensor);
  temperatur = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  message += String(temperatur);
  socketPong.send(message);
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);
    
  while (!socketPong.connectWiFi(ssid, password)) {
    Serial.println("WiFi connection failed, retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  
  socketPong.connectServer();
}

void loop()
{
  static int previousMillis = millis();
  socketPong.update();
  if (millis() - previousMillis > interval) {
    sendTemp();
    previousMillis = millis();
  }
}
