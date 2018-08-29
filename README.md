# Handbrake and Sequential Shifter controller using a (Arduino) Pro Micro
Example code for a sequential shifter and analog handbrake combo. A GUI (made with P5/Processing) is included for editing the handbrake response curve and deadzone. 

## Usage
#### Pro Micro
Connect microswitches to pin 9 and 10 for the sequential shifter and a potentiometer to pin A0 for the handbrake.   

Clone the repo to your harddrive and upload to your Pro Micro. It is recommended you use the 16Mhz version of the board and use the Sparkfun Arduino addon. 
[Check the Sparkfun site for deails](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide)   

The sketch uses the Timer library, so install that if you don't have it already:
[Timer library for Arduino](https://playground.arduino.cc/code/timer)   

#### GUI
The GUI uses the ControlP5 library for all the sliders and buttons. You need to add that library to [Processing](https://processing.org/) to use the sketch. [Check out the ControlP5 site for instructions](http://www.sojamo.de/libraries/controlP5/)   

Then, compile the GUI with [Processing](https://processing.org/). You can export the GUI as an executable if you don't want to run Processing everytime.   
The GUI is designed as an interface for the board to set a curve for the handbrake response and a deadzone. These settings are stored on the EEPROM of the Pro Micro. That way, you don't have to run the GUI to keep the curvature and deadzone.  

**IMPORTANT**  
The GUI checks at 2 second intervals for values stored on the Pro Micro to sync the sliders to the values of the board. Simply wait a moment when running the GUI to sync up to the board.   

## Pro Micro sketch preprocessor defines
I've used preprocessor defines to 'turn on' or 'turn off' code that shouldn't be compiled. Meaning, if you only want to use the code to run a Shifter, set `USE_HANBRAKE` to 0: 
```c++
#define USE_HANDBRAKE 0
```
   
I've added 4 defines:
- `USE_HANDBRAKE`, set this to 1 if you want to use the Handbrake code
- `USE_SERIAL`, set this to 1 if you want to use the GUI
- `SINE_TEST`, set this to 1 if you want to test the Handbrake without a potmeter. It will sweep periodically.
- `USE_EEPROM`, set this to 1 if you want to store settings on the Pro Micro, or else you would have to run the GUI.
   
You have to re-upload the sketch every time you change these defines.

## The GUI/Pro Micro command system
In order to use a GUI to edit settings on the board, I made a command system that sends command messages via the serial connection, to and from the board. If you want to expand the GUI and add more functionality, the command protocol is as follows:
```c++
<N,0000>
```
- '<' denotes the beginning of a command.  
- 'N' is the name of the command and must be unique for each slider/knob/button.  
- ',' denotes the beginning of a value. Values can be any unsigned integer and should be ideally within the range 0 to 1023.  
- '>' denotes the end of a command.   

Commands that are send form the board to the GUI have the following protocol:
```c++
[N,0000]
```
Check the source code for examples on how to use this system and feel free to aks if you have any questions.   

## Disclaimer
Use at your own risk, this code is not thorougly tested.

