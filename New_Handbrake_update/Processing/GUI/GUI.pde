import controlP5.*;
import processing.serial.*;

Serial port;

ControlP5 cp5;
CallbackListener cb;
Slider cSlider, zSlider;
ScrollableList portSelector;
Textlabel info;

PFont font;
PImage curveImg;
PGraphics pg;


void setup()
{
    size(360, 560);
    curveImg = createImage(200 , 200, ARGB);
    pg = createGraphics(200,200);
    
    if (Serial.list().length > 1)
        port = new Serial(this, Serial.list()[1], 9600);
    else
        port = new Serial(this, Serial.list()[0], 9600);
        
        
    cp5 = new ControlP5(this);
    font = createFont("Futura", 11);
  
    info = cp5.addTextlabel("label")
      .setText("Info")
      .setPosition(30,height - 30)
      .setColorValue(color(255,255,255))
      .setFont(font)
      ;
  
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
    .setRange(0, 1023)
    .setValue(0)
    .setPosition(40, curveImg.height + 200)
    .setSize(200,20)
    .setTriggerEvent(Slider.PRESS)
    .setLabel("Curvature")
    .setSliderMode(Slider.FIX)
    ;
    
  // add callback to only send data when released  
  cSlider.addCallback(new CallbackListener() 
     {
      public void controlEvent(CallbackEvent theEvent) {
        if (   theEvent.getAction()==ControlP5.ACTION_RELEASE 
            || theEvent.getAction()==ControlP5.ACTION_RELEASE_OUTSIDE
            || theEvent.getAction()==ControlP5.ACTION_WHEEL
            ) 
        {
          createAndSendCommand(1023 - (int)cSlider.getValue(), "S");
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
    
  zSlider.addCallback(new CallbackListener() 
     {
      public void controlEvent(CallbackEvent theEvent) {
        if (   theEvent.getAction()==ControlP5.ACTION_RELEASE 
            || theEvent.getAction()==ControlP5.ACTION_RELEASE_OUTSIDE
            || theEvent.getAction()==ControlP5.ACTION_WHEEL
            ) 
        {
          createAndSendCommand((int)zSlider.getValue(), "Z");
        }
      }
    }
  );
  
}


void setPort(int num)
{
    //port = null;
    port.clear();
    port.stop();
    
    port = new Serial(this, Serial.list()[num], 9600);
}

void draw()
{
  background(0);
  
  getSerialCommand();
  parseCommand();

  drawGraph();
}


String commandString = "";
boolean inProgress = false;
ArrayList<String> commands = new ArrayList<String>(0);

String temp;
void getSerialCommand()
{
   
    char beginMarker = '[';
    char endMarker = ']';
    char inChar;
    String tmp = "";
  
    
    while (port.available() > 0 ) {
      inChar = port.readChar();
      
      if (inProgress == true)
      {
          if (inChar == endMarker)
          {  
              inProgress = false;
              commands.add(tmp);
              tmp = "";
          }
          else
          {
              tmp += inChar;
          }
        
      }
      else if (inChar == beginMarker)
      {
         inProgress = true;
      }
      
    }
}


void getSerialNormal()
{
  while (port.available() > 0) {
    String inBuffer = port.readString();   
    
    if (inBuffer.charAt(0) != '[')
        info.setText(inBuffer);
   
  }
}

void parseCommand()
{
   if (commands.size() > 0)
   {
     
     for ( String cmd: commands)
     {
        if (cmd.charAt(0) == 'S')
        {
           int val = Integer.parseInt(cmd.substring(2));
           cSlider.setValue( 1023 - val );
        }
        else if (cmd.charAt(0) == 'Z')
        {
           int val = Integer.parseInt(cmd.substring(2));
           zSlider.setValue( val );
        }
     }
     
     commands.clear();
      
   }
}

void fillGraphicsForGraph(PGraphics gr, float skew, float resolution)
{
    float x = 0;
    float y_old = gr.height;
    
    int division = round(gr.width / resolution);
    float stride = 1024 / (gr.width / (float)division);
    
    
    
    gr.beginDraw();
    gr.background(12);
    for (int i = 0; i <= gr.width; i += division)
    {
       gr.stroke(255);
       gr.strokeWeight(1);
       float y = 0;
       y = getSkewedValue((int)x, skew);
       y = map(y, 0, 1024,  gr.height, 0); //invert image
       
       gr.line(i - division, y_old, i, y);
   
       y_old = y;
       
       x += stride;
    }
    
    
    gr.endDraw();
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
        y = round(map(y, 0, 1023, 0 , img.height));
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


void drawGraph()
{
    //image(img, 40, 120);
    image(pg, 40, 120);
}

int getSkewedValue(int value, float skew)
{
    float factor = 1024;
    float norm = (float)value / factor;
    float skewed = pow( norm, 2.0 - skew );

    return round(skewed * factor);
}

void curveSlider(int value)
{
    // update image
    float skewFactor = (1023 - (float)value) / 1024;
    //fillImage(curveImg, skewFactor);
    fillGraphicsForGraph(pg,  skewFactor, 64);
}


void deadzoneSlider(int value)
{
     //createAndSendCommand(value, "Z");
}

void portSelect(int n) 
{
    setPort( n );
}


void createAndSendCommand(int value, String name)
{
    String message = "<";
    message += name;
    message += ",";
    message += str(value);
    message += ">";
    
    if (port != null)
      port.write(message);
}
