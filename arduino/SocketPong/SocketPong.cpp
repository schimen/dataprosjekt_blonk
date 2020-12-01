#include "Arduino.h"
#include "SocketPong.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB
#define potPin 34
#define buttonPin 22

using namespace websockets;
WebsocketsClient client;
TFT_eSPI tft = TFT_eSPI();

//ST7789 dimensions
int16_t h = 240;
int16_t w = 240;

int potValue_ = 0;

//Potmeter 1-2 values
int value1 = 0;
int value2 = 0;

//paddle dimensions and placements
int16_t paddle_h = 40;
int16_t paddle_w = 5;

int16_t lpaddle_x = 0;
int16_t rpaddle_x = w - paddle_w;

int16_t lpaddle_y = 0;
int16_t rpaddle_y = h - paddle_h;

//paddle directions for removing previous frame draws
int16_t lpaddle_d = 1;
int16_t rpaddle_d = -1;

int16_t lpaddle_ball_t = w - w / 4;
int16_t rpaddle_ball_t = w / 4;

//ball placement on grid
int16_t ball_x = 2;
int16_t ball_y = 2;
int16_t oldball_x = 2;
int16_t oldball_y = 2;

//ball directions in (x,y)
int16_t ball_dx = 1;
int16_t ball_dy = 1;

//ball placements
int16_t ball_w = 6;
int16_t ball_h = 6;

//midline
int16_t dashline_h = 4;
int16_t dashline_w = 2;
int16_t dashline_n = h / dashline_h;
int16_t dashline_x = w / 2 - 1;
int16_t dashline_y = dashline_h / 2;

//score counters and score logic
int16_t lscore = 0;
int16_t rscore = 0;

String lscoretext;
String rscoretext;

bool scoreswitch = true;

unsigned long previousMillis = 0;


SocketPong::SocketPong(String server) {
  websocketServer_ = server;
}


bool SocketPong::connectWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
    delay(1000);
  }
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}

void onMessageCallback(WebsocketsMessage message) {
  String msg = message.data();
  Serial.println(msg);
  if (msg.substring(0, 8) == "melding;") {
    if (msg.substring(8, 19) == "player1Pot;") {
      value1 = msg.substring(19).toInt();
      Serial.println(value1);
    }
    else if (msg.substring(8, 19) == "player2Pot;") {
      value2 = msg.substring(19).toInt();
      Serial.println(value2);
    }
  }

}

void SocketPong::onMsg() {
  client.onMessage(onMessageCallback);
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnected to Server");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Disconnected from Server");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}

void SocketPong::onEvnt() {
  client.onEvent(onEventsCallback);
}

void SocketPong::connectServer() {
  client.connect(websocketServer_);
}

void SocketPong::send(String sendData) {
  client.send(sendData);
}

void sendSerial() {
  String serialData;
  serialData = Serial.readString();
  client.send(serialData);
}

void SocketPong::update() {
  client.poll();
  if (Serial.available() > 0) {
    sendSerial();
  }
  if (!client.available()) {
    Serial.println("Disconnected from server retrying.");
    client.connect(websocketServer_);
  }
}


void score() {
  //stops ball from spamming score if out of bounds
  if (ball_x > 240 && scoreswitch) {
    lscore++;
    scoreswitch = false;
  }
  if (ball_x < 0 && scoreswitch) {
    rscore++;
    scoreswitch = false;
  }

  //MAKE scoreswitch TRUE ON BUTTON PRESS TO RESET GAME, TO ENABLE POINT GAIN ON NEW ROUND
  //Serial.println(ball_x); debug

  //update score to string and display
  lscoretext = String(lscore);
  rscoretext = String(rscore);
  tft.drawString (lscoretext, 30, 20, 2);
  tft.drawString (rscoretext, w - 50, 20, 2);
}

void ball() {

  //ball_dx is the direction and change for the ball each frame, same with ball_dy
  ball_x = ball_x + ball_dx;
  ball_y = ball_y + ball_dy;

  //changes direction of ball on paddle hit
  if (ball_dx == -1 && ball_x == paddle_w && ball_y + ball_h >= lpaddle_y && ball_y <= lpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
  }

  else if (ball_dx == 1 && ball_x + ball_w == w - paddle_w && ball_y + ball_h >= rpaddle_y && ball_y <= rpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
  }

  // Keep ball in bounds
  if (ball_y > h - ball_w || ball_y < 0) {
    ball_dy = ball_dy * -1;
    ball_y += ball_dy;
  }

  //oldball and ball functions aliases ball position for less jitter
  //tft.fillRect(oldball_x, oldball_y, ball_w, ball_h, BLACK);
  tft.drawRect(oldball_x, oldball_y, ball_w, ball_h, BLACK); // Less TFT refresh aliasing than line above for large balls
  tft.fillRect(   ball_x,    ball_y, ball_w, ball_h, WHITE);
  oldball_x = ball_x;
  oldball_y = ball_y;
}

void midline_score() {

  // If the ball is not on the line then don't redraw the line
  if ((ball_x < dashline_x - ball_w) && (ball_x > dashline_x + dashline_w)) return;

  tft.startWrite();

  // Quick way to draw a dashed line
  tft.setAddrWindow(dashline_x, 0, dashline_w, h);

  for (int16_t i = 0; i < dashline_n; i += 2) {
    tft.pushColor(WHITE, dashline_w * dashline_h); // push dash pixels
    tft.pushColor(BLACK, dashline_w * dashline_h); // push gap pixels
  }

  tft.endWrite();
}

void lpaddle(int stringPotValue1) {
  //remove remove previous paddle draw. This method only removes the white pixel delta, does not redraw entire paddle
  tft.fillRect(lpaddle_x - 1, lpaddle_y, paddle_w + 1, paddle_w, BLACK);
  tft.fillRect(lpaddle_x, lpaddle_y + paddle_h - 1, paddle_w, paddle_w, BLACK);

  //player controls for lpaddle
  lpaddle_y = map(stringPotValue1, 0 , 4095 , 1 , 240 - paddle_h);

  //draws lpaddle
  tft.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
}

void rpaddle(int stringPotValue2) {
  //See lpaddle code
  tft.fillRect(rpaddle_x - 1, rpaddle_y, paddle_w + 1, paddle_w, BLACK);
  tft.fillRect(rpaddle_x, rpaddle_y + paddle_h - 1, paddle_w, paddle_w, BLACK);

  //player controls for rpaddle
  rpaddle_y = map(stringPotValue2, 0 , 4095 , 1 , 240 - paddle_h);

  tft.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);

}

void initgame() {
  //sets paddle to potmeter value position on y plane
  lpaddle_y = map(value1, 0, 4095, 0 , 240 - paddle_h);
  rpaddle_y = map(value2, 0, 4095, 0 , 240 - paddle_h);

  // ball is placed on the center of the left paddle (switch to right player after a point if you care)
  ball_y = lpaddle_y + (paddle_h / 2);

  midline_score();
}

bool increaseCheck() {
  int currentPot = analogRead(potPin);
  if (currentPot >= potValue_ + 200 || currentPot <= potValue_ - 200) {
    potValue_ = currentPot;
    return true;
  }
  else {
    return false;
  }
}

String SocketPong::playerCheck() {
  int potValue = 0;
  potValue = analogRead(potPin);
  pinMode(buttonPin, INPUT);
  delay(500);
  while (true) {
    potValue = analogRead(potPin);
    if (potValue == 0) {
      if (!digitalRead(buttonPin)) {
        client.send("lytt;player1Pot;START");
        client.send("lytt;player2Pot;START");
        return "screen";
      }
    }
    else if (potValue <= 1000 && potValue != 0) {
      previousMillis = millis();
      while (potValue <= 1000 && potValue != 0) {
        potValue = analogRead(potPin);
        if (millis() - previousMillis >= 5000) {
          return "player1Pot";
        }
      }
    }
    else if (potValue >= 3900) {
      previousMillis = millis();
      while (potValue >= 3900) {
        potValue = analogRead(potPin);
        if (millis() - previousMillis >= 5000) {
          return "player2Pot";
        }
      }
    }
  }
}

void SocketPong::spill(String ply) {
  if (ply == "player1Pot" || ply == "player2Pot") {
    if (increaseCheck()) {
      String msgValue = "send;";
      msgValue +=  ply;
      msgValue +=  ";";
      msgValue += String(potValue_);
      Serial.println(msgValue);
      client.send(msgValue);
    }
  }
  else if (ply == "screen") {
    lpaddle(value1);
    rpaddle(value2);
    midline_score();
    score();
    if (millis() - previousMillis >= 50) {
      ball();
    }
  }
}

void SocketPong::screenInit() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  initgame();
  tft.setTextSize (2);
  tft.setTextColor(WHITE, BLACK);
}
