/*
   Constants.h
*/

#ifndef Constants_h
#define Constants_h
#include <LiquidCrystal.h>
// Pins

const int pressao_in = A1;

const int rotacao_in = 2;
const int saida_motor = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
const int rs = 8;
const int en = 9;
const int backlight = 10;
const int saida_solenoide = 11;
const int led = 12;
const int buzzer = 13;


// Constants

char Str1[ ] = "SOFTWARE BY";
char Str2[ ] = "JULIANO BARCIK";
const int FiO2_min = 21;
const int FiO2_max = 100;
const int ipm_min = 10;
const int ipm_max = 30;
const int volume_min = 50;
const int volume_max = 800;
const int alarme_max = 40;
const float ADC_mV = 4.700;       // convesion multiplier from Arduino ADC value to voltage in mV
const float SensorOffset = 180.0;     // in mV taken from datasheet
const float sensitivity = 4.413;      // in mV/mmH2O taken from datasheet
const float mmh2O_cmH2O = 10;         // divide by this figure to convert mmH2O to cmH2O
const float mmh2O_kpa = 0.00981;      // convesion multiplier from mmH2O to kPa
const float cmH2O_mmHg = 1.36;         // convesion division from cmH2O to mmHg
const float vt = 1600;
const float versao = 500.2;


#endif
