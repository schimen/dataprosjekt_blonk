#include <Arduino.h>
#include <Arduino-idIOT.h>
#include <TFT_eSPI.h>
#include <SPI.h>

const char* ssid = "PongGang"; //Enter SSID
const char* password = "Pong1234"; //Enter Password
String player;

int buttonPin = 22; //Button pin
int potPin = 34; //Potmeter pin

//Client value
int value1 = 0;
int value2 = 0;

//Potmeter value
int potValue_ = 0;

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

//ST7789 dimensions
const int16_t h = 240;
const int16_t w = 240;


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
int16_t ball_x = w/2;
int16_t ball_y = h/2;
int16_t oldball_x = w/2;
int16_t oldball_y = h/2;

//ball directions in (x,y)
int16_t ball_dx = 1;
int16_t ball_dy = 1;

//ball placements
int16_t ball_w = 6;
int16_t ball_h = 6;

const int ball_step = 1;
int16_t max_ball_speed = 15;
int16_t ball_speed = max_ball_speed;

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

//Timing variable
unsigned long previousMillis = 0;

//Function to extract integer from server message
void idIOT::messageHandler(String message) {
  if (message.substring(0, 8) == "melding;") {
    if (message.substring(8, 19) == "player1Pot;") {
      value1 = message.substring(19).toInt();
    }
    else if (message.substring(8, 19) == "player2Pot;") {
      value2 = message.substring(19).toInt();
      Serial.println(value2);
    }
  }
}

void idIOT::eventHandler(String event, String data){};


idIOT connection("ws://192.168.137.95:8000"); //Enter Server address
TFT_eSPI tft = TFT_eSPI(); //Screen library

void score() {
  //stops ball from spamming score if out of bounds
  if (ball_x > w && scoreswitch) {
    lscore++;
    scoreswitch = false;
    initGame();
  }
  if (ball_x < 0 && scoreswitch) {
    rscore++;
    scoreswitch = false;
    initGame();
  }

  //MAKE scoreswitch TRUE ON BUTTON PRESS TO RESET GAME, TO ENABLE POINT GAIN ON NEW ROUND
  //Serial.println(ball_x); debug

  //update score to string and display
  lscoretext = String(lscore);
  rscoretext = String(rscore);
  tft.drawString (lscoretext, 30, 20, 2);
  tft.drawString (rscoretext, w - 50, 20, 2);
}

int new_ball_speed()  {
  static int min_speed = 1;
  static int speed_change = 2;
  if (ball_speed - speed_change >= min_speed) {
    return ball_speed - speed_change;
  }
  else if (ball_speed > min_speed)  {
    return min_speed;
  }
  else  {
    return ball_speed;
  }
}

void ball() {

  //ball_dx is the direction and change for the ball each frame, same with ball_dy
  ball_x = ball_x + ball_dx;
  ball_y = ball_y + ball_dy;

  //changes direction of ball on paddle hit
  if (ball_dx == -1 && ball_x == paddle_w && ball_y + ball_h >= lpaddle_y && ball_y <= lpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
    ball_dy = random_direction_y();
    ball_speed = new_ball_speed();
  }

  else if (ball_dx == 1 && ball_x + ball_w == w - paddle_w && ball_y + ball_h >= rpaddle_y && ball_y <= rpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
    ball_dy = random_direction_y();
    ball_speed = new_ball_speed();
  }

  // Keep ball in bounds
  if (ball_y > h - ball_w || ball_y < 0) {
    ball_dy = ball_dy * -1;
    ball_y += ball_dy;
  }

  //oldball and ball functions aliases ball position for less jitter
  if (ball_dy > 1 || ball_dy < -1)  {
    tft.fillRect(oldball_x, oldball_y, ball_w, ball_h, BLACK); 
  }
  else  {
    tft.drawRect(oldball_x, oldball_y, ball_w, ball_h, BLACK); // Less TFT refresh aliasing than line above for large balls
  }
  tft.fillRect(   ball_x,    ball_y, ball_w, ball_h, WHITE);
  oldball_x = ball_x;
  oldball_y = ball_y;
  previousMillis = millis();
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
  //player controls for lpaddle
  lpaddle_y = map(stringPotValue1, 0 , 4095 , 1 , 240 - paddle_h);
  static int last_y_value = lpaddle_y;
  if (lpaddle_y != last_y_value)  {
    //remove remove previous paddle draw
    tft.fillRect(lpaddle_x, last_y_value, paddle_w, paddle_h, BLACK);
    last_y_value = lpaddle_y;
  
    //draws lpaddle
    tft.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
  }
}

void rpaddle(int stringPotValue2) {
  //player controls for rpaddle
  rpaddle_y = map(stringPotValue2, 0 , 4095 , 1 , 240 - paddle_h);
  static int last_y_value = rpaddle_y;
  if (rpaddle_y != last_y_value)  {
    //remove remove previous paddle draw
    tft.fillRect(rpaddle_x, last_y_value, paddle_w, paddle_h, BLACK);
    last_y_value = rpaddle_y;
  
    //draws lpaddle
    tft.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);
  }
}


//Function to randomize ball direction in x
int random_direction_x() {
  int random_direction_variable = random(-1, 1);
  if (random_direction_variable >= 0)  {
    random_direction_variable = 1;
  }
  else  {
    random_direction_variable = -1;
  }
  return random_direction_variable;
}

//Function to randomize ball direction in y
int random_direction_y() {
  return random(-2, 2);
}

void initGame() {
  
  //sets paddle to potmeter value position on y plane
  lpaddle_y = map(value1, 0, 4095, 0 , 240 - paddle_h);
  rpaddle_y = map(value2, 0, 4095, 0 , 240 - paddle_h);

  // ball is placed on the center of the left paddle (switch to right player after a point if you care)
  ball_y = h / 2 ;
  ball_x = w / 2;
  ball_dx = random_direction_x();
  ball_dy = random_direction_y();
  ball_speed = max_ball_speed;

  midline_score();
  scoreswitch = true;
  screenReset();
}

bool increaseCheck() {
  int currentPot = analogRead(potPin);
  if (currentPot >= potValue_ + 50 || currentPot <= potValue_ - 50) {
    potValue_ = currentPot;
    return true;
  }
  else {
    return false;
  }
}

//Function to check what player (screen, potmeter1 or potmeter2) is on the ESP32
String playerCheck() {
  static int potValue;
  potValue = analogRead(potPin); 
  pinMode(buttonPin, INPUT);
  delay(500);
  while (true) {
    potValue = analogRead(potPin); //If pot value equals 0 and if button pressed assign screen
    if (potValue == 0) {
      if (!digitalRead(buttonPin)) {  //hvis det er skjermen som starter
        connection.send("lytt;player1Pot;START");
        connection.send("lytt;player2Pot;START");
        return "screen";
      }
    }
    else if (potValue <= 1000 && potValue != 0) { //Assign player1 if requirement is met
      previousMillis = millis();
      while (potValue <= 1000 && potValue != 0) {
        potValue = analogRead(potPin);
        if (millis() - previousMillis >= 5000) {
          connection.send("lagre;player1Pot;STOP");
          return "player1Pot";
        }
      }
    }
    else if (potValue >= 3900) {  //Assign player2 if requirement is met
      previousMillis = millis();
      while (potValue >= 3900) {
        potValue = analogRead(potPin);
        if (millis() - previousMillis >= 5000) {
          connection.send("lagre;player2Pot;STOP");
          return "player2Pot";
        }
      }
    }
  }
}

//Function for the game
void spillPong(String ply) {
  if (ply == "player1Pot" || ply == "player2Pot") { //If potmeter is player
    if (increaseCheck()) {
      String msgValue = "send;";
      msgValue +=  ply;
      msgValue +=  ";";
      msgValue += String(potValue_);
      connection.send(msgValue);
    }
  }
  else if (ply == "screen") {
    if (!digitalRead(buttonPin)) {  //If screen is player
        lscore = 0;
        rscore = 0;
        initGame();
      }
    lpaddle(value1);
    rpaddle(value2);
    midline_score();
    score();
    if (millis() - previousMillis >= ball_speed) {
      ball();
    }
  }
}

void screenInit() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  initGame();
  tft.setTextSize (2);
  tft.setTextColor(WHITE, BLACK);

  //draw court:
  tft.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
  tft.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);
}

void screenReset()  {
  tft.fillScreen(BLACK);
  tft.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
  tft.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);
}

void setup() {
  Serial.begin(115200);
  // Connect to wifi
  while (!connection.connectWiFi(ssid, password)) {
    Serial.println("WiFi connection failed, retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  // Setup Callbacks
  connection.onMsg();
  connection.onEvnt();

  // Connect to server
  connection.connectServer();
  player = playerCheck();
  Serial.println(player);
  if(player == "screen"){
    screenInit();
  }
}

void loop() {
  connection.update();
  spillPong(player);
}
