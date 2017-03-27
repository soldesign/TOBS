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
LIBS:HybridChargeController-cache
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
$Comp
L R R?
U 1 1 58D3AA5E
P 2500 4350
F 0 "R?" V 2580 4350 50  0000 C CNN
F 1 "47k" V 2500 4350 50  0000 C CNN
F 2 "" V 2430 4350 50  0000 C CNN
F 3 "" H 2500 4350 50  0000 C CNN
	1    2500 4350
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 58D3AA9E
P 2250 4850
F 0 "R?" V 2330 4850 50  0000 C CNN
F 1 "100k" V 2250 4850 50  0000 C CNN
F 2 "" V 2180 4850 50  0000 C CNN
F 3 "" H 2250 4850 50  0000 C CNN
	1    2250 4850
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q?
U 1 1 58D3AC23
P 2950 4600
F 0 "Q?" H 3250 4650 50  0000 R CNN
F 1 "2N 7002" H 3450 4550 50  0000 R CNN
F 2 "" H 3150 4700 50  0000 C CNN
F 3 "" H 2950 4600 50  0000 C CNN
	1    2950 4600
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GDS Q?
U 1 1 58D3AC5B
P 3500 4100
F 0 "Q?" V 3750 4350 50  0000 R CNN
F 1 "IPD90P04P4_05" V 3750 4200 50  0000 R CNN
F 2 "" H 3700 4200 50  0000 C CNN
F 3 "" H 3500 4100 50  0000 C CNN
	1    3500 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2650 4350 3050 4350
Wire Wire Line
	3500 4300 3050 4300
Wire Wire Line
	3050 4300 3050 4400
Connection ~ 3050 4350
Wire Wire Line
	1100 5200 3750 5200
Text Label 1100 5200 2    60   ~ 0
GND
Text Label 3750 5200 0    60   ~ 0
LA_GND
Wire Wire Line
	3050 4800 3050 5200
Connection ~ 3050 5200
Wire Wire Line
	2250 5000 2250 5200
Connection ~ 2250 5200
Wire Wire Line
	2050 4600 2750 4600
Wire Wire Line
	2250 4600 2250 4700
Connection ~ 2250 4600
Text Label 1100 4750 2    60   ~ 0
SW_BAT
Wire Wire Line
	3700 4000 4000 4000
Text Label 4000 4000 0    60   ~ 0
LA_12V
Wire Wire Line
	2350 4350 2350 4000
Wire Wire Line
	1100 4000 3300 4000
Connection ~ 2350 4000
Text Label 1100 4000 2    60   ~ 0
VCC
Wire Notes Line
	750  3100 750  5500
Wire Notes Line
	750  5500 4450 5500
Wire Notes Line
	4450 5500 4450 3100
Wire Notes Line
	4450 3100 750  3100
Text Notes 1550 3300 0    60   ~ 0
LA switch circuit \nLoad switch is closed, when SW_BAT is LOW
$Comp
L R R?
U 1 1 58D3CBEE
P 1750 1800
F 0 "R?" V 1830 1800 50  0000 C CNN
F 1 "47k" V 1750 1800 50  0000 C CNN
F 2 "" V 1680 1800 50  0000 C CNN
F 3 "" H 1750 1800 50  0000 C CNN
	1    1750 1800
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 58D3CBF4
P 1500 2300
F 0 "R?" V 1580 2300 50  0000 C CNN
F 1 "100k" V 1500 2300 50  0000 C CNN
F 2 "" V 1430 2300 50  0000 C CNN
F 3 "" H 1500 2300 50  0000 C CNN
	1    1500 2300
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q?
U 1 1 58D3CBFA
P 2200 2050
F 0 "Q?" H 2500 2100 50  0000 R CNN
F 1 "2N 7002" H 2700 2000 50  0000 R CNN
F 2 "" H 2400 2150 50  0000 C CNN
F 3 "" H 2200 2050 50  0000 C CNN
	1    2200 2050
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GDS Q?
U 1 1 58D3CC00
P 2750 1550
F 0 "Q?" V 3000 1700 50  0000 R CNN
F 1 "IPD90P04P4_05" V 3000 1550 50  0000 R CNN
F 2 "" H 2950 1650 50  0000 C CNN
F 3 "" H 2750 1550 50  0000 C CNN
	1    2750 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1900 1800 2300 1800
Wire Wire Line
	2750 1750 2300 1750
Wire Wire Line
	2300 1750 2300 1850
Connection ~ 2300 1800
Wire Wire Line
	1200 2650 3000 2650
Text Label 1200 2650 2    60   ~ 0
GND
Text Label 3000 2650 0    60   ~ 0
LA_GND
Wire Wire Line
	2300 2250 2300 2650
Connection ~ 2300 2650
Wire Wire Line
	1500 2450 1500 2650
Connection ~ 1500 2650
Wire Wire Line
	1300 2050 2000 2050
Wire Wire Line
	1500 2050 1500 2150
Connection ~ 1500 2050
Text Label 1300 2050 2    60   ~ 0
SW_BAT
Wire Wire Line
	2950 1450 3250 1450
Text Label 3250 1450 0    60   ~ 0
LA_12V
Wire Wire Line
	1600 1800 1600 1450
Wire Wire Line
	1150 1450 2550 1450
Connection ~ 1600 1450
Text Label 1150 1450 2    60   ~ 0
VCC
Wire Notes Line
	750  550  750  2950
Wire Notes Line
	750  2950 3700 2950
Wire Notes Line
	3700 2950 3700 550 
Wire Notes Line
	3700 550  750  550 
Text Notes 800  750  0    60   ~ 0
LA switch circuit \nLoad switch is closed, when SW_BAT is LOW
$Comp
L Q_NMOS_GSD Q?
U 1 1 58D8C0D3
P 1500 4750
F 0 "Q?" H 1800 4800 50  0000 R CNN
F 1 "2N 7002" H 2000 4700 50  0000 R CNN
F 2 "" H 1700 4850 50  0000 C CNN
F 3 "" H 1500 4750 50  0000 C CNN
	1    1500 4750
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58D8C1D7
P 1600 4250
F 0 "R?" V 1680 4250 50  0000 C CNN
F 1 "47k" V 1600 4250 50  0000 C CNN
F 2 "" V 1530 4250 50  0000 C CNN
F 3 "" H 1600 4250 50  0000 C CNN
	1    1600 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	1600 4100 1600 4000
Connection ~ 1600 4000
Wire Wire Line
	1600 4400 1600 4550
Wire Wire Line
	1600 4450 2050 4450
Wire Wire Line
	2050 4450 2050 4600
Connection ~ 1600 4450
Wire Wire Line
	1600 4950 1600 5200
Connection ~ 1600 5200
Wire Wire Line
	1300 4750 1100 4750
$EndSCHEMATC
