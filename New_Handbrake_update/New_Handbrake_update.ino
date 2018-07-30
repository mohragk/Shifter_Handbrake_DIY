//------------------------------
// - Connect microswitches from shifter to pin 9 and 10
// - Connect  10k potentiometer from handbrake to A0
//-----------------------------

#define MAX_SHIFTER_BTNS 2
#define PIN_BUTTON_OFFSET 9

#include "Joystick.h"
#include "PowLUT.h"

// Last state of the buttons
int lastButtonState[MAX_SHIFTER_BTNS];

int lastHandbrakeButtonState = 0;
int handbrakeButtonNum = 6;

PowLUT mediumCurveLUT(0.5, 16);

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

  memset(lastButtonState,0,sizeof(lastButtonState));
  
  // Initialize Joystick Library
  Joystick.begin();
}


int getSkewedValue(int mappedValue, float skew)
{
  float normalised = (float)mappedValue / 1023.0;
  
  float skewed = pow(normalised, skew);
  
  return static_cast<int> (skewed * 1023);
}

void loop() {

  //update handbrake axis
  int pot    = analogRead( A0 );

  int skewed = mediumCurveLUT.getMappedValue(pot);
  
  skewed       = constrain( skewed, 50, 750 );
  int mapped = map( skewed, 50, 750, 0, 255 );
  Joystick.setXAxis( mapped );



  //if more than half way along travel, set buttonState to 1.
  int currentHandbrakeButtonState = 0;
  
  if ( mapped > 127 ) 
    currentHandbrakeButtonState = 1;
  

  if (lastHandbrakeButtonState != currentHandbrakeButtonState) 
  {
  	Joystick.setButton(handbrakeButtonNum, currentHandbrakeButtonState);
  	lastHandbrakeButtonState = currentHandbrakeButtonState;
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

