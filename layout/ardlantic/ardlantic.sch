EESchema Schematic File Version 4
LIBS:ardlantic-cache
EELAYER 26 0
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
L promicro:ProMicro U1
U 1 1 5C4DF160
P 4650 2950
F 0 "U1" H 4650 3987 60  0000 C CNN
F 1 "Arduino Pro Micro" H 4650 3881 60  0000 C CNN
F 2 "layout:ProMicro" H 4750 1900 60  0001 C CNN
F 3 "" H 4750 1900 60  0000 C CNN
	1    4650 2950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J4
U 1 1 5C4E0B31
P 7300 2700
F 0 "J4" H 7379 2692 50  0000 L CNN
F 1 "Shield NRF24 Input" H 7379 2601 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7300 2700 50  0001 C CNN
F 3 "~" H 7300 2700 50  0001 C CNN
	1    7300 2700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J6
U 1 1 5C4E0E70
P 8700 2750
F 0 "J6" H 8750 3067 50  0000 C CNN
F 1 "Shield NRF24 Output" H 8750 2976 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x08_P2.54mm_Vertical" H 8700 2750 50  0001 C CNN
F 3 "~" H 8700 2750 50  0001 C CNN
	1    8700 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 5C4E122E
P 5850 4200
F 0 "J3" H 5929 4242 50  0000 L CNN
F 1 "IR Input" H 5929 4151 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5850 4200 50  0001 C CNN
F 3 "~" H 5850 4200 50  0001 C CNN
	1    5850 4200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 5C4E133B
P 4850 1100
F 0 "J1" H 4930 1092 50  0000 L CNN
F 1 "PowerSupply" H 4930 1001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4850 1100 50  0001 C CNN
F 3 "~" H 4850 1100 50  0001 C CNN
	1    4850 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J2
U 1 1 5C4E1537
P 7200 900
F 0 "J2" H 7279 942 50  0000 L CNN
F 1 "OnOffSensor" H 7279 851 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 7200 900 50  0001 C CNN
F 3 "~" H 7200 900 50  0001 C CNN
	1    7200 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3300 3950 4300
Wire Wire Line
	3950 4300 5650 4300
Text Label 5500 4300 0    50   ~ 0
DAT
Wire Wire Line
	5350 2500 5650 2500
Wire Wire Line
	5650 2500 5650 3300
Wire Wire Line
	5650 3700 5300 3700
Wire Wire Line
	5300 3700 5300 4200
Wire Wire Line
	5300 4200 5650 4200
Text Label 5500 4200 0    50   ~ 0
VCC
Text Label 5500 4100 0    50   ~ 0
GND
Wire Wire Line
	3950 2600 3800 2600
Wire Wire Line
	3800 2600 3800 1850
Wire Wire Line
	3800 1850 6300 1850
Wire Wire Line
	6300 1850 6300 2500
Wire Wire Line
	6300 2500 7100 2500
Text Label 7000 2500 0    50   ~ 0
CE
Wire Wire Line
	3950 2700 3700 2700
Wire Wire Line
	3700 2700 3700 1700
Wire Wire Line
	3700 1700 6400 1700
Wire Wire Line
	6400 1700 6400 2600
Wire Wire Line
	6400 2600 7100 2600
Text Label 7000 2600 0    50   ~ 0
CS
Text Label 6950 2700 0    50   ~ 0
SCK
Wire Wire Line
	5350 3000 6400 3000
Text Label 7000 2800 0    50   ~ 0
M0
Wire Wire Line
	6400 2700 7100 2700
Wire Wire Line
	6400 2700 6400 3000
Wire Wire Line
	5350 3200 6500 3200
Wire Wire Line
	6500 3200 6500 2800
Wire Wire Line
	6500 2800 7100 2800
Wire Wire Line
	6600 3100 6600 2900
Wire Wire Line
	6600 2900 7100 2900
Wire Wire Line
	5350 3100 6600 3100
Text Label 7000 2900 0    50   ~ 0
M1
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 5C4E447F
P 7300 3300
F 0 "J5" H 7380 3292 50  0000 L CNN
F 1 "Shield NRF24 PowerSupply" H 7380 3201 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7300 3300 50  0001 C CNN
F 3 "~" H 7300 3300 50  0001 C CNN
	1    7300 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3300 7100 3300
Connection ~ 5650 3300
Wire Wire Line
	5650 3300 5650 3700
Wire Wire Line
	5350 2700 6100 2700
Text Label 6950 3300 0    50   ~ 0
VCC
Text Label 6950 3400 0    50   ~ 0
GND
Wire Wire Line
	5350 2200 5350 900 
Wire Wire Line
	5350 900  4400 900 
Wire Wire Line
	4400 900  4400 1100
Wire Wire Line
	4400 1100 4650 1100
Wire Wire Line
	4400 1200 4650 1200
Text Label 4500 1100 0    50   ~ 0
VDC
Text Label 4500 1200 0    50   ~ 0
GND
$Comp
L Device:R_US R1
U 1 1 5C4E967E
P 6900 1350
F 0 "R1" H 6968 1396 50  0000 L CNN
F 1 "1M" H 6968 1305 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6940 1340 50  0001 C CNN
F 3 "~" H 6900 1350 50  0001 C CNN
	1    6900 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 5C4E9799
P 6900 1850
F 0 "R2" H 6968 1896 50  0000 L CNN
F 1 "100K" H 6968 1805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6940 1840 50  0001 C CNN
F 3 "~" H 6900 1850 50  0001 C CNN
	1    6900 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5C4EA1A6
P 4400 1200
F 0 "#PWR0101" H 4400 950 50  0001 C CNN
F 1 "GND" H 4405 1027 50  0000 C CNN
F 2 "" H 4400 1200 50  0001 C CNN
F 3 "" H 4400 1200 50  0001 C CNN
	1    4400 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5C4EDD4D
P 6850 3400
F 0 "#PWR0102" H 6850 3150 50  0001 C CNN
F 1 "GND" H 6855 3227 50  0000 C CNN
F 2 "" H 6850 3400 50  0001 C CNN
F 3 "" H 6850 3400 50  0001 C CNN
	1    6850 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5C4EE350
P 5400 4400
F 0 "#PWR0103" H 5400 4150 50  0001 C CNN
F 1 "GND" H 5405 4227 50  0000 C CNN
F 2 "" H 5400 4400 50  0001 C CNN
F 3 "" H 5400 4400 50  0001 C CNN
	1    5400 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 4100 5650 4100
Wire Wire Line
	5400 4100 5400 4400
Wire Wire Line
	6850 3400 7100 3400
$Comp
L power:GND #PWR0104
U 1 1 5C4EFC4B
P 6900 2100
F 0 "#PWR0104" H 6900 1850 50  0001 C CNN
F 1 "GND" H 6905 1927 50  0000 C CNN
F 2 "" H 6900 2100 50  0001 C CNN
F 3 "" H 6900 2100 50  0001 C CNN
	1    6900 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 2000 6900 2100
Wire Wire Line
	6900 1500 6900 1600
Wire Wire Line
	6900 1200 6900 900 
Wire Wire Line
	6900 900  7000 900 
Wire Wire Line
	6100 2700 6100 1600
Wire Wire Line
	6100 1600 6900 1600
Connection ~ 6900 1600
Wire Wire Line
	6900 1600 6900 1700
Text Label 8150 2650 0    50   ~ 0
VCC_NRF
Text Label 8200 2750 0    50   ~ 0
CS_NRF
Text Label 8200 2850 0    50   ~ 0
M0_NRF
Text Label 9000 2650 0    50   ~ 0
GND_NRF
Text Label 9000 2750 0    50   ~ 0
CE_NRF
Text Label 9000 2850 0    50   ~ 0
SCK_NRF
Text Label 9000 2950 0    50   ~ 0
M1_NRF
Text Label 6950 3000 0    50   ~ 0
1RO
$Comp
L Device:CP C1
U 1 1 5C4F73B4
P 8450 2050
F 0 "C1" H 8568 2096 50  0000 L CNN
F 1 "470µF" H 8568 2005 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 8488 1900 50  0001 C CNN
F 3 "~" H 8450 2050 50  0001 C CNN
	1    8450 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5C4F74CE
P 8950 2050
F 0 "C2" H 9065 2096 50  0000 L CNN
F 1 "100nF" H 9065 2005 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 8988 1900 50  0001 C CNN
F 3 "~" H 8950 2050 50  0001 C CNN
	1    8950 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 2650 9000 2650
Wire Wire Line
	8450 2200 8950 2200
Wire Wire Line
	8450 1900 8950 1900
Wire Wire Line
	9250 2650 9250 2200
Wire Wire Line
	9250 2200 8950 2200
Connection ~ 8950 2200
Wire Wire Line
	8300 1900 8450 1900
Connection ~ 8450 1900
Wire Wire Line
	8300 1900 8300 2650
Wire Wire Line
	8300 2650 8500 2650
$Comp
L Device:R_US R3
U 1 1 5C56EEAD
P 2750 2800
F 0 "R3" H 2818 2846 50  0000 L CNN
F 1 "4.7K" H 2818 2755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2790 2790 50  0001 C CNN
F 3 "~" H 2750 2800 50  0001 C CNN
	1    2750 2800
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 5C56F94D
P 3250 3350
F 0 "J7" H 3329 3392 50  0000 L CNN
F 1 "Temp Input" H 3329 3301 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3250 3350 50  0001 C CNN
F 3 "~" H 3250 3350 50  0001 C CNN
	1    3250 3350
	1    0    0    -1  
$EndComp
Text Label 2850 3250 0    50   ~ 0
VCC
Text Label 2850 3450 0    50   ~ 0
GND
Text Label 2750 3350 0    50   ~ 0
TDATA
Wire Wire Line
	2300 2400 3950 2400
Wire Wire Line
	2300 2400 2300 2800
Wire Wire Line
	2300 3350 3050 3350
Wire Wire Line
	2600 2800 2300 2800
Connection ~ 2300 2800
Wire Wire Line
	2300 2800 2300 3350
Wire Wire Line
	3050 3250 2750 3250
Wire Wire Line
	2750 3250 2750 3000
Wire Wire Line
	2750 3000 2900 3000
Wire Wire Line
	2900 3000 2900 2800
Wire Wire Line
	2900 3000 3850 3000
Wire Wire Line
	3850 3000 3850 3700
Wire Wire Line
	3850 3700 5300 3700
Connection ~ 2900 3000
Connection ~ 5300 3700
$Comp
L power:GND #PWR0105
U 1 1 5C577FB9
P 2750 3550
F 0 "#PWR0105" H 2750 3300 50  0001 C CNN
F 1 "GND" H 2755 3377 50  0000 C CNN
F 2 "" H 2750 3550 50  0001 C CNN
F 3 "" H 2750 3550 50  0001 C CNN
	1    2750 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3450 2750 3450
Wire Wire Line
	2750 3450 2750 3550
$EndSCHEMATC
