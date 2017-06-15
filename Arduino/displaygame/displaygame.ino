/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
int row = 0;
int col = 0;
int rowd = 0;
int cold = 0;
unsigned long lastshow = 0;
bool shot = false;

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
 lcd.setBacklight(WHITE);
}

uint8_t i=0;
void loop() {
  if(millis() >= lastshow+100 & rowd!= 0 || cold!= 0 || shot){
    lastshow = millis();
    Serial.println(millis());
  byte mychar[8] = {
    B10000,
    B11000,
    B11111,
    B11111,
    B11000,
    B10000,
  };

  

  row = row+rowd;
  if(row<0){row=0;}
  if(row>1){row=1;}
  col = col+cold;
  if(col<0){col=0;}
  if(col>15){col=15;}

  rowd=0;
  cold=0;
  
  lcd.clear();
  lcd.createChar(0,mychar);
  lcd.setCursor(col, row);
  lcd.write((byte)0);

   if(shot){
      byte bul[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B00000,
    B00000,
  };
    lcd.createChar(1,bul);
    lcd.setCursor(col+1,row);
    lcd.write((byte)1);
    shot = false;
   }

  
  Serial.println(row);
  Serial.println(col);
  
  }
  
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    }
    if (buttons & BUTTON_DOWN) {
      rowd = 1;
    }
    if (buttons & BUTTON_LEFT) {
      cold = -1;
    }
    if (buttons & BUTTON_RIGHT) {
      cold = 1;
    }
    if (buttons & BUTTON_UP) {
      rowd = -1;
    }
    if (buttons & BUTTON_SELECT){
      shot = true;
    }
    delay(50);
}

