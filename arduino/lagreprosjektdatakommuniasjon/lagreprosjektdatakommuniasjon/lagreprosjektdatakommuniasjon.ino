#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <WString.h>
#include <string.h>
#include <stdlib.h>
#include <WebSocketClient.h>
#include <tiny_websockets/client.hpp>
#include <tiny_websockets/server.hpp>
#include <iostream>

const char* ssid = "Get-2G-D61A81"; //Enter SSID
const char* password = "F7TDK4RTX3"; //Enter Password
const char* websockets_server = "ws://echo.websocket.org:80"; //server adress and port

using namespace websockets;

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

WSString line;
WebsocketsClient client;

String x;

void setup() {
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 1000 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Setup Callbacks
  
    client.onMessage(onMessageCallback);
    
    client.onEvent(onEventsCallback);
   
    // Connect to server
    /*
    client.connect(websockets_server);
    */
    if(client.connect(websockets_server)){
      Serial.println("Skriv adreesen til meldingen ");
      client.send("send;A1;Hei\n");      
    }else{
      Serial.println("no conneted");      
    }    
    // Send a message    
    // Send a ping
    client.ping();
    
   
}

void loop() {

  
  volatile bool done = false;
  auto msg = client.readBlocking();
  if(msg.isText()) {
    Serial.print("Got Text Message: ");
    Serial.print(msg.data());
  }
  
  else if(msg.isPing()) {
    Serial.print("Got Ping with payload: ");
    Serial.print(msg.data());
  }
  
  client.onEvent([&done](WebsocketsClient& client, WebsocketsEvent event, String data) {
      if(event == WebsocketsEvent::GotPong) {
          x = data;
          done = true;
          Serial.println("lytt;B3;start\n");
          Serial.println(data);
      }
  });
  
      
  while(client.available()){
    client.poll();
  }
    
    
      
   
  
}


  /*
  
  WebsocketsMessage(logMessage);
}
  
void logMessage(websockets::WebsocketsMessage& msg) {
  
  if(msg.isComplete()) {
    Serial.print("Message is Complete.");
  } else {
    Serial.print("Message is Partial, It is at the: ");
    if(msg.isFirst()) Serial.print( "Begining");
    else if(msg.isContinuation()) Serial.print("Middle");
    else if(msg.isLast()) Serial.print( "End");
  }
} 

  /*
  enum FragmentsPolicy{
    FagmentsPolicy_Aggregate, FragmentsPlicy_Notify
  
  };
  void setFragmentsPolicy(FragmentsPolicy newPolicy);
  FragmentsPolicy getFragmentsPolicy();
    
}
  
  
  /*
  String character;
  if(client.available() > 0  && (character = client.read()) == 0){
    String data = "";
    bool endReached = false;
      while(!endReached){
        character = client.read();
        endReached = character == -1;
        if(!endReached){
          data += character;
          return Serial.print(data);
        }
      }
  }
  
  
     
    client.poll();
  }
  delay(500);

}
  /*
  //Når der en kontakt skal klienten sende melding til serveren med adresse og melding.
  if(client1){
    Serial.print("Skriv adresse og melding");
    if(Serial.available()>0){
      Serial.println("Sent...");
      serial_data = String(Serial.read());
      client.send(serial_data);
    }
  }

  Serial.println("starting WebSocket client: ");
  client.readString(data);
    
    
  }
    
  client.poll();
  


  

  if(client.available()){
    client1;
    }
      Serial.print(client.readString());    
    }
  }
  
      
    


    /*
    client1 = String(client.read());
    Serial.println("Contents:");
    Serial.println(client1);
    
    
    

/*
  while(Serial.available()>0){
    
  }


  /*
  if(Serial.available()>0){
    Serial.println("Sent...");
    serial_data = String(Serial.read());
    client.send(serial_data);
    
  }
  if(client.connect(websockets_server)){
    while(Serial.available()>0){
        char inChar = Serial.read();       
        Serial.print(inChar);
        
    }
  }
    
  */
  
    
    
    

    

/*“send;adresse;melding\n”
Lagrer meldingen under adressen, og sender den videre til alle andre klienter som lytter på adressen.


Eksempel: > “send;A1;Hei\n”
Serveren lagrer “Hei” til adressen A1 og sender meldingen til alle klienter som lytter på A1.
Lytt
“lytt;adresse;instruks\n”
Starter eller stopper lytting på den spesifiserte adressen. Dvs. at klienten automatisk får melding når adressen oppdateres. Innholdsfeltet i denne kommandoen består av enten «start» eller «stopp», ut i fra hvorvidt man ønsker å starte eller avslutte lyttingen.
Eksempel: > «lytt;B3;start\n»
Klienten blir lagret som lyttende på B3 og vil motta alle oppdaterte verdier på denne adressen.
Hent
«hent;adresse\n»
Henter den siste siste verdien lagret under adressen og sender den til klienten.
Eksempel: > «hent;A2\n»
     < «melding;A2;Hello\n»*/
   

    


    
