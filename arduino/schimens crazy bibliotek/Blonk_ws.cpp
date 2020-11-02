#include "Arduino.h"
#include "Blonk_ws.h"
#include "WebSocketClient.h"
#include <WiFi.h>

char host_[] = "demos.kaazing.com";
char path_[] = "/echo";

Blonk::Blonk(String input_token)
{
  token = token;
}

bool Blonk::connect_to_wifi(String ssid, String password)
{
  WiFi.begin("Eine", "Larsehagan14");
  int timeout_s = 30;
  while (WiFi.status() != WL_CONNECTED && timeout_s-- > 0) {
      delay(100);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Blonk::connect_to_server(String host, String path, int port)
{
  if (!client.connect(host_, 80))
  {
    return false;
  }
  webSocketClient.path = path_;
  webSocketClient.host = host_;
  return true;
}

bool Blonk::handshake()
{
  if (!webSocketClient.handshake(client))
  {
    return false;
  }
  else
  {
    return true;
  }
}

void Blonk::send(String message)
{
  webSocketClient.sendData("testing :)");
}

String Blonk::read()
{
  String data;
  data = webSocketClient.getData(data);
  if (data.length() > 0)  {
    return data;
  }
  else
  {
    return "";
  }
}

bool Blonk::is_connected_wifi()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Blonk::is_connected_server()
{
  if (client.connected())
  {
    return true;
  }
  else
  {
    return false;
  }
}
