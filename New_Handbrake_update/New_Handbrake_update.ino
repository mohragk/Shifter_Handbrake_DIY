//------------------------------
// - Connect microswitches from shifter to pin 9 and 10
// - Connect  10k potentiometer from handbrake to A0
//-----------------------------

#define MAX_SHIFTER_BTNS 2
#define PIN_BUTTON_OFFSET 9

// only compile relevant code when not using handbrake (0)
#define USE_HANDBRAKE 0

#include "Joystick.h"
#include "PowLUT.h"
#include <EEPROM.h>

// Last state of the buttons
int lastButtonState[MAX_SHIFTER_BTNS];

int lastHandbrakeButtonState = 0;
int handbrakeButtonNum = 6;
int handBrakeDeadzone = 50;


int   eeAddress = 0;
float eeSkewFactor = 1.0f;

#if USE_HANDBRAKE
  // create LUT for our curve mapping
  PowLUT curveMapLUT(0.5, 16, 1024);
  
  Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
  8, 0,                  // Button Count, Hat Switch Count
  true, false, false,    // X axis, but no Y and, Z
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering
#else
  Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
  MAX_SHIFTER_BTNS, 0,                  // Button Count, Hat Switch Count
  false, false, false,   // no axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering
#endif

#if USE_HANDBRAKE
	void updateCurve(float skewFactor)
	{ 
		float currentSkew = skewFactor;
		if ( currentSkew != eeSkewFactor ) 
		{
			curveMapLUT.setLUT(currentSkew, 32, 1024);

			EEPROM.put(eeAddress, currentSkew);
			eeSkewFactor = currentSkew;
		}
	}
#endif


void setup() 
{
	// Initialize Pins
#if USE_HANDBRAKE
	pinMode(A0, INPUT);
	// get our saved skewFactor and reset LUT
	EEPROM.get(eeAddress, eeSkewFactor);
	curveMapLUT.setLUT(eeSkewFactor, 32, 1024);
#endif

	pinMode(9, INPUT_PULLUP);
	pinMode(10, INPUT_PULLUP);

	memset(lastButtonState,0,sizeof(lastButtonState));

	// Initialize Joystick Library
	Joystick.begin();
}


void loop() {

#if USE_HANDBRAKE
	//update handbrake axis
	int pot    = analogRead( A0 );
	int skewed = curveMapLUT.getMappedValue( pot );

	int range = 1023;
	int pre = handBrakeDeadzone;
	int end = range - handBrakeDeadzone;
	skewed       = constrain( skewed, pre, end );

	int mapped = map( skewed, pre, end, 0, 255 );
	Joystick.setXAxis( mapped );

	//if more than half way along travel, set buttonState to 1.
	int currentHandbrakeButtonState = 0;
	if ( mapped > 127 ) currentHandbrakeButtonState = 1;

	if (lastHandbrakeButtonState != currentHandbrakeButtonState) 
	{
		Joystick.setButton(handbrakeButtonNum, currentHandbrakeButtonState);
		lastHandbrakeButtonState = currentHandbrakeButtonState;
	}
#endif //USE_HANDBRAKE 

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

