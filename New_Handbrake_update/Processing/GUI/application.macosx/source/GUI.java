import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class GUI extends PApplet {




Serial port;

ControlP5 cp5;
CallbackListener cb;
Slider cSlider, zSlider;
ScrollableList portSelector;

PFont font;
PImage curveImg;


public void setup()
{
  
  
  curveImg = createImage(200 , 200, ARGB);
  
  port = new Serial(this, Serial.list()[1], 9600);
  
  cp5 = new ControlP5(this);
  font = createFont("Futura", 20);
  
    portSelector = cp5.addScrollableList("portSelect")
     .setPosition(40, 40)
     .setSize(200, 100)
     .setBarHeight(20)
     .setItemHeight(20)
     .addItems(Serial.list())
     .close()
     .setType(ScrollableList.DROPDOWN)
     .setLabel("Select Port")
     ;
  
  cSlider = cp5.addSlider("curveSlider")
    .setRange(0, 1023 - 256)
    .setValue(0)
    .setPosition(40, curveImg.height + 200)
    .setSize(200,20)
    .setTriggerEvent(Slider.PRESS)
    .setLabel("Curvature")
    ;
    
  // add callback to only send data when released  
  cSlider.addCallback(new CallbackListener() 
     {
      public void controlEvent(CallbackEvent theEvent) {
        if (   theEvent.getAction()==ControlP5.ACTION_RELEASE 
            || theEvent.getAction()==ControlP5.ACTION_RELEASE_OUTSIDE
            ) 
        {
          createAndSendCommand(1023 - (int)cSlider.getValue(), "#SKEW");
        }
      }
    }
  );
  
  
  zSlider = cp5.addSlider("deadzoneSlider")
    .setRange(0, 512)
    .setValue(0)
    .setPosition(40, height - 120)
    .setSize(200,20)
    .setTriggerEvent(Slider.RELEASE)
    .setLabel("Deadzone");
    ;
  
}


public void setPort(int num)
{
    //port = null;
    port.clear();
    port.stop();
    
    port = new Serial(this, Serial.list()[num], 9600);
}

public void draw()
{
  background(0);
  
  while (port.available() > 0) {
    String inBuffer = port.readString();   
    if (inBuffer != null) 
    {
      println(inBuffer);
    }
  }
  
  drawImage(curveImg);
}




public void fillImage(PImage img, float skew)
{
    float x = 1024;
    float x_stride = 1024 / img.width;
  
    img.loadPixels();
    for (int i = 0; i < img.pixels.length ; i++)
    {
        int y = 0;
    
        y = round( getSkewedValue((int)x, skew) );
        y = round(map(y, 0, 1023, 0 , img.width));
        int strokewidth = 2;
        
        if ( y - strokewidth <= i % img.width && y >= i % img.width )
          img.pixels[i] = color(255, 255, 255, 220);
        else
          img.pixels[i] = color(255, 255, 255, 0);
        
        if (i % img.width == 0)
          x -= x_stride;
    }
    img.updatePixels();
    
}


public void drawImage(PImage img)
{
    image(img, 40, 120);
}

public int getSkewedValue(int value, float skew)
{
    float norm = (float)value / 1024;
    float skewed = pow( norm, skew );

    return round(skewed * 1024);
}

public void curveSlider(int value)
{
    // update image
    float skewFactor = (1023 - (float)value) / 1024;
    fillImage(curveImg, skewFactor);
}


public void deadzoneSlider(int value)
{
    String message = "#ZONE";
    message += str(value);
    message += "\n";
    
    port.write(message);
}

public void portSelect(int n) {
  setPort( n );
}

public void createAndSendCommand(int val, String name)
{
    String message = name;
    message += str(val);
    message += "\n";
    
    if (port != null)
      port.write(message);
}
  public void settings() {  size(360, 560);  smooth(16); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "GUI" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
