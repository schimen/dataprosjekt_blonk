#include <Arduino-idIOT.h>

const char* ssid = "PongGang"; //Enter SSID
const char* password = "Pong1234"; //Enter Password

const int interval = 1000;
const int tempsensor = 34;
int temperatur;

idIOT connection("ws://192.168.137.95:8000");

void sendTemp()
{
  String message = "send;livetemp;";
  float in_min = 0; float in_max = 4096; float out_min = 0; float out_max = 3.3;
  int x = analogRead(tempsensor); 
  //For max spenning er 3.3 volt må vi dele (1024*4) = 4096.   
  float voltage = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  //Sensoren leser av temperaturen under 0 grader
  //derfor legge til 10. I tillegg trekke fra 0.5 til spenningen og multiplisere med 100, for å få 
  //riktig temperatur i celsius. 
  temperatur = (voltage - 0.5) * 100.0 + 10; 
  message += String(temperatur);                 
  connection.send(message);                  
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);
    
  while (!connection.connectWiFi(ssid, password)) {
    Serial.println("WiFi connection failed, retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  
  connection.connectServer();
}

void loop()
{
  static int previousMillis = millis();
  connection.update();
  if (millis() - previousMillis > interval) {
    sendTemp();
    //Leser av temperaturen hver sekund.
    previousMillis = millis();
  }
}