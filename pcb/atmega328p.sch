EESchema Schematic File Version 4
EELAYER 30 0
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
L Connector_Generic:Conn_01x05 J7
U 1 1 602B4E40
P 7800 3300
F 0 "J7" H 7880 3342 50  0000 L CNN
F 1 "Motor" H 7880 3251 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7800 3300 50  0001 C CNN
F 3 "~" H 7800 3300 50  0001 C CNN
	1    7800 3300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 602B5A9C
P 7800 1950
F 0 "J6" H 7880 1942 50  0000 L CNN
F 1 "Outlet Temperature" H 7880 1851 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 1950 50  0001 C CNN
F 3 "~" H 7800 1950 50  0001 C CNN
	1    7800 1950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 602B6735
P 7800 1050
F 0 "J5" H 7880 1042 50  0000 L CNN
F 1 "Room Temperature" H 7880 951 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 1050 50  0001 C CNN
F 3 "~" H 7800 1050 50  0001 C CNN
	1    7800 1050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 602B7E05
P 7900 4300
F 0 "J8" H 7980 4292 50  0000 L CNN
F 1 "Glow Plug" H 7980 4201 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7900 4300 50  0001 C CNN
F 3 "~" H 7900 4300 50  0001 C CNN
	1    7900 4300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J9
U 1 1 602B88C2
P 7900 5150
F 0 "J9" H 7980 5142 50  0000 L CNN
F 1 "Oil Pump" H 7980 5051 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7900 5150 50  0001 C CNN
F 3 "~" H 7900 5150 50  0001 C CNN
	1    7900 5150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 602BB0A8
P 2150 1500
F 0 "J2" V 2114 1312 50  0000 R CNN
F 1 "Power" V 2023 1312 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2150 1500 50  0001 C CNN
F 3 "~" H 2150 1500 50  0001 C CNN
	1    2150 1500
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J4
U 1 1 602BB81E
P 3950 1500
F 0 "J4" V 3914 1312 50  0000 R CNN
F 1 "Hall Sensor" V 3823 1312 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3950 1500 50  0001 C CNN
F 3 "~" H 3950 1500 50  0001 C CNN
	1    3950 1500
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x15 J1
U 1 1 602BCCD9
P 1800 4050
F 0 "J1" H 1880 4092 50  0000 L CNN
F 1 "Arduini MINI Left" H 1450 3200 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x15_P2.54mm_Vertical" H 1800 4050 50  0001 C CNN
F 3 "~" H 1800 4050 50  0001 C CNN
	1    1800 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x15 J3
U 1 1 602BE6EB
P 2850 4050
F 0 "J3" H 2930 4092 50  0000 L CNN
F 1 "Arduini MINI Right" H 2500 3200 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x15_P2.54mm_Vertical" H 2850 4050 50  0001 C CNN
F 3 "~" H 2850 4050 50  0001 C CNN
	1    2850 4050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2150 1700 2150 1750
Text Label 2150 2000 1    50   ~ 0
5V
Text Label 2250 2000 1    50   ~ 0
GND
$Comp
L power:PWR_FLAG #FLG01
U 1 1 602C1891
P 1650 1700
F 0 "#FLG01" H 1650 1775 50  0001 C CNN
F 1 "PWR_FLAG" H 1650 1873 50  0000 C CNN
F 2 "" H 1650 1700 50  0001 C CNN
F 3 "~" H 1650 1700 50  0001 C CNN
	1    1650 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1750 1650 1750
Wire Wire Line
	1650 1750 1650 1700
Connection ~ 2150 1750
Wire Wire Line
	2150 1750 2150 2100
Text Label 7050 1050 0    50   ~ 0
RT_NTC50K_P
Text Label 7050 1150 0    50   ~ 0
RT_NTC50K_N
Text Label 7050 1950 0    50   ~ 0
OT_NTC50K_P
Text Label 7050 2050 0    50   ~ 0
OT_NTC50K_N
Wire Wire Line
	7600 3100 6900 3100
Wire Wire Line
	7600 3200 6900 3200
Text Label 7050 3100 0    50   ~ 0
5V
Text Label 7050 3200 0    50   ~ 0
GND
Text Label 7050 3300 0    50   ~ 0
M_INA
Text Label 7050 3400 0    50   ~ 0
M_INB
Text Label 7050 3500 0    50   ~ 0
M_PWM
Wire Wire Line
	7700 4400 7000 4400
Wire Wire Line
	7700 5250 7000 5250
Wire Wire Line
	3850 1700 3850 2200
Wire Wire Line
	3950 1700 3950 2200
Wire Wire Line
	4050 1700 4050 2200
Text Label 3850 2100 1    50   ~ 0
5V
Text Label 3950 2100 1    50   ~ 0
GND
Text Label 4050 2100 1    50   ~ 0
HS_INPUT
Text Label 7150 4300 0    50   ~ 0
GP_SIG
Text Label 7150 4400 0    50   ~ 0
GND
Text Label 7150 5150 0    50   ~ 0
OP_SIG
Text Label 7150 5250 0    50   ~ 0
GND
Wire Wire Line
	1600 3350 950  3350
Wire Wire Line
	1600 3450 950  3450
Wire Wire Line
	1600 3550 950  3550
Wire Wire Line
	1600 3650 950  3650
Wire Wire Line
	1600 3750 950  3750
Wire Wire Line
	1600 3850 950  3850
Wire Wire Line
	1600 3950 950  3950
Wire Wire Line
	1600 4050 950  4050
Wire Wire Line
	1600 4150 950  4150
Wire Wire Line
	1600 4250 950  4250
Wire Wire Line
	1600 4350 950  4350
Wire Wire Line
	1600 4450 950  4450
Wire Wire Line
	1600 4550 950  4550
Wire Wire Line
	1600 4650 950  4650
Wire Wire Line
	1600 4750 950  4750
Wire Wire Line
	3050 3350 3700 3350
Wire Wire Line
	3050 3450 3700 3450
Wire Wire Line
	3050 3550 3700 3550
Wire Wire Line
	3050 3650 3700 3650
Wire Wire Line
	3050 3750 3700 3750
Wire Wire Line
	3050 3850 3700 3850
Wire Wire Line
	3050 3950 3700 3950
Wire Wire Line
	3050 4050 3700 4050
Wire Wire Line
	3050 4150 3700 4150
Wire Wire Line
	3050 4250 3700 4250
Wire Wire Line
	3050 4350 3700 4350
Wire Wire Line
	3050 4450 3700 4450
Wire Wire Line
	3050 4550 3700 4550
Wire Wire Line
	3050 4650 3700 4650
Wire Wire Line
	3050 4750 3700 4750
Text Label 1200 3750 0    50   ~ 0
GP_SIG
Text Label 1200 3850 0    50   ~ 0
OP_SIG
Text Label 1200 4150 0    50   ~ 0
BTN0
Text Label 3250 3350 0    50   ~ 0
5V
Text Label 3250 3450 0    50   ~ 0
GND
Text Label 3250 4350 0    50   ~ 0
RT
Text Label 3250 4450 0    50   ~ 0
OT
NoConn ~ 950  3350
NoConn ~ 950  3450
NoConn ~ 950  3550
NoConn ~ 950  3650
NoConn ~ 3700 3550
NoConn ~ 3700 3650
NoConn ~ 3700 4550
Text Notes 2400 3750 1    50   ~ 0
ISP Header\nHere
Wire Wire Line
	5900 5150 6250 5150
$Comp
L Device:R R2
U 1 1 60316BAD
P 6250 5550
F 0 "R2" H 6320 5596 50  0000 L CNN
F 1 "2K" H 6320 5505 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 6180 5550 50  0001 C CNN
F 3 "~" H 6250 5550 50  0001 C CNN
	1    6250 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 5400 6250 5150
Connection ~ 6250 5150
Wire Wire Line
	6250 5150 7700 5150
$Comp
L power:GND #PWR02
U 1 1 60318B29
P 6250 5850
F 0 "#PWR02" H 6250 5600 50  0001 C CNN
F 1 "GND" H 6255 5677 50  0000 C CNN
F 2 "" H 6250 5850 50  0001 C CNN
F 3 "" H 6250 5850 50  0001 C CNN
	1    6250 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 5850 6250 5700
Wire Wire Line
	5850 4300 6250 4300
$Comp
L Device:R R1
U 1 1 60320A9D
P 6250 4550
F 0 "R1" H 6320 4596 50  0000 L CNN
F 1 "2K" H 6320 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 6180 4550 50  0001 C CNN
F 3 "~" H 6250 4550 50  0001 C CNN
	1    6250 4550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 60321026
P 6250 4800
F 0 "#PWR01" H 6250 4550 50  0001 C CNN
F 1 "GND" H 6255 4627 50  0000 C CNN
F 2 "" H 6250 4800 50  0001 C CNN
F 3 "" H 6250 4800 50  0001 C CNN
	1    6250 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 4800 6250 4700
Wire Wire Line
	6250 4400 6250 4300
Connection ~ 6250 4300
Wire Wire Line
	6250 4300 7700 4300
$Comp
L Device:R R5
U 1 1 6032892A
P 6750 3750
F 0 "R5" H 6820 3796 50  0000 L CNN
F 1 "2K" H 6820 3705 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 6680 3750 50  0001 C CNN
F 3 "~" H 6750 3750 50  0001 C CNN
	1    6750 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 60328BB5
P 6750 4000
F 0 "#PWR06" H 6750 3750 50  0001 C CNN
F 1 "GND" H 6755 3827 50  0000 C CNN
F 2 "" H 6750 4000 50  0001 C CNN
F 3 "" H 6750 4000 50  0001 C CNN
	1    6750 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 4000 6750 3900
Wire Wire Line
	6750 3600 6750 3500
Wire Wire Line
	6750 3500 7600 3500
$Comp
L power:GND #PWR05
U 1 1 6032BC25
P 6300 4000
F 0 "#PWR05" H 6300 3750 50  0001 C CNN
F 1 "GND" H 6305 3827 50  0000 C CNN
F 2 "" H 6300 4000 50  0001 C CNN
F 3 "" H 6300 4000 50  0001 C CNN
	1    6300 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 4000 6300 3400
Wire Wire Line
	6300 3400 7600 3400
Wire Wire Line
	5950 3300 5950 4050
Wire Wire Line
	5950 3300 7600 3300
Text Label 5950 4050 1    50   ~ 0
5V
Wire Wire Line
	5800 1050 7600 1050
Text Label 5800 1050 0    50   ~ 0
5V
Wire Wire Line
	5800 1950 7600 1950
Text Label 5800 1950 0    50   ~ 0
5V
Wire Wire Line
	6300 1150 7600 1150
$Comp
L Device:R R3
U 1 1 6033C0FA
P 6300 1400
F 0 "R3" H 6370 1446 50  0000 L CNN
F 1 "51K" H 6370 1355 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 6230 1400 50  0001 C CNN
F 3 "~" H 6300 1400 50  0001 C CNN
	1    6300 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 603417A1
P 6300 1650
F 0 "#PWR03" H 6300 1400 50  0001 C CNN
F 1 "GND" H 6305 1477 50  0000 C CNN
F 2 "" H 6300 1650 50  0001 C CNN
F 3 "" H 6300 1650 50  0001 C CNN
	1    6300 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 1650 6300 1550
Wire Wire Line
	6300 1250 6300 1150
$Comp
L Device:R R4
U 1 1 60352D13
P 6300 2350
F 0 "R4" H 6370 2396 50  0000 L CNN
F 1 "51K" H 6370 2305 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 6230 2350 50  0001 C CNN
F 3 "~" H 6300 2350 50  0001 C CNN
	1    6300 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 60352FDC
P 6300 2600
F 0 "#PWR04" H 6300 2350 50  0001 C CNN
F 1 "GND" H 6305 2427 50  0000 C CNN
F 2 "" H 6300 2600 50  0001 C CNN
F 3 "" H 6300 2600 50  0001 C CNN
	1    6300 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2050 6300 2200
Wire Wire Line
	6300 2050 7600 2050
Wire Wire Line
	6300 2500 6300 2600
NoConn ~ 3700 4650
Wire Wire Line
	6300 2050 5800 2050
Connection ~ 6300 2050
Text Label 5800 2050 0    50   ~ 0
OT
Wire Wire Line
	6300 1150 5800 1150
Connection ~ 6300 1150
Text Label 5800 1150 0    50   ~ 0
RT
Text Label 1200 4050 0    50   ~ 0
HS_INPUT
NoConn ~ 3700 3750
NoConn ~ 3700 3850
NoConn ~ 3700 4150
NoConn ~ 3700 4750
NoConn ~ 950  3950
NoConn ~ 950  4550
NoConn ~ 950  4750
NoConn ~ 3700 4250
Wire Wire Line
	2250 1700 2250 1750
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 60392B65
P 2800 1700
F 0 "#FLG0101" H 2800 1775 50  0001 C CNN
F 1 "PWR_FLAG" H 2800 1873 50  0000 C CNN
F 2 "" H 2800 1700 50  0001 C CNN
F 3 "~" H 2800 1700 50  0001 C CNN
	1    2800 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1750 2800 1750
Wire Wire Line
	2800 1750 2800 1700
Connection ~ 2250 1750
Wire Wire Line
	2250 1750 2250 2100
Text Label 1200 3950 0    50   ~ 0
OUT_RSV1
Text Notes 2000 3400 0    50   ~ 0
PD1
Text Notes 2000 3500 0    50   ~ 0
PD0
Text Notes 2000 3600 0    50   ~ 0
RST
Text Notes 2000 3800 0    50   ~ 0
PD2
Text Notes 2000 3900 0    50   ~ 0
PD3
Text Notes 2000 4000 0    50   ~ 0
PD4
Text Notes 2000 4100 0    50   ~ 0
PD5
Text Notes 2000 4200 0    50   ~ 0
PD6
Text Notes 2000 4300 0    50   ~ 0
PD7
Text Notes 2000 4400 0    50   ~ 0
PB0
Text Notes 2000 4500 0    50   ~ 0
PB1
Text Notes 2000 4600 0    50   ~ 0
PB2
Text Notes 2000 4800 0    50   ~ 0
PB4
Text Notes 2000 4700 0    50   ~ 0
PB3
Text Notes 2000 3700 0    50   ~ 0
GND
Text Notes 2500 3400 0    50   ~ 0
VIN
Text Notes 2500 3500 0    50   ~ 0
GND
Text Notes 2500 3600 0    50   ~ 0
RST
Text Notes 2500 3700 0    50   ~ 0
5V
Text Notes 2500 3800 0    50   ~ 0
A7
Text Notes 2500 3900 0    50   ~ 0
A6
Text Notes 2500 4000 0    50   ~ 0
PC5
Text Notes 2500 4100 0    50   ~ 0
PC4
Text Notes 2500 4200 0    50   ~ 0
PC3
Text Notes 2500 4300 0    50   ~ 0
PC2
Text Notes 2500 4400 0    50   ~ 0
PC1
Text Notes 2500 4500 0    50   ~ 0
PC0
Text Notes 2500 4600 0    50   ~ 0
AREF
Text Notes 2500 4700 0    50   ~ 0
3V3
Text Notes 2500 4800 0    50   ~ 0
PB5
Text Label 1200 4250 0    50   ~ 0
BTN1
Text Label 1200 4450 0    50   ~ 0
M_PWM
Text Label 1200 4550 0    50   ~ 0
PWM_RSV
Text Label 3250 4050 0    50   ~ 0
SDA
Text Label 3250 3950 0    50   ~ 0
SCL
$Comp
L Device:R R10
U 1 1 603B2AB1
P 4800 6400
F 0 "R10" H 4870 6446 50  0000 L CNN
F 1 "1K" H 4870 6355 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 4730 6400 50  0001 C CNN
F 3 "~" H 4800 6400 50  0001 C CNN
	1    4800 6400
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 6400 5200 6400
Text Label 5200 6400 0    50   ~ 0
5V
$Comp
L Device:R R11
U 1 1 603B6714
P 4800 6750
F 0 "R11" H 4870 6796 50  0000 L CNN
F 1 "1K" H 4870 6705 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 4730 6750 50  0001 C CNN
F 3 "~" H 4800 6750 50  0001 C CNN
	1    4800 6750
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 6750 5200 6750
Wire Wire Line
	5200 6750 5200 6400
Connection ~ 5200 6400
Wire Wire Line
	5200 6400 5500 6400
Text Label 1200 4350 0    50   ~ 0
BTN2
Text Label 1200 4650 0    50   ~ 0
BTN3
$Comp
L Connector_Generic:Conn_01x05 J10
U 1 1 603C54DD
P 2500 7250
F 0 "J10" V 2372 7530 50  0000 L CNN
F 1 "Buttons" V 2463 7530 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 2500 7250 50  0001 C CNN
F 3 "~" H 2500 7250 50  0001 C CNN
	1    2500 7250
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 7050 2400 6600
Wire Wire Line
	2500 7050 2500 6650
Wire Wire Line
	2600 7050 2600 6700
Wire Wire Line
	2700 7050 2700 6450
Text Label 2700 6850 1    50   ~ 0
GND
Text Label 2600 7000 1    50   ~ 0
BTN3
Text Label 2500 7000 1    50   ~ 0
BTN2
Text Label 2400 7000 1    50   ~ 0
BTN1
Text Label 2300 7000 1    50   ~ 0
BTN0
$Comp
L Device:R R9
U 1 1 603D03ED
P 2000 6050
F 0 "R9" H 1950 6300 50  0000 L CNN
F 1 "2K" H 1950 5800 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 1930 6050 50  0001 C CNN
F 3 "~" H 2000 6050 50  0001 C CNN
	1    2000 6050
	-1   0    0    1   
$EndComp
Wire Wire Line
	2300 7050 2300 6550
$Comp
L Device:R R8
U 1 1 603D7779
P 1850 6050
F 0 "R8" H 1800 6300 50  0000 L CNN
F 1 "2K" H 1800 5800 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 1780 6050 50  0001 C CNN
F 3 "~" H 1850 6050 50  0001 C CNN
	1    1850 6050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R7
U 1 1 603D7A12
P 1700 6050
F 0 "R7" H 1650 6300 50  0000 L CNN
F 1 "2K" H 1650 5800 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 1630 6050 50  0001 C CNN
F 3 "~" H 1700 6050 50  0001 C CNN
	1    1700 6050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R6
U 1 1 603D7C7E
P 1550 6050
F 0 "R6" H 1500 6300 50  0000 L CNN
F 1 "2K" H 1500 5800 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 1480 6050 50  0001 C CNN
F 3 "~" H 1550 6050 50  0001 C CNN
	1    1550 6050
	-1   0    0    1   
$EndComp
Wire Wire Line
	2300 6550 2000 6550
Wire Wire Line
	2000 6550 2000 6200
Connection ~ 2300 6550
Wire Wire Line
	2300 6550 2300 6450
Wire Wire Line
	1850 6200 1850 6600
Wire Wire Line
	1850 6600 2400 6600
Connection ~ 2400 6600
Wire Wire Line
	2400 6600 2400 6450
Wire Wire Line
	2500 6650 1700 6650
Wire Wire Line
	1700 6650 1700 6200
Connection ~ 2500 6650
Wire Wire Line
	2500 6650 2500 6450
Wire Wire Line
	2600 6700 1550 6700
Wire Wire Line
	1550 6700 1550 6200
Connection ~ 2600 6700
Wire Wire Line
	2600 6700 2600 6450
Wire Wire Line
	1550 5900 1550 5650
Wire Wire Line
	1700 5900 1700 5650
Wire Wire Line
	1700 5650 1550 5650
Connection ~ 1550 5650
Wire Wire Line
	1550 5650 1550 5350
Wire Wire Line
	1700 5650 1850 5650
Wire Wire Line
	1850 5650 1850 5900
Connection ~ 1700 5650
Wire Wire Line
	2000 5900 2000 5650
Wire Wire Line
	2000 5650 1850 5650
Connection ~ 1850 5650
Text Label 1550 5550 1    50   ~ 0
5V
$Comp
L Connector_Generic:Conn_01x04 J11
U 1 1 603F8D62
P 4300 7200
F 0 "J11" V 4172 7380 50  0000 L CNN
F 1 "I2C" V 4263 7380 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 4300 7200 50  0001 C CNN
F 3 "~" H 4300 7200 50  0001 C CNN
	1    4300 7200
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 7000 4400 6750
Wire Wire Line
	4300 7000 4300 6400
Wire Wire Line
	4200 7000 4200 6050
Wire Wire Line
	4100 7000 4100 6050
Wire Wire Line
	4650 6750 4400 6750
Connection ~ 4400 6750
Wire Wire Line
	4400 6750 4400 6050
Wire Wire Line
	4650 6400 4300 6400
Connection ~ 4300 6400
Wire Wire Line
	4300 6400 4300 6050
Text Label 4400 6250 1    50   ~ 0
SCL
Text Label 4300 6250 1    50   ~ 0
SDA
Text Label 4200 6250 1    50   ~ 0
GND
Text Label 4100 6250 1    50   ~ 0
5V
$EndSCHEMATC
