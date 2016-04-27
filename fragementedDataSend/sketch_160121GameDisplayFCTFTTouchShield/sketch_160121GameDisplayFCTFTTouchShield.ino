#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

String receiveString = "";
long pastMillis = millis();

//button s
int firstAr0deg = 93;
int firstAr1deg = 90;

int s = 0;
int b0 = 0;
int b1 = 0;
int b2 = 0;
int b3 = 0;
int ar0deg = 0;
int ar1deg = 0;

//display
int fillFlag = 0;
int tiltX = 0;
int tiltY = 0;

//drawCircle
int drawCircleX = 15;
int drawCircleY = 31;

//enemy
int enemyX = 127 - 15;
int enemyY = 31;
int hitCounter = 0;

//bullet
int allBulletCounter = 0;
#define NUM_OF_BULLETS 10

class Bullet {
  public:
    int flag;
    int x;
    int y;
    int vx;
    int vy;
    int count;

    void shoot(int _x, int _y, int _vx, int _vy) {
      flag = 1;
      x = _x;
      y = _y;
      vx = _vx;
      vy = _vy;
    }

    void fly() {
      x += vx;
      y += vy;
      if (x < 0 || x > 127 || y < 0 || y > 63) flag = 0;
    }

    void judge(int _targetX, int _targetY) {    //judge hit
      if (x == _targetX) {
        if (abs(_targetY - y) < 4) {
          hitCounter++;
        }
      }
    }

    void show() {
      tft.drawCircle(x - vx, y - vy, 2, ILI9341_BLACK);
      tft.drawCircle(x, y, 2, ILI9341_GREEN);
    }
};

Bullet bullets[NUM_OF_BULLETS];

//waitScreenMillis
long waitScreenMillis = millis();
long waitScreenInterval = 5000;
long waitPastMillis = millis();
long waitPastInterval = 500;

//invert switching
long invertPastMillis = millis();
long invertInterval = 1L * 1000L;
long invertFlag = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!");

  tft.begin();

  while (!Serial);
  delay(2000);
  Serial.println(F("JoyStick Game Started by MatGamez."));
  tft.println(F("JoyStick Game Started by MatGamez"));
  invertFlag = 0;

  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
} // setup

void loop() {
  //  if (millis() - invertPastMillis > invertInterval) {
  //    invertPastMillis = millis();
  //  tft.fillScreen(ILI9341_BLACK);
  //   invertFlag = 1 - invertFlag;
  //    if (invertFlag) tft.SetInvertMode(tft_INVERT);
  //    else tft.SetInvertMode(tft_NON_INVERT);
  //  }


  if (Serial.available()) {
    int data;

    data = Serial.read();
    waitScreenMillis = millis();

    receiveString += (char)data;
    if (receiveString.length() > 30) {
      receiveString = "";
    }

    if (data == '\n') {
      //clearScreen
      tft.fillRect(0, 0, 0 + 16 * 2, 0 + 100, ILI9341_BLACK);
      tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
      tft.setCursor(0, 16 * 2);
      tft.print(receiveString);
      receiveString.trim();

      int ordinal = 0;
      String temp = "";

      b0 = 1;
      b1 = 1;
      b2 = 1;
      b3 = 1;
      s = 1;

      for (int i = 0; i < receiveString.length(); i++) {
        char c = receiveString.charAt(i);
        switch (c) {
          case 'a':
            b0 = receiveString.substring(i + 1, i + 2).toInt();
            break;
          case 'b':
            b1 = receiveString.substring(i + 1, i + 2).toInt();
            break;
          case 'c':
            b2 = receiveString.substring(i + 1, i + 2).toInt();
            break;
          case 'd':
            b3 = receiveString.substring(i + 1, i + 2).toInt();
            break;
          case 'e':
            s = receiveString.substring(i + 1, i + 2).toInt();
            break;
          case 'f':
            ar0deg = receiveString.substring(i + 1, i + 4).toInt();
            break;
          case 'g':
            ar1deg = receiveString.substring(i + 1, i + 4).toInt();
            break;
        }
      }
      receiveString = "";

      //judge and execute command
      fillFlag = 0;
      if (s == 0 || b0 == 0 || b1 == 0 || b2 == 0 || b3 == 0)  fillFlag = 1;//buttons are active high
      //horizontal tilt
      if (ar0deg - firstAr0deg < -60) tiltX = -3;
      else if (ar0deg - firstAr0deg < -30) tiltX = -2;
      else if (ar0deg - firstAr0deg < -5) tiltX = -1;
      else if (60 < ar0deg - firstAr0deg ) tiltX = 3;
      else if (30 < ar0deg - firstAr0deg ) tiltX = 2;
      else if (5 < ar0deg - firstAr0deg ) tiltX = 1;
      else tiltX = 0;
      //vertical tilt
      if (ar1deg - firstAr1deg < -60) tiltY = 3;
      else if (ar1deg - firstAr1deg < -30) tiltY = 2;
      else if (ar1deg - firstAr1deg < -5) tiltY = 1;
      else if (60 < ar1deg - firstAr1deg) tiltY = -3;
      else if (30 < ar1deg - firstAr1deg) tiltY = -2;
      else if (5 < ar1deg - firstAr1deg) tiltY = -1;
      else tiltY = 0;

      tft.fillCircle(drawCircleX, drawCircleY, 8, ILI9341_BLACK);
      drawCircleX += tiltX;
      drawCircleY += tiltY;
      if (drawCircleX > 127 - 8) drawCircleX = 127 - 8;
      if (drawCircleX < 0 + 8) drawCircleX = 0 + 8;
      if (drawCircleY > 63 - 8) drawCircleY = 63 - 8;
      if (drawCircleY < 0 + 8) drawCircleY = 0 + 8;

      if (b0 == 0 || b1 == 0 || b2 == 0 || b3 == 0) { //shoot bullet!!!
        int tempVX = 0, tempVY = 0;
        if (b0 == 0) tempVX = 1;
        if (b1 == 0) tempVY = -1;
        if (b2 == 0) tempVY = 1;
        if (b3 == 0) tempVX = -1;

        bullets[allBulletCounter].shoot(drawCircleX, drawCircleY, tempVX, tempVY);

        allBulletCounter++;
        if (allBulletCounter >= NUM_OF_BULLETS) allBulletCounter = 0;
      }

      if (s == 0) { //shoot burst bullet!!!!!!
        for (int i = 0; i < 5; i++) {
          allBulletCounter++;
          if (allBulletCounter >= NUM_OF_BULLETS) allBulletCounter = 0;
          bullets[allBulletCounter].shoot(drawCircleX, drawCircleY + (i - 2) * 7, 2, 0);
        }
      }
    }

    //    // Print data on the LCD screen.
    //    if (data >= 0) {
    //      tft.PutChar(data);
    //      // Serial.write(data);  // If you need echo back, enable this line.
    //    } // if

  } // if

  if (millis() - waitScreenMillis > waitScreenInterval) {
    if (millis() - waitPastMillis > waitPastInterval) {
      waitPastMillis = millis();
      tft.print(F("I wait for your controlling."));
    }
  } else {

    //display functions
    tft.fillRect(0, 0, 0 + 16 * 2, 0 + 100, ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.print(millis() - pastMillis);
    tft.setCursor(0, 16);
    tft.print(hitCounter);
    pastMillis = millis();
    delay(10);

    //display drawCircle
    if (fillFlag)   tft.fillCircle(drawCircleX, drawCircleY, 8, ILI9341_GREEN);
    else  tft.drawCircle(drawCircleX, drawCircleY, 8, ILI9341_GREEN);

    //display enemy change
    int enemyPositionChange = random(1, 3);
    if (enemyPositionChange >= 2) enemyPositionChange = (enemyPositionChange - 1) * -1;
    tft.drawCircle(enemyX, enemyY, 4, ILI9341_BLACK);
    enemyY += enemyPositionChange;
    if (enemyY > 63 - 4) enemyY = 63 - 4;
    if (enemyY < 0 + 4) enemyY = 0 + 4;
    tft.drawCircle(enemyX, enemyY, 4, ILI9341_GREEN);

    //display button preesed info
    //lower left joy stick
    if (s) {
      tft.drawRect(0, 63 - 15, 15, 63, ILI9341_GREEN); //frame
      tft.fillRect(7 + tiltX * 2 , 63 - 15 + 7 + tiltY * 2, 7 + tiltX * 2 + 1, 63 - 15 + 7 + tiltY * 2 + 1, ILI9341_GREEN);
    } else {
      tft.fillRect(0, 63 - 15, 15, 63, ILI9341_GREEN); //frame
      tft.fillRect(7 + tiltX * 2 , 63 - 15 + 7 + tiltY * 2, 7 + tiltX * 2 + 1, 63 - 15 + 7 + tiltY * 2 + 1, ILI9341_GREEN);
    }
    //lower right buttons
    //right button
    if (b0) tft.drawRect(127 - 5, 63 - 10, 127, 63 - 5, ILI9341_GREEN);
    else tft.fillRect(127 - 5, 63 - 10, 127, 63 - 5, ILI9341_GREEN);
    //top button
    if (b1)  tft.drawRect(127 - 10, 63 - 15, 127 - 5, 63 - 10, ILI9341_GREEN);
    else  tft.fillRect(127 - 10, 63 - 15, 127 - 5, 63 - 10, ILI9341_GREEN);
    //bottom button
    if (b2)  tft.drawRect(127 - 10, 63 - 5, 127 - 5, 63, ILI9341_GREEN);
    else tft.fillRect(127 - 10, 63 - 5, 127 - 5, 63, ILI9341_GREEN);
    //left button
    if (b3)  tft.drawRect(127 - 15, 63 - 10, 127 - 10, 63 - 5, ILI9341_GREEN);
    else tft.fillRect(127 - 15, 63 - 10, 127 - 10, 63 - 5, ILI9341_GREEN);

    //bullet
    for (int i = 0; i < NUM_OF_BULLETS; i++) {
      if (bullets[i].flag) {
        bullets[i].fly();
      }
    }
    for (int i = 0; i < NUM_OF_BULLETS; i++) {
      if (bullets[i].flag) {
        bullets[i].show();
        bullets[i].judge(enemyX, enemyY);
      }
    }
  }
} // loop
