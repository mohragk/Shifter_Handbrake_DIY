//------------------------------
// - Connect microswitches from shifter to pin 9 and 10
// - Connect  10k potentiometer from handbrake to A0
//
// Serial command protocol: # NAME VALUE /n (without spaces)
//-----------------------------


#include "Joystick.h"


#define MAX_SHIFTER_BTNS 2
#define PIN_BUTTON_OFFSET 9

// only compile relevant code when not using handbrake (0)
#define USE_HANDBRAKE false
#define USE_SERIAL 0

// Last state of the buttons
int lastButtonState[MAX_SHIFTER_BTNS];

int lastHandbrakeButtonState = 0;
int handbrakeButtonNum = 6;

float skewFactor = 1.0f;
int deadZone = 0;




#if USE_HANDBRAKE
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

#if USE_SERIAL
    // serial variables
    String  inputString = "";         // a string to hold incoming data
    boolean stringComplete = false;  // whether the string is complete
    String  commandString = "";

    
    void serialEvent(String& output)
    {
        while (Serial.available()) 
        {
          char inChar = (char)( Serial.read() );
          output += inChar;
          
          // if the incoming character is a newline, set a flag
          // so the main loop can do something about it:
          if (inChar == '\n') {
            stringComplete = true;
            
          }
      }
    }
    
    String getCommand(String input)
    {
      if ( input.length() > 0 )
         return input.substring(1,5); 
      else
        return "";
    }
    
    void parseCommand(String& input)
    {
        if (stringComplete)
        {
             String command = getCommand(input);

             if      ( command.equals("SKEW") )
             {
                 String value = input.substring(5,9);
                 skewFactor = static_cast<float>( value.toInt() ) / 1024;
                 Serial.print("Skew command received: ");
                 Serial.print(skewFactor);
                 Serial.println();
             }
             else if ( command.equals("ZONE") )
             {
                String value = input.substring(5,9);
                deadZone = value.toInt();
                Serial.print("Deadzone command received: ");
                Serial.print(deadZone);
                Serial.println();
             }
             else if ( command.equals("TEST") )
             {
                String value = input.substring(5,9);
                testBrakePos = value.toInt();
                Serial.print("HB Test command received: ");
                Serial.print(testBrakePos);
                Serial.println();
             }
             
             
             input = "";
             stringComplete = false;
             
        }
    }
#endif //USE_SERIAL


#if USE_HANDBRAKE
    int getSkewedValue(int value, float skew)
    {

        float norm = (float)value / 1024;
        float skewed = pow( norm, skew );
    
        return static_cast<int> (round( skewed * 1024) );
    }

#endif //USE_HANDBRAKE



void setup() 
{
	  // Initialize Pins
    pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    
#if USE_HANDBRAKE
    pinMode(A0, INPUT);
#endif

#if USE_SERIAL
  Serial.begin(9600);
#endif

    memset( lastButtonState, 0, sizeof(lastButtonState) );

    // Initialize Joystick Library
    Joystick.begin();
   
}


void loop() {

#if USE_SERIAL
    serialEvent(inputString);
    parseCommand(inputString);
#endif

#if USE_HANDBRAKE
    //update handbrake axis
    int pot    = analogRead( A0 );
  
    int skewed = getSkewedValue(pot, skewFactor);
    skewed     = constrain(skewed, deadZone, 1023);
    int mapped = map(skewed, deadZone, 1023, 0, 255);
    Joystick.setXAxis(mapped);
    
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

    delay(10);
}

