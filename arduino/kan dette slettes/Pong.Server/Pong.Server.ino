#include <WiFi.h>

char ssid[] = "Get-2G-D61A81";      
char password[] = "F7TDK4RTX3";   
int keyIndex = 0; 

#define potPin 32

WiFiServer server(8000);

void setup(){
  
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
   
  }
  Serial.println("");
  Serial.println("WIFI CONNECTED.");
  Serial.println("IP adress:");
 
  
}
void loop(){
  Serial.println(analogRead(potPin));
 
  delay(500);
}
