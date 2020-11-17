#include "Blonk_ws.h"

#define SSID "Yoschimin"
#define PASSWORD "simensimen123"
#define HOST "wss://echo.websocket.org"
#define PORT 80
#define PATH ""

String response;

Blonk blonk("abc");

void setup()
{
  Serial.begin(115200);
  while (!blonk.connect_to_wifi(SSID, PASSWORD))
  {
    Serial.println("could not connect, to wifi, trying again..");
    delay(1000);
  }
  Serial.println("connected to wifi");
  delay(1000);
  if (!blonk.connect_to_server(HOST, PATH, PORT))
  {
    Serial.println("could not connect to server");
  }
  else
  {
    Serial.println("connected to server");
  }
  delay(1000);
  if (!blonk.handshake())
  {
    Serial.println("could not perform handshake");
  }
  else
  {
    Serial.print("performed handshake");
  }
  delay(1000);
}

void loop()
{
  if (blonk.is_connected_server())
  {
    blonk.send("test");
    //response = "should be working";
    response = blonk.read();
    if (response.length() > 0)
    {
      Serial.println(response);
    }
  }
  delay(2000);
}
