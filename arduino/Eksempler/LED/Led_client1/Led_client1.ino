
#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = "Get-2G-351461"; //Enter SSID
const char* password = "NB8UJY7RAE"; //Enter Password
const char* websockets_server_host = "192.168.0.142"; //Enter server adress
const uint16_t websockets_server_port = 8000; // Enter server port

String readIn = "";
boolean connection = false;

const int ledpin = 2;

String sourceMessageOn = "melding;SindreLedStatus;On\n";
String sourceMessageOff = "melding;SindreLedStatus;Off\n";

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
    
    pinMode(ledpin, OUTPUT);
  
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
      readLedStatus(message.data());
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
      client.send("hent;SindreLedStatus");
    }
    if(client.available()) 
    {
        client.poll();
    }
    delay(500);
}


void readLedStatus(String ledMessage)
{  
  if (ledMessage == sourceMessageOn)
  {
    digitalWrite(ledpin, HIGH);
    Serial.println("Led turned on.");
  }
  else if (ledMessage == sourceMessageOff)
  {
    digitalWrite(ledpin, LOW);
    Serial.println("Led turned off.");
  }
  else
  {
    Serial.println("Message was not interpreted.");
  }
  Serial.println();
  return;
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
