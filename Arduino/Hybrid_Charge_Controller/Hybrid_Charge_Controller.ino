//--------DEBUG---------------------
const int PRINT_INTERVAL = 500;
unsigned long led_lighttime = 1000; //turn on led for 1 sec on startup

//--------protection ratings------------
//LI handeled by balancer
const double LA_I_MAX_CHARGE = 5.0;	//5A 
const double LA_I_MAX_DISCHARGE = 14.0; //14A
const double LD_I_MAX = 14.0; //14A
const double PV_I_MAX = 5.0; //5A

const double LA_V_MAX = 16.0;	//16V absolute maximum 
const double PV_V_MAX = 25.0; //25V if full irradiation and not charging

//--------calibration values--------------
const double LA_V_FACTOR  = 0.001;
const double LA_V_SUMMAND = 0;

const double LA_I_FACTOR  = 0.001;
const double LA_I_SUMMAND = 0;

const double PV_V_FACTOR  = 0.001;
const double PV_V_SUMMAND = 0;

const double PV_I_FACTOR  = 0.001;
const double PV_I_SUMMAND = 0;

const double LD_I_FACTOR  = 0.001;
const double LD_I_SUMMAND = 0;

const double TEMPERATURE_FACTOR  = 1;
const double TEMPERATURE_SUMMAND = 0;

//--------charge protcoll values----------
const double V_ABS = 14.65; //absorption voltage
const double V_EQU = 15.5; //equalization voltage
const double V_FLT = 13.25; //float voltage
const double LA_I_MIN_ABSORP = 0.001; // C-rate/100
double la_soc = 100;
double li_soc = 100;

//---------LA state definitions-------------
const byte LA_IDLE = 0;
const byte LA_DISCHARGE = 1;
const byte LA_LOWDISCON = 2;
const byte LA_BULK = 3;
const byte LA_ABSORP = 4;
const byte LA_EQUALIZE = 5;
const byte LA_FLOAT = 6;

//---------LI state definitions-------------
const byte LI_IDLE = 0;
const byte LI_DISCHARGE = 1;
const byte LI_LOWDISCON = 2;
const byte LI_CHARGE = 3;

//---------LD & PV state definitions-------------
const bool LD_OFF = 0;
const bool LD_ON = 1;

const bool PV_OFF = 0;
const bool PV_ON = 1;


//---------System state definitions-------------
const byte SYS_IDLE = 0;
const byte SYS_CHARGE = 1;
const byte SYS_DISCHARGE = 2;

const double SYS_IDLE_THRESHOLD = 0.002; //+-20mA don't count as charge or discharge THIS CON NOT BE SMALLER THAN LA_I_MIN_ABSORP

//---------state variables-------------
byte la_state = 0;
byte li_state = 0;
bool pv_state = 0;
bool ld_state = 0;
byte sys_state = 0;

//-----------switching parameters---------
const double SWITCH_THRESHOLD = 0.5; //0.5A discharge current

const bool LA_BATTERY = 0;
const bool LI_BATTERY = 1;

bool current_battery = 0; //LA
bool next_battery = 0; //LA

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

//-----------PWM PI controller values-----------
const double V_KP = 0.05; //p-factor
const double V_KI = 0.05; //i-factor
const double V_DMIN = 0;	   	//
const double V_DO = 0.5; 	//

const double I_KP = 0.05; //p-factor
const double I_KI = 0.05; //i-factor
const double I_DMIN = 0;	//
const double I_DO = 0.5; 	//

double V_d = 0;
double V_sum_e = 0;
double I_d = 0;
double I_sum_e = 0;
double duty_cycle = 0;

//-----------------measurement------------------
const double SYS_MEASUREMENT_INACCURACY = 0.01; //1mA is acceptable
const int NR_SAMPLES = 100;			//for measureing analog values
double la_v = 0;
double la_i = 0;
double li_v = 0;
double li_i = 0;
double pv_v = 0;
double pv_i = 0;
double ld_i = 0;
double temperature = 0;


//-----------------other------------------
bool multiplex_state_a = 0;
bool multiplex_state_b = 0;
bool multiplex_state_c = 0;
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
	measure();            	 	// read all sensor data
	protection();				//software protection for all devices
	calculate_SOCs();			// SOCs for both batteries
	set_sys_state();			// dis-/charging/idle
	select_battery();       	// LA or LI
	switchto_battery();			// LA or LI
	charge_protocol();      	// pwm charging of LA
	load_control();				// load on/off
	print_data();       	    // print to serial monitor
	led_handler();				// turn led on or off
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

void protection(){
	//LA battery protection
	//overvoltage
	if(la_v>LA_V_MAX){
		la_overvoltage();
	}
	//overdischarge
	if(la_i>LA_I_MAX_CHARGE){
		la_overcurrent();
	}
	//overcharge
	if(la_i<LA_I_MAX_DISCHARGE*(-1)){
		la_overcurrent();
	}

	//LD protection
	if(ld_i>LD_I_MAX){
		ld_overcurrent();
	}

	//PV protection
	if(pv_v>PV_V_MAX){
		pv_overvoltage();
	}
	if(pv_i>PV_I_MAX){
		pv_overcurrent();
	}

	//check if "current is lost somewhere"
	double measure_mistake = -pv_i+la_i+li_i+ld_i;
	if(abs(measure_mistake) > SYS_MEASUREMENT_INACCURACY){
		measure_error(measure_mistake);
	}
}

void la_overvoltage(){
	next_battery = LI_BATTERY;
	switchto_battery();
	ld_state = 0;
	load_control();
	pv_state = 0;
	pv_control();
	led_set(100000);
	Serial.println(F("LA OVERVOLTAGE... STOPPING"));
	Serial.println(la_v);
	while(true);
}

void la_overcurrent(){
	next_battery = LI_BATTERY;
	switchto_battery();
	ld_state = 0;
	load_control();
	pv_state = 0;
	pv_control();
	led_set(100000);
	Serial.println(F("LA OVERCURRENT... STOPPING"));
	Serial.println(la_i);
	while(true);
}

void ld_overcurrent(){
	ld_state = 0;
	load_control();
	pv_state = 0;
	pv_control();
	next_battery = LA_BATTERY; //can handle higher loads, thats why switching
	switchto_battery(); 
	led_set(100000);
	Serial.println(F("LOAD OVERCURRENT... STOPPING"));
	Serial.println(ld_i);
	while(true);
}

void pv_overvoltage(){
	pv_state = 0;
	pv_control();
	ld_state = 0;
	load_control();
	next_battery = LA_BATTERY; //can handle higher loads, thats why switching
	switchto_battery();
	led_set(100000);
	Serial.println(F("PV OVERVOLTAGE... STOPPING"));
	Serial.println(pv_v);
	while(true);
}

void pv_overcurrent(){
	pv_state = 0;
	pv_control();
	ld_state = 0;
	load_control();
	next_battery = LA_BATTERY; //can handle higher loads, thats why switching
	switchto_battery();
	led_set(100000);
	Serial.println(F("PV OVERCURRENT... STOPPING"));
	Serial.println(pv_i);
	while(true);
}

void measure_error(double mistake){
	pv_state = 0;
	pv_control();
	ld_state = 0;
	load_control();
	next_battery = LA_BATTERY; //can handle higher loads, thats why switching
	switchto_battery();
	led_set(100000);
	Serial.println(F("MEASURE ERROR"));
	Serial.println(mistake);
	while(true);
}

void calculate_SOCs(){
	//table
}

void set_sys_state(){
	//compare current pv and ld plus threshold
	if(pv_i*(-1) >= ld_i + SYS_IDLE_THRESHOLD){ 
		sys_state = SYS_CHARGE;
	}else if(pv_i*(-1) <= ld_i - SYS_IDLE_THRESHOLD){
		sys_state = SYS_DISCHARGE;
	}else{
		sys_state = SYS_IDLE;
	}
}

void select_battery(){
	switch(sys_state){
		case SYS_IDLE:
			next_battery = LI_BATTERY;
		break;
		case SYS_CHARGE:
			if(li_soc < 100){ //only if LI is full LA gets charged
				next_battery = LI_BATTERY;
			}else{
				next_battery = LA_BATTERY;
			}
		break;
		case SYS_DISCHARGE:
			if(la_soc > 60){//if la has enough
				if(ld_i > pv_i*(-1)+SWITCH_THRESHOLD){ //if more then 0.5A dischare current
					next_battery = LA_BATTERY;
				}else{	//less then 0.5A discharge
					if(li_soc>60){ //if LI has enough
						next_battery = LI_BATTERY;
					}else{ //if not
						next_battery = LA_BATTERY;
					}
				}
			}else{//if la has not enough
				next_battery = LI_BATTERY;
			}
		break;
	}
}

void switchto_battery(){
	if(current_battery != next_battery){ //only if switch is needed
		switch(next_battery){
			case LA_BATTERY:
				digitalWrite(LD_SW_PIN, LOW); 	//turn LOAD off
				digitalWrite(LED_BTN_PIN, LOW); //turn LED off
				delay(100);
				//LI_SW_OFF();					//turn LI off
				delay(10);
				digitalWrite(LA_SW_PIN, LOW); 	//turn LA on
				delay(100);
				if(ld_state == LD_ON){
					digitalWrite(LD_SW_PIN, HIGH); 	//turn LOAD on if its supposed to be on
				}
				li_state = LI_IDLE; 
			break;
			case LI_BATTERY: 
				digitalWrite(LD_SW_PIN, LOW); //turn LOAD off
				digitalWrite(LED_BTN_PIN, LOW); //turn LED off
				delay(100);
				digitalWrite(LA_SW_PIN, HIGH); //turn LA off
				delay(10);
				//LI_SW_ON();						//turn LI on
				delay(100);
				if(ld_state == LD_ON){
					digitalWrite(LD_SW_PIN, HIGH); 	//turn LOAD on if its supposed to be on
				}
				la_state = LA_IDLE;
			break;
		}
		current_battery = next_battery;
	}
}

void charge_protocol(){
	//only execute if LA is charging 
	if(sys_state == SYS_CHARGE && current_battery == LA_BATTERY){
		select_chargemode();
		execute_chargemode();
	}
}

void select_chargemode(){
	if(la_v <= V_ABS){
		la_state = LA_BULK;
	}else{
		if(la_state == LA_EQUALIZE){
			if(la_i<=LA_I_MIN_ABSORP){
				la_soc = 100;
				la_state = LA_FLOAT;
			}else{
				la_state = LA_EQUALIZE;
			}
		}else if(la_state == LA_ABSORP){
			if(la_i<=LA_I_MIN_ABSORP){
				la_soc = 100;
				la_state = LA_FLOAT;
			}else{
				la_state = LA_ABSORP;
			}
		}else{
			if(equalize_due()){
				la_state = LA_EQUALIZE;
			}else{
				la_state = LA_ABSORP;
			}
		}
	}
}

bool equalize_due(){
	//timemanagement needed
	return 0; //not due
}

void execute_chargemode(){
	switch(la_state){
		case LA_BULK:
			bulkmode();
		break;
		case LA_ABSORP:
			absorpmode();
		break;
		case LA_FLOAT:
			floatmode();
		break;
		case LA_EQUALIZE:
			equalizemode();
		break;
	}
}

void bulkmode(){
	//la current up to LA_I_MAX_CHARGE with PI controller
	double d;
	d = pi_controller(LA_I_MAX_CHARGE, la_i, I_KP, I_KI, I_sum_e, I_DMIN, I_DO, I_d);
	duty_cycle = d *255;
}

void absorpmode(){
	//la voltage limited to hold at V_ABS with PI controller
	double d;
	d = pi_controller(V_ABS, la_v, V_KP, V_KI, V_sum_e, V_DMIN, V_DO, V_d);
	duty_cycle = d*255;
}

void floatmode(){
	//la voltage limited to hold at V_FLT with PI controller
	double d;
	d = pi_controller(V_FLT, la_v, V_KP, V_KI, V_sum_e, V_DMIN, V_DO, V_d);
	duty_cycle = d*255;
}

void equalizemode(){
	//la voltage limited to hold at V_EQU with PI controller
	double d;
	d = pi_controller(V_EQU, la_v, V_KP, V_KI, V_sum_e, V_DMIN, V_DO, V_d);
	duty_cycle = d*255;
}

void load_control(){
	if(ld_state==LD_ON){
		digitalWrite(LD_SW_PIN, HIGH);
	}else{
		digitalWrite(LD_SW_PIN, LOW);
	}
}

void pv_control(){
	//only allow pv switch to open, if voltage is high enough
	if(pv_v > active_bat_v()){
		analogWrite(PV_SW_PIN,duty_cycle);
	}
}

double active_bat_v(){
	if(current_battery == LA_BATTERY){
		return la_v;
	}else{
		return li_v;
	}
}

double pi_controller(double setpoint, double process_variable, double kp, double ki, double sum_e, double dmin, double d_o, double d){
	//d und sum_e sollten für die nächste runde gespeichert werden...
	double e = setpoint - process_variable;
	if(d<1){
		sum_e = sum_e + e;
	}
	d=d_o + kp*e + ki*sum_e;
	if(d>=1){
		d = 1;
	}
	if(d<dmin){
		d = dmin;
	}
	return d;
}

void led_handler(){
	//if led_lighttime is not reached, turn in led
	if(millis() < led_lighttime){
		digitalWrite(LED_BTN_PIN, HIGH);
	}else{
		digitalWrite(LED_BTN_PIN, LOW);
	}
}

void led_set(unsigned long shinetime){
	//set new time to turn off led
	led_lighttime = millis() + shinetime;
	led_handler();
}

void print_data(){
	if(millis()> lastprint + PRINT_INTERVAL){ //only print if last print was PRINT_INTERVAL millisec ago
		lastprint = millis();
		//for easy distinction of blocks
		Serial.println();
		Serial.println();
		Serial.println();
		Serial.println();
		Serial.println();
		Serial.println();
		//state--------------------------
		Serial.println(F("----------------------STATES"));
		Serial.print(F("Time: "));
		Serial.println(millis());
		Serial.print(F("current Battery: "));
		Serial.println(current_battery);
		Serial.print(F("sys_state: "));
		Serial.println(sys_state);
		Serial.print(F("la_state: "));
		Serial.println(la_state);
		Serial.print(F("li_state: "));
		Serial.println(li_state);
		Serial.print(F("pv_state: "));
		Serial.println(pv_state);
		Serial.print(F("ld_state: "));
		Serial.println(ld_state);

		//measured data-------------------
		Serial.println(F("----------------------MEASUREMENTS"));
		Serial.print(F("la_v: "));
		Serial.println(la_v);
		Serial.print(F("la_i: "));
		Serial.println(la_i);
		Serial.print(F("pv_v: "));
		Serial.println(pv_v);
		Serial.print(F("pv_i: "));
		Serial.println(pv_i);
		Serial.print(F("ld_i: "));
		Serial.println(ld_i);

		//other
		Serial.println(F("----------------------OTHER"));
		Serial.print(F("duty_cycle: "));
		Serial.println(duty_cycle);
	}
}        
