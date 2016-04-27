/*
 Joystick Shield Sample Sketch

 Reads the buttons and joystick position of SparkFun's Arduino
 Joystick Shield. The Joystick Shield is available from SparkFun's
 website:

 http://www.sparkfun.com/commerce/product_info.php?products_id=9490

 created Nov. 23, 2009
 by Ryan Owens

 modified by matz tada
 equiped with 2 servo motors
 each move refered to the Joystick value
*/

//Create variables for each button on the Joystick Shield to assign the pin numbers
char button0 = 3, button1 = 4, button2 = 5, button3 = 6;
char sel = 2;

#include <Servo.h>
Servo servo0;
Servo servo1;

#include<LiquidCrystal.h>
#define LCD_COLS 16
LiquidCrystal lcd(9, 8, 7, A5, A4, A3, A2);//         Syntax : LiquidCrystal(rs, rw, enable, d4, d5, d6, d7)
String lcdStr0 = "hello hoge";
String lcdStr1 = "humu nyann nyann ";
int lcdStrStartPosition = 0;
long pastMillis = millis() ;
long sendInterval = 500;
int eventFlag = 0;
float firstAr0deg = 0;
float firstAr1deg = 0;

void setup(void)
{
  //Buttons initialize
  pinMode(sel, INPUT);      //Set the Joystick 'Select'button as an input
  digitalWrite(sel, HIGH);  //Enable the pull-up resistor on the select button

  pinMode(button0, INPUT);      //Set the Joystick button 0 as an input
  digitalWrite(button0, HIGH);  //Enable the pull-up resistor on button 0

  pinMode(button1, INPUT);      //Set the Joystick button 1 as an input
  digitalWrite(button1, HIGH);  //Enable the pull-up resistor on button 1

  pinMode(button2, INPUT);      //Set the Joystick button 2 as an input
  digitalWrite(button2, HIGH);  //Enable the pull-up resistor on button 2

  pinMode(button3, INPUT);      //Set the Joystick button 3 as an input
  digitalWrite(button3, HIGH);  //Enable the pull-up resistor on button 3

  //Serial initialize
  Serial.begin(9600);           //Turn on the Serial Port at 9600 bps

  //servo initialize
  servo0.attach(10);
  servo1.attach(11);
  servo0.write(90);
  servo1.write(90);

  //LCD initialize
  lcd.begin(LCD_COLS, 2);

  delay(1000);

  //initialize joyStick first value
  for (int i = 0; i < 10; i++) {
    float ar0 = analogRead(0);
    float ar1 = analogRead(1);
    firstAr0deg = ar0 / 1024 * 180;
    firstAr1deg = ar1 / 1024 * 180;
  }

  Serial.println(firstAr0deg);
  Serial.println(firstAr1deg);
}

void loop(void)
{
  float ar0 = analogRead(0);
  float ar1 = analogRead(1);
  int s = digitalRead(sel);
  int b0 = digitalRead(button0);
  int b1 = digitalRead(button1);
  int b2 = digitalRead(button2);
  int b3 = digitalRead(button3);

  //servo
  float ar0deg = ar0 / 1024 * 180;
  float ar1deg = ar1 / 1024 * 180;

  //  //for safety
  //  if (ar0deg <= 45) ar0deg = 45;
  //  else if (ar0deg >= 135) ar0deg = 135;
  //  if (ar1deg <= 45) ar1deg = 45;
  //  else if (ar1deg >= 135) ar1deg = 135;

  lcdStr1 = String((int)ar0deg);         //Read the position of the joysticks X axis and print it on the serial port.
  lcdStr1 += ",";
  lcdStr1 += String((int)ar1deg);         //Read the position of the joysticks Y axis and print it on the serial port.
  lcdStr1 += ",";
  lcdStr1 += "\n";
  lcdStr0  = String(s);      //Read the value of the select button and print it on the serial port.
  lcdStr0  += ",";
  lcdStr0 += String(b0);  //Read the value of the button 0 and print it on the serial port.
  lcdStr0 += ",";
  lcdStr0 += String(b1);  //Read the value of the button 1 and print it on the serial port.
  lcdStr0 += ",";
  lcdStr0 += String(b2);  //Read the value of the button 2 and print it on the serial port.
  lcdStr0 += ",";
  lcdStr0 += String(b3); //Read the value of the button 3 and print it on the serial port.
  lcdStr0 += ",";

  servo0.write(ar0deg);
  servo1.write(ar1deg);

  //LCD
  lcd.setCursor(0, 0);
  lcd.print(lcdStr0);
  lcd.setCursor(0, 1);
  lcd.print(lcdStr1);

  //event watcher
  if (s == 0 || b0 == 0 || b1 == 0 || b2 == 0 || b3 == 0) eventFlag = 1; //buttons are active high
  if (ar0deg - firstAr0deg < -5 || 5 < ar0deg - firstAr0deg ) eventFlag = 1;
  if (ar1deg - firstAr1deg < -5 || 5 < ar1deg - firstAr1deg ) eventFlag = 1;

  //avoid congestion of the wireless siganl, think about timing and event occur
  if (eventFlag) {
    if (millis() - pastMillis > sendInterval) {
      pastMillis = millis();

      Serial.print(lcdStr0);
      Serial.print(lcdStr1);
      eventFlag = 0;
    }
  }
}
