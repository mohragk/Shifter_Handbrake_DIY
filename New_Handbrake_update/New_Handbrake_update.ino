//------------------------------
// - Connect microswitches from shifter to pin 9 and 10
// - Connect  10k potentiometer from handbrake to A0
//-----------------------------

#define MAX_SHIFTER_BTNS 2
#define PIN_BUTTON_OFFSET 9

#include "Joystick/Joystick.h"

// Last state of the buttons
int lastButtonState[MAX_SHIFTER_BTNS] = {0,0};

int lastOverrideButtonState = 0;
int overrideButtonNum = 6;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
  8, 0,                  // Button Count, Hat Switch Count
  true, false, false,    // X axis, but no Y and, Z
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering


void setup() {
  // Initialize Pins
  pinMode(A0, INPUT);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  
  // Initialize Joystick Library
  Joystick.begin();
}


void loop() {

  //update handbrake axis
  int pot    = analogRead( A0 );
  pot        = constrain( pot, 50, 750 );
  int mapped = map( pot, 50, 750, 0, 255 );
  Joystick.setXAxis( mapped );



  //if more than half way along travel, set buttonState to 1.
  int currentOverrideButtonState = 0;
  
  if ( mapped >= 127 ) 
    currentOverrideButtonState = 1;
  

  if (lastOverrideButtonState != currentOverrideButtonState) 
  {
  	Joystick.setButton(overrideButtonNum, currentOverrideButtonState);
  	lastOverrideButtonState = currentOverrideButtonState;
  }
  

  // Read pin values and update shifter buttons
  for (int i = 0; i < MAX_SHIFTER_BTNS; i++)
  {
    int currentButtonState = !digitalRead(i + PIN_BUTTON_OFFSET);

    if (currentButtonState != lastButtonState[i])
    {
      Joystick.setButton(i, currentButtonState);
      lastButtonState[i] = currentButtonState;
    }
  }


}

