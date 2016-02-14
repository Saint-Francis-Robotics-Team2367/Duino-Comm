//This example implements a simple sliding On/Off button. The example
// demonstrates drawing and touch operations.
//
//Thanks to Adafruit forums member Asteroid for the original sketch!
//
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

#define in_range(x,a,b) (x > a && x < b)
#define LEFT_BUTT_END_LIM width / 2 - 10
#define RIGHT_BUTT_START_LIM width / 2 + 5
#define BUTTON_WIDTH width / 2 - 5
#define BUTTON_HEIGHT 35


#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
#define TFT_CS 10
#define TFT_DC 9
int width, height;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


char lastButton;
void setup(void)
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);

  Serial.begin(9600);
  tft.begin();
  if (!ts.begin()) {
    Serial.println("Unable to start touchscreen.");
  }
  else {
    Serial.println("Touchscreen started.");
  }
  height = tft.width();//needed for proper screen rotation
  width  = tft.height();
  tft.fillScreen(ILI9341_BLACK);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(1);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(3);
  tft.println("  FRC TEAM 2367");
  tft.drawFastHLine(0, 30, width, ILI9341_WHITE);
  tft.setCursor(0, 35);
  tft.print("Battery: ");
  tft.setCursor(0, 60);
  tft.print("Comms: ");
  tft.drawFastHLine(0, 85, width, ILI9341_WHITE);

  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);

  tft.setCursor(5, 100);
  tft.print("Low Gl");
  tft.setCursor(width / 2 + 10, 100);
  tft.print("Rock Wl");
  tft.setCursor(5, 145);
  tft.print("Moat");
  tft.setCursor(width / 2 + 10, 145);
  tft.print("Ruf Terr");
  tft.setCursor(5, 190);
  tft.print("Ramparts");
  tft.setCursor(width / 2 + 10, 190);
  tft.print("Nothing");
  tft.setCursor(5, 205);

  tft.drawRect(0, 95,BUTTON_WIDTH, BUTTON_HEIGHT, ILI9341_RED);
  tft.drawRect(RIGHT_BUTT_START_LIM, 95, BUTTON_WIDTH, BUTTON_HEIGHT, ILI9341_RED);

  tft.drawRect(0, 140, BUTTON_WIDTH, BUTTON_HEIGHT, ILI9341_RED);
  tft.drawRect(RIGHT_BUTT_START_LIM, 140, BUTTON_WIDTH, BUTTON_HEIGHT, ILI9341_RED);

  tft.drawRect(0, 185, BUTTON_WIDTH, BUTTON_HEIGHT, ILI9341_RED);
  tft.drawRect(RIGHT_BUTT_START_LIM, 185, BUTTON_WIDTH, BUTTON_HEIGHT, ILI9341_RED);
}
void loop()
{
  if (!ts.bufferEmpty())
  {
    TS_Point pt = ts.getPoint();
    pt.x = map(pt.x, TS_MINY, TS_MAXY, 0, tft.height());
    pt.y = map(pt.y, TS_MINX, TS_MAXX, 0, tft.width());
    int y = tft.height() - pt.x;
    int x = pt.y;
    if (in_range(x, 0, LEFT_BUTT_END_LIM) && in_range(y, 95, 95+BUTTON_HEIGHT))
    {
      lastButton = '1';
      Serial.println("B1");
    }
    else if (in_range(x, 0, BUTTON_WIDTH) && in_range(y, 140, 140+BUTTON_HEIGHT))
    {
      lastButton = '2';
      Serial.println("B2");
    }
    else if (in_range(x, 0, BUTTON_WIDTH) && in_range(y, 185, 185+BUTTON_HEIGHT))
    {
      lastButton = '3';
      Serial.println("B3");
    }
    else if (in_range(x, RIGHT_BUTT_START_LIM,RIGHT_BUTT_START_LIM+ BUTTON_WIDTH) && in_range(y, 95, 95+BUTTON_HEIGHT))
    {
      lastButton = '4';
      Serial.println("B4");
    }
    else if (in_range(x, RIGHT_BUTT_START_LIM,RIGHT_BUTT_START_LIM+ BUTTON_WIDTH) && in_range(y, 140, 140+BUTTON_HEIGHT))
    {
      lastButton = '5';
      Serial.println("B5");
    }
    else if (in_range(x, RIGHT_BUTT_START_LIM,RIGHT_BUTT_START_LIM+ BUTTON_WIDTH) && in_range(y, 185, 185+BUTTON_HEIGHT))
    {
      lastButton = '6';
      Serial.println("B6");
    }
    
  }
  delay(500);
}

void updateBattery(String volts)
{
  tft.setTextColor(ILI9341_GREEN);
  tft.fillRect(0, 31, width, 26, ILI9341_BLACK);
  tft.drawFastHLine(0, 30, width, ILI9341_WHITE);
  tft.setCursor(0, 35);
  tft.print("Battery: "); tft.print(volts);
}
void updateComms(String stats)
{
  Serial.println("comms");
  tft.setTextColor(ILI9341_GREEN);
  tft.fillRect(0, 59, width, 26, ILI9341_BLACK);
  tft.drawFastHLine(0, 85, width, ILI9341_WHITE);
  tft.setCursor(0, 60);
  tft.print("Comms: "); tft.print(stats);
}
void receiveEvent(int howMany) {
  String temp = "";
  int byteNum = 8;
  char text[byteNum + 1];
  char blank[byteNum + 1];
  int i = 0;
  while (Wire.available() && (i < (byteNum - 1))) {
    char c = Wire.read();
    if (c != '\0') {
      text[i] = c;
      text[i + 1] = '\0';
      i++;
    }
  }
  String data = String(text);
  data = data.substring(1, sizeof(text) - 2);
  if(text[0] == 'B')
  {
    updateBattery(data);
  }
  if(text[0] == 'C')
  {
    updateComms(data);
  }
  memset(text, 0, byteNum + 1);
}
void requestEvent()
{
  Serial.println("request");
  Wire.write(lastButton);
}

