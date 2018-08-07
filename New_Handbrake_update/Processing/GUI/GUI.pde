import controlP5.*;
import processing.serial.*;

Serial port;

ControlP5 cp5;
PFont font;

void setup()
{
  size(460, 320);
  
  printArray(Serial.list());
  
  port = new Serial(this, Serial.list()[1], 9600);
  
  cp5 = new ControlP5(this);
  font = createFont("Futura", 20);
  
    
  cp5.addSlider("curveSlider")
    .setRange(256, 1023)
    .setValue(1023)
    .setPosition(width * 0.1, height*0.1)
    .setSize(20,200)
    .setTriggerEvent(Slider.RELEASE)
    ;
}

void draw()
{
  background(0);
  
  while (port.available() > 0) {
    String inBuffer = port.readString();   
    if (inBuffer != null) {
      println(inBuffer);
    }
  }
}

void curveSlider(int value)
{
    String message = "#SKEW";
    message += str(value);
    message += "\n";
    
    port.write(message);
    println(message);
}
