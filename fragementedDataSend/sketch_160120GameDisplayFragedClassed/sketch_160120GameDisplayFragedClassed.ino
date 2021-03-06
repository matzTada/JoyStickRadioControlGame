/*
  SerialLcd.ino

    This sketch displays characters sent via serial port.
    Software flow control (XON/XOFF) is adopted, so usually both RX and TX signals must be connected.
  However, if you send characters slowly enough, no flow control is needed, so you can use only RX signal.
    This program sends a 'F' character following XOFF and sends a 'N' character following XON in order to
  visualize software flow control.
    This file is designed for multi-device.  YOU HAVE TO MODIFY A PART OF THIS FILE to accommodate it to your design.

*/

#include <mglcd.h>

#define XON  0x11 // XON character
#define XOFF 0x13 // XOFF character
#define XON_SIZE  16 // XON is sent when result of Serial.available() function reaches this value
#define XOFF_SIZE 32 // XOFF is sent when result of Serial.avaiiable() function reaches this value

// The following table illustrates SG12232 LCD-Arduino pin assignemnts.
// If you edit the following table, you can change pin assignemts.  Maximum performance is, however,
// obtained in default pin assignments.
// LCD's RES pin must be connected to Arduino's RESET pin although RES and RESET pins are not mentioned
// in the table.
// CS1 and CS2 pins require pull up registers to keep their level high while Arduino is initializing
// or programming.
// Avoid to connect CS1 or CS2 to a Arduino's serial pin (1, 2) or a ICSP pin (11, 12, 13) becasuse
// serial pins and ICSP pins can change thier logic levels while initializing or programming.
// LCD's CL pin must be connected to 2kHz clock oscillator.  Although this 2kHz clock signal can be
// generated utilizing an Arduino's timer, it is not recommended.  You have to feed the 2kHz clock
// signal while VDD is supplied to LCD.  But Arduino doesn't generate 2kHz clock while initializing
// or programming.  It can harm the LCD pannel.  An external dedicated clock oscillator is recommemded.
const TLcdPinAssignTable PinAssignTable = {
  //    A0_DI  : A3, // A0 for SG12232, D/I for SG12864
  //    CS1_E1 : A0, // CS1
  //    CS2_E2 : A1, // CS2
  A0_DI  : 3, // A0 for SG12232, D/I for SG12864
  CS1_E1 : 13, // CS1
  CS2_E2 : 12, // CS2
E      : A4,
RW     : A2,
  DB0    : 8 ,
  DB1    : 9 ,
  DB2    : 10,
  DB3    : 11,
  DB4    : 4 ,
  DB5    : 5 ,
  DB6    : 6 ,
  DB7    : 7
}; // PinAssignTable;

static mglcd_SG12864 MGLCD(PinAssignTable);

static bool xon;
boolean CrReceived = false;

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

//circle
int circleX = 15;
int circleY = 31;

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
      MGLCD.Circle(x - 1, y - 1, 2, 0);
      MGLCD.Circle(x, y, 2);
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
long invertInterval = 300L * 1000L;
long invertFlag = 0;

void setup() {
  MGLCD.Reset();
  Serial.begin(9600);
  while (!Serial);
  delay(2000);
  Serial.println(F("JoyStick Game Started by MatGamez."));
  MGLCD.println(F("JoyStick Game Started by MatGamez"));
  MGLCD.SetScrollMode(MGLCD_SCROLL);
  invertFlag = 0;
  MGLCD.SetInvertMode(MGLCD_NON_INVERT);
  Serial.write(XON);
  xon = true;
} // setup

void loop() {
  if (millis() - invertPastMillis > invertInterval) {
    invertPastMillis = millis();
    invertFlag = 1 - invertFlag;
    if (invertFlag) MGLCD.SetInvertMode(MGLCD_INVERT);
    else MGLCD.SetInvertMode(MGLCD_NON_INVERT);
  }

  // Software flow control.
  if (xon && Serial.available() >= XOFF_SIZE) {
    Serial.write(XOFF);
    xon = false;
    Serial.print('F'); // This line is added to visualize flowcontrol.  You can comment out this line if you don't need visualization.
  } // if
  if (!xon && Serial.available() <= XON_SIZE) {
    Serial.write(XON);
    xon = true;
    Serial.print('N'); // This line is added to visualize flowcontrol.  You can comment out this line if you don't need visualization.
  } // if

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
      MGLCD.ClearScreen();
      MGLCD.Locate(0, 2);
      MGLCD.print(receiveString);
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

      circleX += tiltX;
      circleY += tiltY;
      if (circleX > 127 - 8) circleX = 127 - 8;
      if (circleX < 0 + 8) circleX = 0 + 8;
      if (circleY > 63 - 8) circleY = 63 - 8;
      if (circleY < 0 + 8) circleY = 0 + 8;

      if (b0 == 0 || b1 == 0 || b2 == 0 || b3 == 0) { //shoot bullet!!!
        int tempVX = 0, tempVY = 0;
        if (b0 == 0) tempVX = 1;
        if (b1 == 0) tempVY = -1;
        if (b2 == 0) tempVY = 1;
        if (b3 == 0) tempVX = -1;

        bullets[allBulletCounter].shoot(circleX, circleY, tempVX, tempVY);

        allBulletCounter++;
        if (allBulletCounter >= NUM_OF_BULLETS) allBulletCounter = 0;
      }

      if (s == 0) { //shoot burst bullet!!!!!!
        for (int i = 0; i < 5; i++) {
          allBulletCounter++;
          if (allBulletCounter >= NUM_OF_BULLETS) allBulletCounter = 0;
          bullets[allBulletCounter].shoot(circleX, circleY + (i - 2) * 7, 2, 0);
        }
      }
    }

    //    // Print data on the LCD screen.
    //    if (data >= 0) {
    //      MGLCD.PutChar(data);
    //      // Serial.write(data);  // If you need echo back, enable this line.
    //    } // if

  } // if

  if (millis() - waitScreenMillis > waitScreenInterval) {
    if (millis() - waitPastMillis > waitPastInterval) {
      waitPastMillis = millis();
      MGLCD.print(F("I wait for your controlling."));
    }
  } else {

    //display functions
    MGLCD.Locate(0, 0);
    MGLCD.print(millis() - pastMillis);
    MGLCD.Locate(0, 1);
    MGLCD.print(hitCounter);
    pastMillis = millis();
    delay(10);

    //display circle
    if (fillFlag)   MGLCD.FillCircle(circleX, circleY, 8);
    else  MGLCD.Circle(circleX, circleY, 8);

    //display enemy change
    int enemyPositionChange = random(1, 3);
    if (enemyPositionChange >= 2) enemyPositionChange = (enemyPositionChange - 1) * -1;
    enemyY += enemyPositionChange;
    if (enemyY > 63 - 4) enemyY = 63 - 4;
    if (enemyY < 0 + 4) enemyY = 0 + 4;
    MGLCD.Circle(enemyX, enemyY, 4);

    //display button preesed info
    //lower left joy stick
    if (s) {
      MGLCD.Rect(0, 63 - 15, 15, 63); //frame
      MGLCD.FillRect(7 + tiltX * 2 , 63 - 15 + 7 + tiltY * 2, 7 + tiltX * 2 + 1, 63 - 15 + 7 + tiltY * 2 + 1);
    } else {
      MGLCD.FillRect(0, 63 - 15, 15, 63); //frame
      MGLCD.FillRect(7 + tiltX * 2 , 63 - 15 + 7 + tiltY * 2, 7 + tiltX * 2 + 1, 63 - 15 + 7 + tiltY * 2 + 1, 0);
    }
    //lower right buttons
    //right button
    if (b0) MGLCD.Rect(127 - 5, 63 - 10, 127, 63 - 5);
    else MGLCD.FillRect(127 - 5, 63 - 10, 127, 63 - 5);
    //top button
    if (b1)  MGLCD.Rect(127 - 10, 63 - 15, 127 - 5, 63 - 10);
    else  MGLCD.FillRect(127 - 10, 63 - 15, 127 - 5, 63 - 10);
    //bottom button
    if (b2)  MGLCD.Rect(127 - 10, 63 - 5, 127 - 5, 63);
    else MGLCD.FillRect(127 - 10, 63 - 5, 127 - 5, 63);
    //left button
    if (b3)  MGLCD.Rect(127 - 15, 63 - 10, 127 - 10, 63 - 5);
    else MGLCD.FillRect(127 - 15, 63 - 10, 127 - 10, 63 - 5);

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
