//--------DEBUG---------------------
const int PRINT_INTERVAL = 1000;
unsigned long led_lighttime = 1000; //turn on led for 1 sec on startup
const int NR_SAMPLES = 100;			//for measureing analog values
//--------calibration values--------------
const double LA_V_FACTOR  = 1;
const double LA_V_SUMMAND = 0;

const double LA_I_FACTOR  = 1;
const double LA_I_SUMMAND = 0;

const double PV_V_FACTOR  = 1;
const double PV_V_SUMMAND = 0;

const double PV_I_FACTOR  = 1;
const double PV_I_SUMMAND = 0;

const double LD_I_FACTOR  = 1;
const double LD_I_SUMMAND = 0;

const double TEMPERATURE_FACTOR  = 1;
const double TEMPERATURE_SUMMAND = 0;
//--------charge protcoll values----------

//---------status definitions-------------
const byte LA_IDLE = 0;
const byte LA_DISCHARGE = 1;
const byte LA_LOWDISCON = 2;
const byte LA_BULK = 3;
const byte LA_ABSORB = 4;
const byte LA_EQUAL = 5;
const byte LA_FLOAT = 6;

const byte LI_IDLE = 0;
const byte LI_DISCHARGE = 1;
const byte LI_LOWDISCON = 2;
const byte LI_CHARGE = 3;

byte LA_STATUS = 0;
byte LI_STATUS = 1;
//-----------switching parameters---------
const double SWITCH_LD_I_THRESHHOLD = 0.5;

//-----------------pins-------------------
const byte LA_SW_PIN = 6; //LOW=closed
const byte PV_SW_PIN = 9; //HIGH=closed
const byte LD_SW_PIN = 7; //HIGH=closed
const byte MULTIPLEX_PIN_A = 14; //A0
const byte MULTIPLEX_PIN_B = 15; //A1
const byte MULTIPLEX_PIN_C = 16; //A2
const byte MULTIPLEX_PIN_OUT = 17; //A3
const byte LED_BTN_PIN = 5; //led and button pin
const byte TEMPERATURE_PIN = 21; //A7
//-----------------other------------------
double la_v = 0;
double la_i = 0;
double li_v = 0;
double li_i = 0;
double pv_v = 0;
double pv_i = 0;
double ld_i = 0;
double temperature = 0;

bool multiplex_state_a = 0;
bool multiplex_state_b = 0;
bool multiplex_state_c = 0;


bool selected_battery = 0; //LA
unsigned long lastprint = 0;

void setup() {
	//pinModes-----------------
	pinMode(MULTIPLEX_PIN_A, OUTPUT);
	pinMode(MULTIPLEX_PIN_B, OUTPUT);
	pinMode(MULTIPLEX_PIN_C, OUTPUT);
	pinMode(MULTIPLEX_PIN_OUT, INPUT);
	pinMode(LA_SW_PIN, OUTPUT);
	pinMode(PV_SW_PIN, OUTPUT);
	pinMode(LD_SW_PIN, OUTPUT);
	//initial pin writing----------
	digitalWrite(LA_SW_PIN, LOW); //LA on
	digitalWrite(PV_SW_PIN, LOW); //PV off
	digitalWrite(LD_SW_PIN, LOW); //LD off
	//miscelaneous--------------
	Serial.begin(115200);

}

void loop() {
	measure();              // read all sensor data
	select_battery();       // LA or LI
	charge_protocol();      // pwm charging of battery
	load_control();         // load on/off
	print_data();           // print to serial monitor
	led_handler();			// turn led on or off
	//button_checker();		// check if button is pressed
}

void measure(){
	la_v = get_value(1);
	la_i = get_value(2);
	pv_v = get_value(3);
	pv_i = get_value(4);
	ld_i = get_value(5);
	temperature = get_temperature();
}

double get_value(byte measurepoint){
	set_multiplexchannel(measurepoint);
	delay(1);	//allow multiplexer to switch channels
	double temp_average_value = read_average_value();
	double temp_transformed_value = transform_average_value(temp_average_value, measurepoint);
	return temp_transformed_value;
}

void set_multiplexchannel(byte measurepoint){
	switch (measurepoint){
		case 1:	//la_v
			multiplex_state_a = 1;
			multiplex_state_b = 0;
			multiplex_state_c = 1;
			break;
		case 2:	//la_i
			multiplex_state_a = 1;
			multiplex_state_b = 0;
			multiplex_state_c = 0;
			break;
		case 3:	//pv_v
			multiplex_state_a = 0;
			multiplex_state_b = 1;
			multiplex_state_c = 0;
			break;
		case 4:	//pv_i
			multiplex_state_a = 1;
			multiplex_state_b = 1;
			multiplex_state_c = 0;
			break;
		case 5:	//ld_i
			multiplex_state_a = 0;
			multiplex_state_b = 0;
			multiplex_state_c = 1;
			break;
	}
	digitalWrite(MULTIPLEX_PIN_A, multiplex_state_a);
	digitalWrite(MULTIPLEX_PIN_B, multiplex_state_b);
	digitalWrite(MULTIPLEX_PIN_C, multiplex_state_c);
}

double read_average_value(){
	//take one value every millisec and summ it up, divide it
	double value = 0;
    for (byte i = 0; i < NR_SAMPLES; i++) {
    	value = value + analogRead(MULTIPLEX_PIN_OUT);
    	delayMicroseconds(50);
  	}
  	value = value / NR_SAMPLES;
 	return value;
}
double transform_average_value(double temp_average_value, byte measurepoint){
	double value_factor = 0;
	double value_summand = 0;
	switch (measurepoint){
		case 1:	//la_v
			value_factor = LA_V_FACTOR;
			value_summand = LA_V_SUMMAND;
			break;
		case 2:	//la_i
			value_factor = LA_I_FACTOR;
			value_summand = LA_I_SUMMAND;
			break;
		case 3:	//pv_v
			value_factor = PV_V_FACTOR;
			value_summand = PV_V_SUMMAND;
			break;
		case 4:	//pv_i
			value_factor = PV_I_FACTOR;
			value_summand = PV_I_SUMMAND;
			break;
		case 5:	//ld_i
			value_factor = LD_I_FACTOR;
			value_summand = LD_I_SUMMAND;
			break;
	}
	double transformed_value = (temp_average_value * value_factor) + value_summand;
	return transformed_value;
}

double get_temperature(){
	//take one value every millisec and summ it up, divide it
	double temp_value = 0;
    for (byte i = 0; i < NR_SAMPLES; i++) {
    	temp_value = temp_value + analogRead(TEMPERATURE_PIN);
    	delayMicroseconds(50);
  	}
  	temp_value = temp_value / NR_SAMPLES;
  	temp_value = TEMPERATURE_FACTOR * temp_value + TEMPERATURE_SUMMAND;
 	return temp_value;
}

void select_battery(){
	bool new_battery = 0; //LA

	if(selected_battery != new_battery){ //only if switch is needed
		switchto_battery(new_battery);
		
	}
}

void switchto_battery(bool new_battery){
	switch(new_battery){
		case 0: //LA
			digitalWrite(LD_SW_PIN, LOW); 	//turn LOAD off
			digitalWrite(LED_BTN_PIN, LOW); //turn LED off
			delay(100);
			//LI_SW_OFF();					//turn LI off
			delay(10);
			digitalWrite(LA_SW_PIN, LOW); 	//turn LA on
			delay(100);
			digitalWrite(LD_SW_PIN, HIGH); 	//turn LOAD on
			break;
		case 1: //LI
			digitalWrite(LD_SW_PIN, LOW); //turn LOAD off
			digitalWrite(LED_BTN_PIN, LOW); //turn LED off
			delay(100);
			digitalWrite(LA_SW_PIN, HIGH); //turn LA off
			delay(10);
			//LI_SW_ON();						//turn LI on
			delay(100);
			digitalWrite(LD_SW_PIN, HIGH); //turn LOAD on
			break;
	}
	selected_battery = new_battery;
}

void charge_protocol(){
	// pwm charging of battery
}

void load_control(){
	// load on/off
}

void led_handler(){
	//if led_lighttime is not reached, turn in led
	if(millis() < led_lighttime){
		digitalWrite(LED_BTN_PIN, HIGH);
	}else{
		digitalWrite(LED_BTN_PIN, LOW);
	}
}

void led_set(int shinetime){
	//set new time to turn off led
	led_lighttime = millis() + shinetime;
}

void print_data(){
	if(millis()> lastprint + PRINT_INTERVAL){ //only print if last print was PRINT_INTERVAL millisec ago
		lastprint = millis();
		//status--------------------------
		Serial.print(F("Selected Battery(0=LA,1=LI): "));
		Serial.println(selected_battery);
		Serial.print(F("LA_STATUS: "));
		Serial.println(LA_STATUS);
		Serial.print(F("LI_STATUS: "));
		Serial.println(LI_STATUS);
		//measured data-------------------
		Serial.print(F("LA_V: "));
		Serial.println(la_v);
		Serial.print(F("LA_I: "));
		Serial.println(la_i);
		Serial.print(F("PV_V: "));
		Serial.println(pv_v);
		Serial.print(F("PV_I: "));
		Serial.println(pv_i);
		Serial.print(F("LD_V: "));
		Serial.println(ld_i);
		Serial.println(F("--------------"));
	}
}        
