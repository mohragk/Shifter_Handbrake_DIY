#include <Joystick.h>

// Last state of the buttons
const int pinToButtonMap = A0; //UNUSED, could be removed
int lastButtonState = 0;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
  1, 0,                  // Button Count, Hat Switch Count
  true, false, false,    // X axis, but no Y and, Z
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering


void setup() {
  // Initialize Pins
  pinMode(A0, INPUT);
  
  // Initialize Joystick Library
  Joystick.begin();
}


void loop() {

  int pot = analogRead(A0);
  pot = constrain(pot, 50, 750);
  int mapped = map(pot,50,750,0,255);
  Joystick.setXAxis(mapped);

  int currentButtonState = 0;

  //if more than half way along travel, set buttonState to 1.
  if (mapped >= 127) {
    currentButtonState = 1;
  } 


  // make sure we only change once
  if (lastButtonState != currentButtonState) {
  	Joystick.setButton(0, currentButtonState);
  	lastButtonState = currentButtonState;
  }
  

  // can be removed, just useful for debugging
  { 
    Serial.print ("XAxis:");  // prints text
    Serial.print (' ');        // prints a space
    Serial.print (pot);       // prints current byte value
    Serial.print ("\n");       // prints a newline
  }
}

