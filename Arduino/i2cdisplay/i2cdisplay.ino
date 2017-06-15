#include "Wire.h"
byte inputs=0;
void setup()
{
 Serial.begin(9600);
 Wire.begin(); // wake up I2C bus
}
void loop()
{
 Wire.beginTransmission(0x20);
 Wire.write(0x12); // set MCP23017 memory pointer to GPIOB address
 Wire.endTransmission();
 Wire.requestFrom(0x20, 1); // request one byte of data from MCP20317
 inputs=Wire.read(); // store the incoming byte into "inputs"
 //if (inputs>0) // if a button was pressed
 //{
 Serial.println(inputs, BIN); // display the contents of the GPIOB register in binary
 delay(20); // for debounce
// }
}
