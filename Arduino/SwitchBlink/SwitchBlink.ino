
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
