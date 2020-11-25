//Possible program expansions
//1. reset button (won't take long, call initgame() on button press??
//2. win condition at 10 points? (store score in flash memory? would work well with point 1)

//------Define pins to your own config!-----

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB
#define potpin1 35
#define potpin2 25

//libraries
#include <TFT_eSPI.h> 
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();

//ST7789 dimensions
int16_t h = 240;
int16_t w = 240;

//Potmeter 1-2 values
int value1 = 0;
int value2 = 0;

//Program refresh rate, change to 37.37 for perferct 27Hz refresh rate of screen
int dly = 5;

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


void setup(void) {

  Serial.begin(9600);
  tft.init();
  //landscape or vertical mode, doesn't really matter with a square screen
  tft.setRotation(1);

  tft.fillScreen(BLACK);

  //Can call initgame to restart game after point (TODO??)
  initgame();

  tft.setTextSize (3);
  tft.setTextColor(WHITE, BLACK);
  
}

void loop() {
  
  delay(dly);
  lpaddle();
  rpaddle();
  midline_score();
  score();
  ball();
  
  
}

void initgame() {
  //sets paddle to potmeter value position on y plane
  lpaddle_y = map(analogRead(potpin1), 0, 4095, 0 , 240-paddle_h);
  rpaddle_y = map(analogRead(potpin2), 0, 4095, 0 , 240-paddle_h);

  // ball is placed on the center of the left paddle (switch to right player after a point if you care)
  ball_y = lpaddle_y + (paddle_h / 2);
  
  midline_score();
}

void midline_score() {

  // If the ball is not on the line then don't redraw the line
  if ((ball_x<dashline_x-ball_w) && (ball_x > dashline_x+dashline_w)) return;

  tft.startWrite();

  // Quick way to draw a dashed line
  tft.setAddrWindow(dashline_x, 0, dashline_w, h);
  
  for(int16_t i = 0; i < dashline_n; i+=2) {
    tft.pushColor(WHITE, dashline_w*dashline_h); // push dash pixels
    tft.pushColor(BLACK, dashline_w*dashline_h); // push gap pixels
  }

  tft.endWrite();
}

void lpaddle() {

  //remove remove previous paddle draw. This method only removes the white pixel delta, does not redraw entire paddle
  tft.fillRect(lpaddle_x-1, lpaddle_y, paddle_w+1, paddle_w, BLACK);
  tft.fillRect(lpaddle_x, lpaddle_y + paddle_h - 1, paddle_w, paddle_w, BLACK);

  //player controls for lpaddle
  value1 = analogRead(potpin1);
  value1 = map(value1, 0 , 4095 , 1 , 240-paddle_h);
  
  lpaddle_y = value1;
  //draws lpaddle
  tft.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
}

void rpaddle() {

  //See lpaddle code
  
  tft.fillRect(rpaddle_x-1, rpaddle_y, paddle_w+1, paddle_w, BLACK);
  tft.fillRect(rpaddle_x, rpaddle_y + paddle_h - 1, paddle_w, paddle_w, BLACK);
  
  value2 = analogRead(potpin2);
  value2 = map(value2, 0 , 4095 , 1 , 240-paddle_h);

  rpaddle_y = value2;

  tft.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);

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
    ball_dx = ball_dx * -1;}
    
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
  tft.drawString (rscoretext, w-50, 20, 2);
}
