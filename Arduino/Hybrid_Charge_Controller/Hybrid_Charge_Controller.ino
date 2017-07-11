//--------INCLUDES--------------
#include <bq769x0.h>    // Library for Texas Instruments bq76920 battery management IC

//--------FOR TESTING--------------
unsigned long lastswitch = 0;

//--------DEBUG---------------------
const int PRINT_INTERVAL = 1000;
unsigned long led_lighttime = 1000; //turn on led for 1 sec on startup

//--------protection ratings------------
//LI handeled by balancer
const double LA_I_MAX_CHARGE = 5000;	//5A 
const double LA_I_MAX_DISCHARGE = 14000; //14A
const double LD_I_MAX = 14000; //14A
const double PV_I_MAX = 5000; //5A

const double LA_V_MAX = 16000;	//16V absolute maximum
const double PV_V_MAX = 25000; //25V if full irradiation and not charging

//--------calibration values--------------
const double LA_V_FACTOR  = 19.7333;
const double LA_V_SUMMAND = -45.9976;

const double LA_I_FACTOR  = 15.5647;
const double LA_I_SUMMAND = -7564.6633;

const double PV_V_FACTOR  = 19.7805;
const double PV_V_SUMMAND = 334.8203;

const double PV_I_FACTOR  = 15.9332;
const double PV_I_SUMMAND = -7855.5799;

const double LD_I_FACTOR  = -16.5688;
const double LD_I_SUMMAND = 8210.8356;

const double TEMPERATURE_FACTOR  = 0.2504;
const double TEMPERATURE_SUMMAND = -20.51;

//--------charge protcoll values----------
//const double V_ABS = 14650; //absorption voltage
const double V_ABS = 12300; //for testing
const double V_EQU = 15500; //equalization voltage
//const double V_FLT = 13250; //float voltage
const double V_FLT = 12500; //for testing
const double LA_I_MIN_ABSORP = 10; // C-rate/100
const double PV_VOLTAGE_THRESHOLD = 100; //100mV PV v must be higher then actual battery voltage
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

const double SYS_IDLE_THRESHOLD = 15; //+-20mA don't count as charge or discharge THIS CAN NOT BE SMALLER THAN LA_I_MIN_ABSORP

//---------state variables-------------
byte la_state = 0;
byte li_state = 0;
bool pv_state = 1; 
bool ld_state = 1; 
byte sys_state = 0;

//-----------switching parameters---------
const double SWITCH_THRESHOLD = 200; //200mA discharge current

const bool LA_BATTERY = 0;
const bool LI_BATTERY = 1;

bool current_battery = 0; //LA
bool next_battery = 0; //LA

//-----------------Balancer initializations-------------------
#define BMS_ALERT_PIN 8       // balancer alert pin
#define BMS_BOOT_PIN -1       // there is no boot pin, balancer boots, whenever LiIon is connected
#define BMS_I2C_ADDRESS 0x18  // I2C adress of balancer

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
const double V_KP = 500; //p-factor
const double V_KI = 500; //i-factor
const double V_DMIN = 0;	   	//
const double V_DO = 500; 	//

const double I_KP = 0.05; //p-factor
const double I_KI = 0.05; //i-factor
const double I_DMIN = 0;	//
const double I_DO = 0.5; 	//

double v_d = 0;
double v_sum_e = 0;
double i_d = 0;
double i_sum_e = 0;
double duty_cycle = 255;

//-----------------measurement------------------
const double SYS_MEASUREMENT_INACCURACY = 100; //100mA is acceptable
const int NR_SAMPLES = 100;			//for measureing analog values
double la_v = 0;
double la_i = 0;
double li_v = 0;
double li_i = 0;
double pv_v = 0;
double pv_i = 0;
double ld_i = 0;
double temperature = 0;
double measure_mistake = 0;


//-----------------other------------------
bool multiplex_state_a = 0;
bool multiplex_state_b = 0;
bool multiplex_state_c = 0;
unsigned long lastprint = 0;

//-----------------objects------------------
bq769x0 BMS(bq76920, BMS_I2C_ADDRESS);    // balancer object

void setup() {
	//balancer setup
	int err = BMS.begin(BMS_ALERT_PIN, BMS_BOOT_PIN);

  	BMS.setTemperatureLimits(-20, 45, 0, 45);
  	BMS.setShuntResistorValue(5);						//5mOhm
  	BMS.setShortCircuitProtection(14000, 200);  		// delay in us
  	BMS.setOvercurrentChargeProtection(8000, 200);  	// delay in ms
  	BMS.setOvercurrentDischargeProtection(8000, 320);	// delay in ms
  	BMS.setCellUndervoltageProtection(2600, 2); 		// delay in s
  	BMS.setCellOvervoltageProtection(3650, 2);  		// delay in s

  	BMS.setBalancingThresholds(0, 3300, 20);  			// minIdleTime_min, minCellV_mV, maxVoltageDiff_mV
  	BMS.setIdleCurrentThreshold(100);					// 
  	BMS.enableAutoBalancing();							//let balancer handle balancing

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
	TCCR1B = (TCCR1B & 0b11111000) | 0x05; //PWM frequency set to 60Hz
	Serial.begin(115200);

}

void loop() {
	//some excluded for testing
	measure();            	 	// read all sensor data
	protection();				//software protection for all devices
	//BMS.update();  				// should be called at least every 250 ms
	calculate_SOCs();			// SOCs for both batteries
	set_sys_state();			// dis-/charging/idle
	select_battery();       	// LA or LI
	//switchto_battery();			// LA or LI
	charge_protocol();      	// pwm charging of LA
	load_control();				// load on/off
  	pv_control();				//duty cycle here
	print_data();       	    // print to serial monitor
	led_handler();				// turn led on or off

	//PI tuning

	Serial.print(la_v);
	Serial.print(" ");
	Serial.println(duty_cycle*10);
}

void measure(){
	la_v = get_value(1);
	la_i = get_value(2);
	pv_v = get_value(3);
	pv_i = get_value(4);
	ld_i = get_value(5);
	li_v = BMS.getBatteryVoltage();
	li_i = BMS.getBatteryCurrent();
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
	//take one value every timestep and summ it up, divide it
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
	//take one value every timestep and summ it up, divide it
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
	measure_mistake = pv_i+la_i+li_i-ld_i;
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
	//excluded fot testing
	/*
	pv_state = 0;
	pv_control();
	ld_state = 0;
	load_control();
	led_set(100000);
	Serial.print(F("MEASURE ERROR: "));
	Serial.println(mistake);
	*/
}

void calculate_SOCs(){
	//something for testing
	la_soc = 80;
	li_soc = 100;
}

void set_sys_state(){
	//compare current pv and ld plus threshold
	if(pv_i >= ld_i + SYS_IDLE_THRESHOLD){ 
		sys_state = SYS_CHARGE;
	}else if(pv_i <= ld_i - SYS_IDLE_THRESHOLD){
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
				if(ld_i - pv_i >= SWITCH_THRESHOLD){ //if more then 0.5A dischare current
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

	/*
	//switch every 2 sec for testing
	if(millis()> lastswitch + 2000){ 
		lastswitch = millis();
		if(current_battery == LA_BATTERY){
			next_battery = LI_BATTERY;
		}else{
			next_battery = LA_BATTERY;
		}
	}
	*/

}

void switchto_battery(){
	if(current_battery != next_battery){ //only if switch is needed
		switch(next_battery){
			case LA_BATTERY:
				digitalWrite(LD_SW_PIN, LOW); 	//turn LOAD off
				digitalWrite(LED_BTN_PIN, LOW); //turn LED off
				delay(100);
				BMS.disableDischarging();		//turn LI on
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
				BMS.enableDischarging();		//turn LI on
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
	if(pv_power_available() && current_battery == LA_BATTERY){
		select_chargemode();
		execute_chargemode();
	}
}

bool pv_power_available(){
	if(pv_v > active_bat_v() + PV_VOLTAGE_THRESHOLD){
		return 1;
	}else{
		return 0;
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
	d = pi_controller(LA_I_MAX_CHARGE, la_i, I_KP, I_KI, i_sum_e, I_DMIN, I_DO, i_d);
	duty_cycle = d*255;
}

void absorpmode(){
	//la voltage limited to hold at V_ABS with PI controller
	double d;
	d = pi_controller(V_ABS, la_v, V_KP, V_KI, v_sum_e, V_DMIN, V_DO, v_d);
	duty_cycle = d*255;
}

void floatmode(){
	//la voltage limited to hold at V_FLT with PI controller
	double d;
	d = pi_controller(V_FLT, la_v, V_KP, V_KI, v_sum_e, V_DMIN, V_DO, v_d);
	duty_cycle = d*255;
}

void equalizemode(){
	//la voltage limited to hold at V_EQU with PI controller
	double d;
	d = pi_controller(V_EQU, la_v, V_KP, V_KI, v_sum_e, V_DMIN, V_DO, v_d);
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
	//only allow pv switch to open, if voltage is high enough and pv_state = 1
	if(pv_power_available() & pv_state == 1){
		analogWrite(PV_SW_PIN,duty_cycle);
	}else{
		analogWrite(PV_SW_PIN, 0);
	}
}

double active_bat_v(){
	//gives voltage of active battery
	if(current_battery == LA_BATTERY){
		return la_v;
	}else{
		return li_v;
	}
}

double active_bat_i(){
	//gives current of active battery
	if(current_battery == LA_BATTERY){
		return la_i;
	}else{
		return li_i;
	}
}

double pi_controller(double setpoint, double process_variable, double kp, double ki, double sum_e, double dmin, double d_o, double d){
	//d und sum_e sollten für die nächste runde gespeichert werden...
	double e = setpoint/1000 - process_variable/1000;
	//Serial.println(e);
	if(d<1){
		sum_e = sum_e + e;
	}
	Serial.println(e);
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
	bool plotting_format = 0;
	if(millis()> lastprint + PRINT_INTERVAL){ //only print if last print was PRINT_INTERVAL millisec ago
		lastprint = millis();
		
		if(!plotting_format){
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
			Serial.println(F("----------------MEASUREMENTS"));
			Serial.print(F("la_v: "));
			Serial.println(la_v);
			Serial.print(F("la_i: "));
			Serial.println(la_i);
			Serial.print(F("li_v: "));
			Serial.println(li_v);
			Serial.print(F("li_i: "));
			Serial.println(li_i);
			Serial.print(F("pv_v: "));
			Serial.println(pv_v);
			Serial.print(F("pv_i: "));
			Serial.println(pv_i);
			Serial.print(F("ld_i: "));
			Serial.println(ld_i);
			Serial.print(F("temperature: "));
			Serial.println(temperature);
			Serial.print("ld_i - pv_i: ");
			Serial.println(ld_i - pv_i);
			Serial.print(F("measure_mistake: "));
			Serial.println(measure_mistake);

			//PI-Controller--------------------
			Serial.println(F("--------------PI-CONTROLLER"));
			Serial.print(F("duty_cycle: "));
			Serial.println(duty_cycle);
			Serial.print(F("v_d: "));
			Serial.println(v_d);
			Serial.print(F("v_sum_e: "));
			Serial.println(v_sum_e);
			Serial.print(F("i_d: "));
			Serial.println(i_d);
			Serial.print(F("i_sum_e: "));
			Serial.println(i_sum_e);


			//other
			Serial.println(F("----------------------OTHER"));


			//LI-ION
			//BMS.printRegisters();
		}else{
			Serial.print(millis());
			Serial.print(" ");
			Serial.print(current_battery);
			Serial.print(F(" "));
			Serial.print(sys_state);
			Serial.print(F(" "));
			Serial.print(la_state);
			Serial.print(F(" "));
			Serial.print(li_state);
			Serial.print(F(" "));
			Serial.print(pv_state);
			Serial.print(F(" "));
			Serial.print(ld_state);

			//measured data-------------------
			Serial.print(F(" "));
			Serial.print(la_v);
			Serial.print(F(" "));
			Serial.print(la_i);
			Serial.print(F(" "));
			Serial.print(li_v);
			Serial.print(F(" "));
			Serial.print(li_i);
			Serial.print(F(" "));
			Serial.print(pv_v);
			Serial.print(F(" "));
			Serial.print(pv_i);
			Serial.print(F(" "));
			Serial.print(ld_i);
			Serial.print(F(" "));
			Serial.print(temperature);
			Serial.print(" ");
			Serial.print(ld_i - pv_i);
			Serial.print(F(" "));
			Serial.print(measure_mistake);

			//other
			Serial.print(F(" "));
			Serial.println(duty_cycle);
		}
	}
}        
