/**
   Constants.h
   Defines all the global constants used in `e-vent.ino`.
*/

#ifndef Constants_h
#define Constants_h

// States
enum States {
  DEBUG_STATE,       // 0
  IN_STATE,          // 1
  HOLD_IN_STATE,     // 2
  EX_STATE,          // 3
  PEEP_PAUSE_STATE,  // 4
  HOLD_EX_STATE,     // 5
  PREHOME_STATE,     // 6
  HOMING_STATE,      // 7
  OFF_STATE          // 8
};

// Timing Settings
const float LOOP_PERIOD = 0.03;       // The period (s) of the control loop
const float HOLD_IN_DURATION = 0.1;  // Duration (s) to pause after inhalation
const float MIN_PEEP_PAUSE = 0.05;    // Time (s) to pause after exhalation / before watching for an assisted inhalation
const float MAX_EX_DURATION = 1.00;   // Maximum exhale duration (s)


// Pins

const int pressao_in= A1;
const int beep_in = 11;
const int led_alarm = 12;
const int rotacao_in = 2; 
const int saida_motor =3;
const int rs = 8;
const int en = 9;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
const int ipm_min = 10;
const int ipm_max = 30;
const int volume_min = 50;
const int volume_max = 800;
const int peep_min = 5;
const int peep_max = 40;
const float ADC_mV = 4.700;       // convesion multiplier from Arduino ADC value to voltage in mV
const float SensorOffset = 180.0;     // in mV taken from datasheet
const float sensitivity = 4.413;      // in mV/mmH2O taken from datasheet
const float mmh2O_cmH2O = 10;         // divide by this figure to convert mmH2O to cmH2O
const float mmh2O_kpa = 0.00981;      // convesion multiplier from mmH2O to kPa
const float cmH2O_mmHg = 1.36;         // convesion division from cmH2O to mmHg


// Control knob mappings
const int BPM_MIN = 10;
const int BPM_MAX = 35;
const int BPM_RES = 1;
const float IE_MIN = 1;
const float IE_MAX = 4;
const float IE_RES = 0.1;
const int VOL_MIN = 100;
const int VOL_MAX = 800;
const int VOL_RES = 25;
const float AC_MIN = 2;
const float AC_MAX = 5;
const float AC_RES = 0.1;
const int ANALOG_PIN_MAX = 1023; // The maximum count on analog pins



// Safety settings
const float MAX_PRESSURE = 40.0;        // Trigger high pressure alarm
const float MIN_PLATEAU_PRESSURE = 5.0; // Trigger low pressure alarm
const float MAX_RESIST_PRESSURE = 2.0;  // Trigger high-resistance notification
const float MIN_TIDAL_PRESSURE = 5.0;   // Trigger no-tidal-pressure alarm
const float VOLUME_ERROR_THRESH = 50.0; // Trigger incomplete breath alarm




#endif
