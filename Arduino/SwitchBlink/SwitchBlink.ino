/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(5, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(11, HIGH); //LOAD ON
  delay(2000);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(11, LOW); //LOAD OFF
  delay(100);
  digitalWrite(13, HIGH); //LEAD OFF
  delay(10);
  digitalWrite(12,HIGH); // LI ON
  delay(100);
  digitalWrite(11, HIGH); //LOAD ON
  delay(5000);                       // wait for a second
  digitalWrite(5, LOW);// turn the LED off by making the voltage LOW
  digitalWrite(11, LOW); //LOAD OFF
  delay(100);
  digitalWrite(12, LOW); // LI OFF
  delay(10);
  digitalWrite(13, LOW); // LEAD ON
  delay(100);
  digitalWrite(11, HIGH); //LOAD ON
  delay(5000);                       // wait for a second
}
