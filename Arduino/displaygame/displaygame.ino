
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#define WHITE 0x7
int row = 0;
int col = 0;
int rowd = 0;
int cold = 0;
int srow = 0;
int scol = 0;
int scold = 1;
int erow = 0;
int ecol = 0;
int ecold = -1;
unsigned long lastshow = 0;
unsigned long lastenemy = 0;
bool shot = false;
bool enemy = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
}

uint8_t i=0;
void loop() {
  //if(millis() >= lastshow+100 & rowd!= 0 || cold!= 0 || shot){
   if(millis() >= lastenemy+4000 & enemy == false){
      enemy = true;
      ecol = 16;
      erow = 1;
    };
  if(millis() >= lastshow+400){
     lastshow = millis();
     lcd.clear();

   if(enemy){
   byte ene[8] = {
    B10001,
    B01010,
    B11100,
    B11100,
    B01010,
    B10001,
  };
  ecol = ecol+ecold;
  if(ecol<0){
    enemy = false;
    }
  if(scol>15){ecol = 15;}
  
  
    lcd.createChar(2,ene);
    lcd.setCursor(ecol,erow);
    lcd.write((byte)2);
   }
   
  
  byte ship[8] = {
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
  
  scol = scol+scold;
  if(scol<0){scol=0;}
  if(scol>15){
    shot = false;
    }
  

  lcd.createChar(0,ship);
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
    lcd.setCursor(scol,srow);
    lcd.write((byte)1);
   }

  
  //Serial.println(row);
  //Serial.println(col);
  
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
      scol = col;
      srow = row;
    }
    delay(50);
}

