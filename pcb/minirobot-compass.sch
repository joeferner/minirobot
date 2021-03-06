EESchema Schematic File Version 2
LIBS:minirobot
LIBS:minirobot-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
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
L ST-LIS3MDL U6
U 1 1 56A3F454
P 5700 3500
F 0 "U6" H 5700 4387 60  0000 C CNN
F 1 "ST-LIS3MDL" H 5700 4281 60  0000 C CNN
F 2 "kicadlib:LGA12S50P4X4_192X192X100" H 4350 3000 60  0001 C CNN
F 3 "" H 4350 3000 60  0000 C CNN
F 4 "636" H 5700 3500 60  0001 C CNN "inventoryId"
	1    5700 3500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR29
U 1 1 56A3F45B
P 5100 2800
F 0 "#PWR29" H 5100 2890 20  0001 C CNN
F 1 "+3.3V" H 5093 2929 30  0000 C CNN
F 2 "" H 5100 2800 60  0000 C CNN
F 3 "" H 5100 2800 60  0000 C CNN
	1    5100 2800
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR30
U 1 1 56A3F461
P 5100 4200
F 0 "#PWR30" H 5100 4250 40  0001 C CNN
F 1 "GNDPWR" H 5100 4120 40  0001 C CNN
F 2 "" H 5100 4200 60  0000 C CNN
F 3 "" H 5100 4200 60  0000 C CNN
	1    5100 4200
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 56A3F46B
P 6300 3800
F 0 "C6" H 6378 3831 30  0000 L CNN
F 1 "0.1uF" H 6378 3770 30  0000 L CNN
F 2 "kicadlib:CAPC2012X100N" H 6300 3800 60  0001 C CNN
F 3 "" H 6300 3800 60  0000 C CNN
F 4 "10" H 6300 3800 60  0001 C CNN "inventoryId"
	1    6300 3800
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR33
U 1 1 56A3F472
P 6300 4200
F 0 "#PWR33" H 6300 4250 40  0001 C CNN
F 1 "GNDPWR" H 6300 4120 40  0001 C CNN
F 2 "" H 6300 4200 60  0000 C CNN
F 3 "" H 6300 4200 60  0000 C CNN
	1    6300 4200
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 56A3F478
P 3600 3100
F 0 "C1" H 3678 3131 30  0000 L CNN
F 1 "0.1uF" H 3678 3070 30  0000 L CNN
F 2 "kicadlib:CAPC2012X100N" H 3600 3100 60  0001 C CNN
F 3 "" H 3600 3100 60  0000 C CNN
F 4 "10" H 3600 3100 60  0001 C CNN "inventoryId"
	1    3600 3100
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 56A3F47F
P 3900 3100
F 0 "C3" H 3978 3131 30  0000 L CNN
F 1 "1uF" H 3978 3070 30  0000 L CNN
F 2 "kicadlib:CAPC2012X100N" H 3900 3100 60  0001 C CNN
F 3 "" H 3900 3100 60  0000 C CNN
F 4 "13" H 3900 3100 60  0001 C CNN "inventoryId"
	1    3900 3100
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR16
U 1 1 56A3F486
P 3900 3400
F 0 "#PWR16" H 3900 3450 40  0001 C CNN
F 1 "GNDPWR" H 3900 3320 40  0001 C CNN
F 2 "" H 3900 3400 60  0000 C CNN
F 3 "" H 3900 3400 60  0000 C CNN
	1    3900 3400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR15
U 1 1 56A3F48C
P 3900 2800
F 0 "#PWR15" H 3900 2890 20  0001 C CNN
F 1 "+3.3V" H 3893 2929 30  0000 C CNN
F 2 "" H 3900 2800 60  0000 C CNN
F 3 "" H 3900 2800 60  0000 C CNN
	1    3900 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3000 5200 3000
Wire Wire Line
	5100 2800 5100 3500
Wire Wire Line
	5200 2900 5100 2900
Connection ~ 5100 2900
Wire Wire Line
	5100 3400 5200 3400
Connection ~ 5100 3000
Wire Wire Line
	5100 3500 5200 3500
Connection ~ 5100 3400
Wire Wire Line
	5200 3900 5100 3900
Wire Wire Line
	5100 3900 5100 4200
Wire Wire Line
	5200 4000 5100 4000
Connection ~ 5100 4000
Wire Wire Line
	5200 4100 5100 4100
Connection ~ 5100 4100
Wire Wire Line
	6200 3600 6300 3600
Wire Wire Line
	6300 3600 6300 3700
Wire Wire Line
	6300 3900 6300 4200
Wire Wire Line
	3600 3200 3600 3300
Wire Wire Line
	3600 3300 3900 3300
Wire Wire Line
	3900 3200 3900 3400
Connection ~ 3900 3300
Wire Wire Line
	3900 2800 3900 3000
Wire Wire Line
	3600 3000 3600 2900
Wire Wire Line
	3600 2900 3900 2900
Connection ~ 3900 2900
Text HLabel 4800 3200 0    60   Input ~ 0
I2CSCL
Text HLabel 4800 3300 0    60   BiDi ~ 0
I2CSDA
Text HLabel 4800 3600 0    60   Output ~ 0
COMPASSINT
Text HLabel 4800 3700 0    60   Output ~ 0
COMPASSRDY
Wire Wire Line
	5200 3200 4800 3200
Wire Wire Line
	4800 3300 5200 3300
Wire Wire Line
	5200 3600 4800 3600
Wire Wire Line
	4800 3700 5200 3700
$EndSCHEMATC
