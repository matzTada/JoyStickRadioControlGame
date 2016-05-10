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
}

void loop(void)
{
  float ar0 = analogRead(0);
  float ar1 = analogRead(1);
  Serial.print(ar0);          //Read the position of the joysticks X axis and print it on the serial port.
  Serial.print(",");
  Serial.print(ar1);          //Read the position of the joysticks Y axis and print it on the serial port.
  Serial.print(",");
  Serial.print(digitalRead(sel));       //Read the value of the select button and print it on the serial port.
  Serial.print(",");
  Serial.print(digitalRead(button0));   //Read the value of the button 0 and print it on the serial port.
  Serial.print(",");
  Serial.print(digitalRead(button1));   //Read the value of the button 1 and print it on the serial port.
  Serial.print(",");
  Serial.print(digitalRead(button2));   //Read the value of the button 2 and print it on the serial port.
  Serial.print(",");
  Serial.print(digitalRead(button3)); //Read the value of the button 3 and print it on the serial port.
  Serial.print(", ");

  float ar0deg = ar0 / 1024 * 180;
  float ar1deg = ar1 / 1024 * 180;
  Serial.print(ar0deg);          //Read the position of the joysticks X axis and print it on the serial port.
  Serial.print(",");
  Serial.print(ar1deg);          //Read the position of the joysticks Y axis and print it on the serial port.
  Serial.print(",");
  Serial.println("");

  servo0.write(ar0deg);
  servo1.write(ar1deg);

  //Wait for 100 ms, then go back to the beginning of 'loop' and repeat.
  delay(100);
}
