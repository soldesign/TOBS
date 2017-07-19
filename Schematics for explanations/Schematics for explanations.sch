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
LIBS:Schematics for explanations-cache
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
Text Label 3000 1575 2    60   ~ 0
Li-Ion+(14V)
Text Label 5900 1575 0    60   ~ 0
COM+(12V)
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
Text Notes 3600 1925 0    60   ~ 0
Balancer
Text Label 4975 2150 0    60   ~ 0
GND
Text Label 5700 2125 2    60   ~ 0
5V
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
Text Label 5925 2125 0    60   ~ 0
5V(regulator)
Text Notes 2375 1350 0    60   ~ 0
Li-Ion battery
Text Notes 4825 1350 0    60   ~ 0
Hybrid Charge Controller
Text Notes 3800 2825 0    60   ~ 0
Li-Ion switch
$Comp
L R R?
U 1 1 596CEAE9
P 2800 4250
F 0 "R?" V 2880 4250 50  0001 C CNN
F 1 "R" V 2800 4250 50  0000 C CNN
F 2 "" V 2730 4250 50  0000 C CNN
F 3 "" H 2800 4250 50  0000 C CNN
	1    2800 4250
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 596CEB13
P 2800 4650
F 0 "R?" V 2880 4650 50  0001 C CNN
F 1 "R" V 2800 4650 50  0000 C CNN
F 2 "" V 2730 4650 50  0000 C CNN
F 3 "" H 2800 4650 50  0000 C CNN
	1    2800 4650
	1    0    0    -1  
$EndComp
Text Label 2725 4450 2    60   ~ 0
PV_V
$Comp
L D_Schottky D?
U 1 1 596CEC42
P 2300 4000
F 0 "D?" H 2300 4100 50  0001 C CNN
F 1 "Schottky" H 2300 3900 50  0000 C CNN
F 2 "" H 2300 4000 50  0000 C CNN
F 3 "" H 2300 4000 50  0000 C CNN
	1    2300 4000
	-1   0    0    1   
$EndComp
$Comp
L BS107 Q?
U 1 1 596CED69
P 3200 4100
F 0 "Q?" H 3400 4175 50  0001 L CNN
F 1 "BS107" H 3400 4100 50  0001 L CNN
F 2 "TO-92" H 3400 4025 50  0001 L CIN
F 3 "" H 3200 4100 50  0000 L CNN
	1    3200 4100
	0    -1   -1   0   
$EndComp
Text Label 3250 4425 0    60   ~ 0
PV_ON/Off
Text Label 3525 4000 0    60   ~ 0
COM+
Text Label 1550 4900 2    60   ~ 0
PV-
Text Label 1550 4000 2    60   ~ 0
PV+
Text Label 3550 4900 0    60   ~ 0
GND
$Comp
L R R?
U 1 1 596CF085
P 1975 4250
F 0 "R?" V 2055 4250 50  0001 C CNN
F 1 "R" V 1975 4250 50  0000 C CNN
F 2 "" V 1905 4250 50  0000 C CNN
F 3 "" H 1975 4250 50  0000 C CNN
	1    1975 4250
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 596CF08B
P 1975 4650
F 0 "R?" V 2055 4650 50  0001 C CNN
F 1 "R" V 1975 4650 50  0000 C CNN
F 2 "" V 1905 4650 50  0000 C CNN
F 3 "" H 1975 4650 50  0000 C CNN
	1    1975 4650
	1    0    0    -1  
$EndComp
Text Label 1900 4450 2    60   ~ 0
PV_V
Wire Wire Line
	3000 1575 3300 1575
Wire Wire Line
	3300 1575 5900 1575
Wire Wire Line
	2950 2675 3300 2675
Wire Wire Line
	3300 2675 3575 2675
Wire Wire Line
	4575 2675 4675 2675
Wire Wire Line
	4675 2675 5925 2675
Wire Notes Line
	3575 1825 4225 1825
Wire Notes Line
	4225 1825 4225 2175
Wire Notes Line
	4225 2175 3575 2175
Wire Notes Line
	3575 2175 3575 1825
Wire Notes Line
	4950 1825 5725 1825
Wire Notes Line
	5725 1825 5725 2175
Wire Notes Line
	5725 2175 4950 2175
Wire Notes Line
	4950 2175 4950 1825
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
Wire Wire Line
	4300 2425 4075 2425
Wire Wire Line
	4075 2425 4075 2625
Wire Wire Line
	5700 2125 5925 2125
Wire Notes Line
	3225 1250 3225 2800
Wire Notes Line
	3225 2800 2325 2800
Wire Notes Line
	2325 2800 2325 1250
Wire Notes Line
	2325 1250 3225 1250
Wire Notes Line
	4800 1250 6750 1250
Wire Notes Line
	6750 1250 6750 2800
Wire Notes Line
	6750 2800 4800 2800
Wire Notes Line
	4800 2800 4800 1250
Wire Wire Line
	2800 4400 2800 4450
Wire Wire Line
	2800 4450 2800 4500
Wire Wire Line
	2800 4900 2800 4800
Wire Wire Line
	2800 4100 2800 4000
Wire Wire Line
	2800 4450 2725 4450
Connection ~ 2800 4450
Wire Wire Line
	1550 4000 1975 4000
Wire Wire Line
	1975 4000 2150 4000
Wire Wire Line
	2450 4000 2800 4000
Wire Wire Line
	2800 4000 3000 4000
Connection ~ 2800 4000
Wire Wire Line
	3250 4300 3250 4425
Wire Wire Line
	3400 4000 3525 4000
Wire Wire Line
	1550 4900 1975 4900
Wire Wire Line
	1975 4900 2800 4900
Wire Wire Line
	2800 4900 3550 4900
Connection ~ 2800 4900
Wire Wire Line
	1975 4400 1975 4450
Wire Wire Line
	1975 4450 1975 4500
Wire Wire Line
	1975 4450 1900 4450
Connection ~ 1975 4450
Wire Wire Line
	1975 4100 1975 4000
Connection ~ 1975 4000
Wire Wire Line
	1975 4800 1975 4900
Connection ~ 1975 4900
Wire Notes Line
	2075 3950 2075 4950
Wire Notes Line
	2075 4950 1675 4950
Wire Notes Line
	1675 4950 1675 3950
Wire Notes Line
	1675 3950 2075 3950
Wire Notes Line
	2900 3950 2500 3950
Wire Notes Line
	2500 3950 2500 4950
Wire Notes Line
	2500 4950 2900 4950
Wire Notes Line
	2900 4950 2900 3950
Text Notes 2175 4675 0    60   ~ 0
<--\nmove \nover \nhere\n<--
$EndSCHEMATC
