
#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = "Sindre sin iPhone"; //Enter SSID
const char* password = "sindrespassord"; //Enter Password
const char* websockets_server_host = "172.20.10.8"; //Enter server adress
const uint16_t websockets_server_port = 8000; // Enter server port

String readIn = "";
boolean connection = false;

const int tempsensor = 34;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

String message = "";

using namespace websockets;

void onEventsCallback(WebsocketsEvent event, String data) 
  {
    if(event == WebsocketsEvent::ConnectionOpened) 
      {
        Serial.println("Connnection Opened");
      } 
    else if(event == WebsocketsEvent::ConnectionClosed) 
      {
        Serial.println("Connnection Closed");
        connection = false;
      }
  }

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    Serial.println();
    // Connect to wifi
    Serial.println("Connecting to WiFi");
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
    if(connected) 
    {
      Serial.println("Connected!");
      connection = true;
    } 
    else 
    {
      Serial.println("Not Connected!");
    }

    client.onEvent(onEventsCallback);
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message)
    {
        Serial.print("Got Message: ");
        Serial.println(message.data());
    });
}

void loop() 
{
    while(!connection)
    {
      lostconnection();
      delay(2000); 
    }
    if(connection)
    {
      temp();
    }
    if(client.available()) 
    {
        client.poll();
    }
    delay(500);
}

void temp()
{
  message = "send;Sindre;";
  Vo = analogRead(tempsensor);
  R2 = R1 * (4095.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15; 

  message += String(Tc);
  
  Serial.print("Sending temperature: ");
  Serial.print(Tc);
  Serial.println(" C");
  client.send(message);
  
}

void lostconnection()
{
  Serial.println("Attempting to reconnect");
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
  if(connected) 
    {
      Serial.println("Connected!");
      connection = true;
    }
  else if (!connected)
  {
    Serial.println("Failed");
  }
}
