EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	3000 1575 3300 1575
Wire Wire Line
	3300 1575 5900 1575
Text Label 3000 1575 2    60   ~ 0
Li-Ion+(14V)
Text Label 5900 1575 0    60   ~ 0
COM+(12V)
Wire Wire Line
	2950 2675 3300 2675
Wire Wire Line
	3300 2675 3575 2675
Wire Wire Line
	4575 2675 4675 2675
Wire Wire Line
	4675 2675 5925 2675
Text Label 2950 2675 2    60   ~ 0
Li-Ion-
Text Label 5925 2675 0    60   ~ 0
GND
Text Label 3625 2025 0    60   ~ 0
VCC
Text Label 3625 2125 0    60   ~ 0
GND(b)
Text Label 4175 2150 2    60   ~ 0
SW
Wire Notes Line
	3575 1825 4225 1825
Wire Notes Line
	4225 1825 4225 2175
Wire Notes Line
	4225 2175 3575 2175
Wire Notes Line
	3575 2175 3575 1825
Text Notes 3600 1925 0    60   ~ 0
Balancer
Text Label 4975 2150 0    60   ~ 0
GND
Text Label 5700 2125 2    60   ~ 0
5V
Wire Notes Line
	4950 1825 5725 1825
Wire Notes Line
	5725 1825 5725 2175
Wire Notes Line
	5725 2175 4950 2175
Wire Notes Line
	4950 2175 4950 1825
Text Notes 5175 1925 0    60   ~ 0
KaranaBase
Text Label 4175 1925 2    60   ~ 0
SDA
Text Label 4175 2025 2    60   ~ 0
SCL
Text Label 4975 1925 0    60   ~ 0
SDA
Text Label 4975 2025 0    60   ~ 0
SCL
Wire Wire Line
	4175 1925 4975 1925
Wire Wire Line
	4175 2025 4975 2025
Wire Wire Line
	4175 2150 4300 2150
Wire Wire Line
	4300 2150 4300 2425
Wire Wire Line
	3625 2025 3300 2025
Wire Wire Line
	3300 2025 3300 1575
Connection ~ 3300 1575
Wire Wire Line
	3625 2125 3300 2125
Wire Wire Line
	3300 2125 3300 2675
Connection ~ 3300 2675
Wire Wire Line
	4975 2150 4675 2150
Wire Wire Line
	4675 2150 4675 2675
Connection ~ 4675 2675
$Comp
L SPST SW?
U 1 1 5937CFFA
P 4075 2675
F 0 "SW?" H 4075 2775 50  0001 C CNN
F 1 "SPST" H 4075 2575 50  0001 C CNN
F 2 "" H 4075 2675 50  0000 C CNN
F 3 "" H 4075 2675 50  0000 C CNN
	1    4075 2675
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 2425 4075 2425
Wire Wire Line
	4075 2425 4075 2625
Wire Wire Line
	5700 2125 5925 2125
Text Label 5925 2125 0    60   ~ 0
5V(regulator)
Wire Notes Line
	3225 1250 3225 2800
Wire Notes Line
	3225 2800 2325 2800
Wire Notes Line
	2325 2800 2325 1250
Wire Notes Line
	2325 1250 3225 1250
Text Notes 2375 1350 0    60   ~ 0
Li-Ion battery
Wire Notes Line
	4800 1250 6750 1250
Wire Notes Line
	6750 1250 6750 2800
Wire Notes Line
	6750 2800 4800 2800
Wire Notes Line
	4800 2800 4800 1250
Text Notes 4825 1350 0    60   ~ 0
Hybrid Charge Controller
Text Notes 3800 2825 0    60   ~ 0
Li-Ion switch
$EndSCHEMATC
