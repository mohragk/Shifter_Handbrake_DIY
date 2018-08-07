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
  
  cp5.addButton("Curvature")
    .setPosition(100,50)
    .setSize(120, 50)
    ;
}

void draw()
{
  background(0);
}


void Curvature()
{
  
}
