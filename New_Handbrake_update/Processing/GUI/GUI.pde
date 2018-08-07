import controlP5.*;
import processing.serial.*;

Serial port;

ControlP5 cp5;
CallbackListener cb;
Slider cSlider, zSlider;

PFont font;
PImage curveImg;


void setup()
{
  size(640, 560);
  curveImg = createImage(120 , 120, ARGB);
  
  printArray(Serial.list());
  
  port = new Serial(this, Serial.list()[1], 9600);
  
  cp5 = new ControlP5(this);
  font = createFont("Futura", 20);
  
  
  
  cSlider = cp5.addSlider("curveSlider")
    .setRange(0, 1023 - 256)
    .setValue(0)
    .setPosition(width * 0.1 - 20, curveImg.height + 80)
    .setSize(200,20)
    .setTriggerEvent(Slider.PRESS)
    ;
    
  zSlider = cp5.addSlider("deadzoneSlider")
    .setRange(0, 512)
    .setValue(0)
    .setPosition(width * 0.75 - 20, height * 0.1)
    .setSize(20,200)
    .setTriggerEvent(Slider.RELEASE)
    ;
    
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
  
  
  drawImage(curveImg);
}




void fillImage(PImage img, float skew)
{
    float x = 1024;
    float x_stride = 1024 / img.width;
  
    img.loadPixels();
    for (int i = 0; i < img.pixels.length ; i++)
    {
        int y = 0;
    
        y = round( getSkewedValue((int)x, skew) );
        y = round(map(y, 0, 1023, 0 , img.width));
        int strokewidth = 1;
        
        if ( y <= i % img.width && y + strokewidth >= i % img.width )
          img.pixels[i] = color(255, 255, 255, 220);
        else
          img.pixels[i] = color(255, 255, 255, 0);
        
        if (i % img.width == 0)
          x -= x_stride;
    }
    img.updatePixels();
    
}


void drawImage(PImage img)
{
    image(img, width * 0.1 - 20, 40);
}

int getSkewedValue(int value, float skew)
{
    float norm = (float)value / 1024;
    float skewed = pow( norm, skew );

    return round(skewed * 1024);
}

void curveSlider(int value)
{
    /*  
    String message = "#SKEW";
    message += str(1023 - value);
    message += "\n";
    
    port.write(message);
    */
    
    float skewFactor = (1023 - (float)value) / 1024;
    fillImage(curveImg, skewFactor);
   
}


void deadzoneSlider(int value)
{
    String message = "#ZONE";
    message += str(value);
    message += "\n";
    
    port.write(message);
}


void createAndSendCommand(int val, String name)
{
    String message = name;
    message += str(val);
    message += "\n";
    
    port.write(message);
}
