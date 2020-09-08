// include the library code:

#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>
#include "Constants.h"
#include "Logging.h"
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DFR_LCD_Keypad keypad(A0, &lcd);

// variaveis

byte last_key, key ;
unsigned int  presao_min, pressao_max;
unsigned int pwm, pwm1 = 0, pwm_out;
int _mmHg, pressao_kpa, pressao_cmH2O;
int old_value;
int value;
int ipm_out = 10, volume_out = 50, peep_out = 5.0;
float ipm = 10.0, volume = 50.0, peep = 5.0;
float altura_out ;
float  pressao;

void setup() {
  // set up the LCD's number of columns and rows
  pinMode(rotacao_in, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  analogWrite(3, 0);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("RESPIRADOR V500");
  delay(2000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("AGUARDE");
  lcd.setCursor(0, 1);
  lcd.print("INICIALIZANDO");
  lcd.setCursor(13, 1);
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.setCursor(14, 1);
  lcd.print(".");
  delay(1000);
  lcd.setCursor(15, 1);
  lcd.print(".");
  delay(1000);
  lcd.clear();
  entrada_ipm();
}

void entrada_ipm() {
  delay(200);
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  analogWrite(saida_motor, 0);
  ipm = 10.0, ipm_out = 10;
  volume = 50.0, volume_out = 50;
  lcd.setCursor(1, 0);
  lcd.print("SELECIONE IPM");
  lcd.setCursor(1, 1);
  lcd.print("APERTE SELECT");

  while (key != KEY_SELECT) {
    key = keypad.read_key();
  }
  delay(200);
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SELECIONE IPM");
  lcd.setCursor(8, 1);
  lcd.print("IPM");
  while (key != KEY_SELECT) {
    key = keypad.read_key();
    lcd.setCursor(5, 1);
    lcd.print(ipm_out);
    seleciona_ipm();
  }
}

void seleciona_ipm() {
  key = keypad.read_key();

  switch (key) {
    case KEY_SELECT:
      delay(100);
      entrada_volume();
      break;

    case KEY_UP:
      if (ipm < ipm_max) {
        ipm ++;
        delay(300);
        ipm_out = ipm;
        int(ipm_out);
      }
      break;

    case KEY_DOWN:
      if (ipm > ipm_min) {
        ipm --;
        delay(300);
        ipm_out = ipm;
        int(ipm_out);
      }
      break;
  }
}

void  entrada_volume() {
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("SELECIONE VOLUME");
  lcd.setCursor(1, 1);
  lcd.print("APERTE SELECT");

  while (key != KEY_SELECT) {
    key = keypad.read_key();
  }
  delay(200);
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  volume_out = 50.0;
  volume = 50.0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SELECIONE VOLUME");
  lcd.setCursor(8, 1);
  lcd.print("ml/s");
  while (key != KEY_SELECT) {
    key = keypad.read_key();
    lcd.setCursor(5, 1);
    lcd.print(volume_out);
    seleciona_volume();
  }
}

void seleciona_volume() {
  key = keypad.read_key();

  switch (key) {
    case KEY_SELECT:
      delay(100);
      calcula_altura();
      break;

    case KEY_UP:
      if (volume < volume_max) {
        volume += 50 ;
        delay(300);
        volume_out = volume;
        int(volume_out);
      }
      break;

    case KEY_DOWN:
      if (volume > volume_min) {
        volume -= 50;
        delay(300);
        if (volume < 100) {
          lcd.setCursor(7, 1);
          lcd.print(" ");
        }
        volume_out = volume;
        int(volume_out);
      }
      break;
  }
}

  void calcula_altura() {

    while (key != KEY_NONE) {
      key = keypad.read_key();
    }
    altura_out = 0.0;
    volume = volume / 1000;
    float r = 0.09;
    float f = (ipm / 60.0);
    float w = (3.1416 * 2.0 * f);
    float vps = 0.042 * ipm - 0.316;
    float v = (w * r);
    float vpp = (volume / ipm);
    float tempo = volume / vps;
    float fi = w * tempo;
    float s = (r * fi);
    float altura = (s - 0.144) / 0.021 + 0.01;
    altura_out = altura;

    while (altura_out < -0.1) {
      lcd.clear();
      lcd.setCursor(0, 0.0);
      lcd.print("SELECIONE VOLUME");
      lcd.setCursor(1, 1);
      lcd.print ("MAIOR >");
      lcd.setCursor(12, 1);
      lcd.print ("ml/s");
      lcd.setCursor(9, 1);
      lcd.print (volume_out);
      delay(2000);
      lcd.clear();
      entrada_volume();
    }
    while (altura_out > 6.1) {
      lcd.clear();
      lcd.setCursor(0, 0.0);
      lcd.print("SELECIONE VOLUME");
      lcd.setCursor(1, 1);
      lcd.print ("MENOR < ");
      lcd.setCursor(12, 1);
      lcd.print ("ml/s");
      lcd.setCursor(9, 1);
      lcd.print (volume_out);
      delay(2000);
      lcd.clear();
      entrada_volume();
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AJUSTE ALTURA EM");
    lcd.setCursor(5, 1);
    lcd.print ( altura_out);

    key = keypad.read_key();
    while (key != KEY_SELECT ) {
      key = keypad.read_key();
      delay(100);
    }
    lcd.clear();
  }

  void converte_pressao() {
    pressao_cmH2O = (analogRead(A1) * ADC_mV - SensorOffset) / sensitivity / mmh2O_cmH2O;// result in cmH2O
    int old_pressao ;
    value = pressao_cmH2O; // hypothetical code reads a sensor
    while (value > old_pressao) {
      pressao_max = value;
      old_pressao = pressao_max;
    }
    if (pressao_cmH2O < 10) {
      lcd.setCursor(1, 0);
      lcd.print(" ");
    }
    if (pressao_cmH2O < 100) {
      lcd.setCursor(2, 0);
      lcd.print(" ");
      pressao_cmH2O;
    }
  }

  void loop() {
    // put your main code here, to run repeatedly:
    while (key != KEY_NONE) {
      key = keypad.read_key();
      delay(100);
    }
    key = keypad.read_key();
    switch (key) {
      case KEY_SELECT:
        delay(100);
        lcd.clear();
        entrada_ipm();
        break;
    }
    delay(100);
    converte_pressao();
    lcd.setCursor(0, 0);
    lcd.print(pressao_cmH2O);
    lcd.setCursor(4, 0);
    lcd.print ("cmH2O");
    lcd.setCursor(10, 0);
    lcd.print(ipm_out);
    lcd.setCursor(13, 0);
    lcd.print ("IPM");
    lcd.setCursor(0, 1);
    lcd.print(volume_out);
    lcd.setCursor(4, 1);
    lcd.print ("ml/s");
    lcd.setCursor(9, 1);
    lcd.print(altura_out);
    lcd.setCursor(13, 1);
    lcd.print ("Alt");
    pwm_out = ipm_out * 8 + 10;
    analogWrite(saida_motor, pwm_out);
  }
