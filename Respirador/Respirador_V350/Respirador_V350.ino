//RESPIRADOR V350 ARDUINO


// include the library code:
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DFR_LCD_Keypad keypad(A0, &lcd);

//variaveis
int inPin = 2, buttonState ;   // pushbutton connected to digital pin 7
byte  menu = 1, menu_min = 1, menu_max = 2;
byte  seleciona = 1, seleciona_min = 1, seleciona_max = 3;
byte last_key, key ;
int  pressao_mmHg, pressao_kpa, pressao_cmH2O;
unsigned int  pressao, presao_min, pressao_max;
unsigned int pwm, pwm1 = 0, pwm2, pwm_min, pwm_max, pwm_out;
float timer1, timer2, timer3, timer4, timer5, tempo1;

int old_value;
int maxval = 0;
int minval;
int value;

// constantes
const float ADC_mV = 4.700;       // convesion multiplier from Arduino ADC value to voltage in mV
const float SensorOffset = 180.0;     // in mV taken from datasheet
const float sensitivity = 4.413;      // in mV/mmH2O taken from datasheet
const float mmh2O_cmH2O = 10;         // divide by this figure to convert mmH2O to cmH2O
const float mmh2O_kpa = 0.00981;      // convesion multiplier from mmH2O to kPa
const float cmH2O_mmHg = 1.36;         // convesion division from cmH2O to mmHg


void setup() {
  // set up the LCD's number of columns and rows
  pinMode(inPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("RESPIRADOR V350");
  lcd.setCursor(1, 1);
  lcd.print("APERTE SELECT");

  while (key != KEY_SELECT) {
    key = keypad.read_key();
  }
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  lcd.clear();
  menu_1();
}
void menu_1() {
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  analogWrite(3, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  lcd.clear();
  while (key != KEY_SELECT) {
    key = keypad.read_key();

    if (keypad.read_key() == KEY_LEFT) {
      if (menu > menu_min)
        menu--;
      delay(500);
    }
    if (keypad.read_key() == KEY_RIGHT) {
      if (menu < menu_max)
        menu ++;
      delay(500);
    }
    switch (menu) {
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("   <PACIENTE>  >");
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("<   <TESTE>     ");
        break;
    }
  }
  switch (menu) {
    case 1:
      seleciona_paciente();
      break;
    case 2:
      teste();
      break;
  }
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
}

void atualiza() {
  key = keypad.read_key();

  switch (key) {
    case KEY_SELECT:
      menu_1();
      break;

    case KEY_UP:
      if (pwm < pwm_max) {
        pwm ++;
        delay(300);
      }
      break;

    case KEY_DOWN:
      if (pwm > pwm_min) {
        pwm --;
        delay(300);
      }
      break;
  }
  delay(300);
  converte_pressao();
  lcd.setCursor(0, 0);
  lcd.print("    ");
  lcd.setCursor(1, 0);
  lcd.print ("0");
  lcd.setCursor(5, 0);
  lcd.print("     ");
  lcd.setCursor(7, 0);
  lcd.print (pressao_cmH2O);
  lcd.setCursor(13, 0);
  lcd.print (pwm);
  pwm1 = pwm;
  pwm_out = pwm1 * 8 + 10;
  analogWrite(3, pwm_out);
}
void seleciona_paciente() {
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  analogWrite(3, 0);
  digitalWrite(LED_BUILTIN, HIGH);

  while (key != KEY_SELECT) {
    key = keypad.read_key();

    if (keypad.read_key() == KEY_LEFT) {
      if (seleciona > seleciona_min)
        seleciona--;
      delay(500);
    }
    if (keypad.read_key() == KEY_RIGHT) {
      if (seleciona < seleciona_max)
        seleciona ++;
      delay(500);
    }
    switch (seleciona) {
      case 1:
        pwm = 0;
        lcd.setCursor(0, 0);
        lcd.print("    <PADRAO>   >");
        lcd.setCursor(0, 1);
        lcd.print("   <10/36 IPM>  ");
        pwm_min = 255 / 4 / 6;
        pwm_max = 255 / 4 / 1.71;
        pwm = pwm_min;
        break;
      case 2:
        pwm = 0;
        lcd.setCursor(0, 0);
        lcd.print("<   <ADULTO>   >");
        lcd.setCursor(0, 1);
        lcd.print("   <10/20 IPM>  ");
        pwm_min = 255 / 4 / 6;
        pwm_max = 255 / 4 / 3.1;
        pwm = pwm_min;
        break;
      case 3:
        pwm = 0;
        lcd.setCursor(0, 0);
        lcd.print("<   <CRIANCA>   ");
        lcd.setCursor(0, 1);
        lcd.print("   <15/36 IPM>  ");
        pwm_min = 255 / 4 / 4.2;
        pwm_max = 255 / 4 / 1.71;
        pwm = pwm_min;
        break;
    }
   
  }
  
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("ml/s");
  lcd.setCursor(11, 0);
  lcd.print("|");
  lcd.setCursor(11, 1);
  lcd.print("|");
  //lcd.setCursor(12, 0);
  //lcd.print("<  >");
  lcd.setCursor(12, 1);
  lcd.print("IPM");

  lcd.setCursor(4, 0);
  lcd.write("|");
  lcd.setCursor(4, 1);
  lcd.print("|");
  lcd.setCursor(5, 1);
  lcd.print("cmH2O");




  digitalWrite(LED_BUILTIN, LOW);
}

void atualiza_teste() {

  key = keypad.read_key();

  switch (key) {
    case KEY_UP:
      if (pwm < pwm_max) {
        pwm ++;
        delay(300);
      }
      break;

    case KEY_DOWN:
      if (pwm > pwm_min) {
        pwm --;
        delay(300);
      }
      break;
  }
  delay(300);
  lcd.setCursor(6, 1);
  lcd.print (pwm);
  pwm1 = pwm;
  pwm_out = pwm1 * 8 + 10;

}





void teste() {
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  timer1, timer2, timer3 = 0;
  pwm_min = 255 / 4 / 6;
  pwm_max = 255 / 4 / 1.71;
  pwm = pwm_min;

  analogWrite(3, 0);
  digitalWrite(LED_BUILTIN, LOW);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("INICIAR TESTE");
  lcd.setCursor(1, 1);
  lcd.print("APERTE SELECT");
  while (key != KEY_SELECT) {
    key = keypad.read_key();
  }
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("SELECIONE IPM");

  while (key != KEY_SELECT) {
    key = keypad.read_key();
    atualiza_teste();
  }
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }

  analogWrite(3, pwm_out);
  lcd.clear();
  lcd.setCursor(13, 1);
  lcd.print("IPM");
  lcd.setCursor(11, 1);
  lcd.print(pwm);
  lcd.setCursor(11, 0);
  lcd.print("TEMPO");
  lcd.setCursor(1, 0);
  lcd.print(timer3);
  while (key != KEY_SELECT) {

    while (buttonState == HIGH) {
      buttonState = digitalRead(inPin);
      key = keypad.read_key();
      while (key == KEY_SELECT) {
        menu_1();
      }
      buttonState = digitalRead(inPin);
    }
    timer1 = 0;
    timer1 = millis();

    while (buttonState == LOW) {
      buttonState = digitalRead(inPin);
    }

    while (buttonState == HIGH) {
      buttonState = digitalRead(inPin);
      key = keypad.read_key();
      while (key == KEY_SELECT) {
        menu_1();
      }
      buttonState = digitalRead(inPin);
    }

    while (buttonState == LOW) {
      buttonState = digitalRead(inPin);
    }
    timer2 = millis();
    timer3 = timer2 - timer1;
    Serial.print("TEMPO = ");
    Serial.println(timer3);
    lcd.setCursor(1, 0);
    lcd.print(timer3);
    key = keypad.read_key();
    while (key == KEY_SELECT) {
      menu_1();
    }
  }

}
int calcula_volume(int ipm, int volume) {
  float vps = 0.042 * ipm - 0.316;
  float tempo = (volume / vps);
  return tempo1 = tempo;
}
void converte_pressao() {
  pressao_cmH2O = (analogRead(A1) * ADC_mV - SensorOffset) / sensitivity / mmh2O_cmH2O;// result in cmH2O
  int old_pressao ;
  value = pressao_cmH2O; // hypothetical code reads a sensor
  while (value > old_pressao) {
    pressao_max = value;
    old_pressao = pressao_max;
  }

}

void loop() {
  atualiza();
  calcula_volume(pwm1, 500);
  Serial.print("PWM = ");
  Serial.println(pwm_out);
  Serial.print("PWM min = ");
  Serial.println(pwm_min);
  Serial.print("PWM max = ");
  Serial.println(pwm_max);
  Serial.print("ENTRADA PWM = ");
  Serial.println(pwm1);
  Serial.print("MENU = ");
  Serial.println(menu);
  Serial.print("BOTÃO= ");
  Serial.println(key);
  Serial.print("SUB MENU = ");
  Serial.println(seleciona);
  Serial.print("SENSOR PRESSÃO = ");
  Serial.println(pressao_cmH2O);
  Serial.print("TEMPO = ");
  Serial.println(tempo1);
  Serial.print("PRESSAO MAX = ");
  Serial.println(pressao_max);
}
