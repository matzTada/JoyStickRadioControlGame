/*
2015/11/12
 JoyStickShield visualizer 
 please use this with 151111JoyStrickShieldServoLCD
 
 serial strings must be
 sel,r,t,b,l,Alr,Abt
 ex) 1,1,1,1,1,90.00,90.00
 
 */

import processing.serial.*;

Serial myPort;  // Create object from Serial class
String myString = "";      // Data received from the serial port
String tempStr [];

PImage offscr;

int Bsel, Br, Bt, Bb, Bl;
float Alr, Abt;

void setup() 
{
  size(340, 220);
  myPort = new Serial(this, "COM35", 9600);

  offscr = createImage(width, height, RGB);
}

void draw()
{
  //  //off screen buffer first <===
  //  loadPixels();
  //  offscr.pixels = pixels;
  //  offscr.updatePixels();
  //  //===>

  fill(0, 30);
  rect(0, 0, width, height);

  if ( myPort.available() > 0) {  // If data is available,
    myString = myPort.readStringUntil('\n');         // read it and store it in val
    if (myString != null) {
      print(myString);
      tempStr = myString.split(",");
      try {
        Bsel = int(tempStr[0]); //select button
        Br = int(tempStr[1]); //right button
        Bt = int(tempStr[2]); //top button
        Bb = int(tempStr[3]); //bottom button
        Bl = int(tempStr[4]); //left button
        Alr = float(tempStr[5]); //angle between left and right of joy stick left:0, right:180
        Abt = float(tempStr[6]); //angle between top and bottom of joy stick bottom:0, top:180
      }
      catch(Exception e) {
      }

      //fill buttons
      stroke(255, 200);
      strokeWeight(1);
      ellipse(60, 100, 40, 40);
      ellipse(20 + Alr / 180 * 80, 60 + (180 - Abt) / 180 * 80, 20, 20);
      rect(280, 80, 40, 40);
      rect(240, 40, 40, 40);
      rect(240, 120, 40, 40);
      rect(200, 80, 40, 40);
      noStroke();

      fill(255, 0, 0);
      if (Bsel == 0) ellipse(60, 100, 40, 40);
      ellipse(20 + Alr / 180 * 80, 60 + (180 - Abt) / 180 * 80, 20, 20);
      stroke(255, 0, 0);
      strokeWeight(5);
      line(60, 100, 20 + Alr / 180 * 80, 60 + (180 - Abt) / 180 * 80);
      noStroke();
      if (Br == 0) rect(280, 80, 40, 40);
      if (Bt == 0) rect(240, 40, 40, 40);
      if (Bb == 0) rect(240, 120, 40, 40);
      if (Bl == 0) rect(200, 80, 40, 40);
      noFill();
    }
  }

  //  //off screen buffer second <===
  //  tint(255, 250);
  //  image(offscr, 0, 0, width, height);
  //  //===>
}

