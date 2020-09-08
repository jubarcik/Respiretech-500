// include the library code:
#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>
#include "Constants.h"

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DFR_LCD_Keypad keypad(A0, &lcd);

// variaveis
byte int_fiO2, flag, last_key, key, in;
unsigned int pwm, pwm1 = 0, pwm_out, presao_min, pressao_max;
int value, pressao_kpa, pressao_cmH2O, old_value, ipm_out = 10, volume_out = 50, FiO2_out, volume_FiO2;
float ipm = 10.0, volume = 50.0, FiO2, volume1, altura_out, pressao, tempo1, tempo2;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  key = keypad.read_key();
  while (key == KEY_SELECT) {
    lcd.setCursor(0, 0);
    lcd.print(Str1);
    lcd.setCursor(2, 1);
    lcd.print(Str2);
    lcd.setCursor(13, 1);
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SOFTWARE:");
    lcd.setCursor(10, 0);
    lcd.print(versao);
    lcd.setCursor(13, 1);
    delay(3000);
    lcd.clear();
    break;
  }
  pinMode(rotacao_in, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(backlight, OUTPUT);
  pinMode(saida_solenoide, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(backlight, 0);
  delay(500);
  digitalWrite(backlight, 1);
  analogWrite(3, 0);
  lcd.setCursor(0, 0);
  lcd.print("RESPIRETECH V500");
  delay(2000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("AGUARDE");
  lcd.setCursor(1, 1);
  lcd.print("INICIALIZANDO");
  lcd.setCursor(13, 1);
  pwm_out = 100;
  calibracao();
}
void calibracao() {
  in = digitalRead(rotacao_in);
  analogWrite(saida_motor, pwm_out);
  while (in == 1) {
    in = digitalRead(rotacao_in);
  }
  analogWrite(saida_motor, 0);
  delay(300);
  analogWrite(saida_motor, 100);
  delay(300);
  analogWrite(saida_motor, 0);

  entrada_ipm();
}

void entrada_ipm() {
  detachInterrupt(digitalPinToInterrupt(rotacao_in));
  analogWrite(saida_motor, 0);
  ipm = 10.0, ipm_out = 10;
  delay(200);
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("SELECIONE BPM");
  lcd.setCursor(8, 1);
  lcd.print("BPM");
  while (key != KEY_SELECT) {
    key = keypad.read_key();
    lcd.setCursor(5, 1);
    lcd.print(ipm_out);
    pwm_out = ipm_out * 8 + 10;
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
  delay(200);
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  volume_out = 400.0;
  volume = 400.0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SELECIONE VOLUME");
  lcd.setCursor(8, 1);
  lcd.print("ml");
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
        volume += 20 ;
        delay(300);
        volume_out = volume;
        int(volume_out);
      }
      break;

    case KEY_DOWN:
      if (volume > volume_min) {
        volume -= 20;
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
  volume1 = volume;
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
    lcd.print ("ml");
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
    lcd.print ("ml");
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

  while (key != KEY_SELECT) {
    key = keypad.read_key();
  }
  flag = 1;
  entrada_FiO2();
}

void entrada_FiO2() {
  FiO2 = FiO2_min;
  FiO2_out = FiO2;
  delay(200);
  while (key != KEY_NONE) {
    key = keypad.read_key();
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SELECIONE FiO2");
  lcd.setCursor(8, 1);
  lcd.print("%");
  while (key != KEY_SELECT) {
    key = keypad.read_key();
    lcd.setCursor(5, 1);
    lcd.print(FiO2_out);
    seleciona_FiO2();
  }
}
void seleciona_FiO2() {
  key = keypad.read_key();

  switch (key) {
    case KEY_SELECT:
      delay(500);
      calcula_FiO2();
      break;

    case KEY_UP:
      if (FiO2 < FiO2_max ) {
        FiO2 += 1;
        delay(200);
        FiO2_out = FiO2;
      }
      break;

    case KEY_DOWN:
      if (FiO2 > FiO2_min) {
        FiO2 -= 1;
        delay(200);
        if (FiO2 < 100) {
          lcd.setCursor(7, 1);
          lcd.print(" ");
        }
        FiO2_out = FiO2;

      }
      break;
  }
}

void calcula_FiO2() {
  while (true) {
    tempo1 = 0;
    tempo2 = 0;
    in = 0;
    float vo2_1 = 0;
    float vo2_2 = 0;
    float t = 0;
    float perc = FiO2_out / 100.0 ;
    vo2_1 = vt * (perc - 0.21);
    t = vo2_1 / 333.0;
    tempo1 = t;

    vo2_2 = volume1 * (perc - 0.21);
    t = vo2_2 / 333.0;
    tempo2 = t;
    tempo1 = tempo1 * 1000;
    tempo2 = tempo2 * 1000;

    break;
  }
  inicializa_maquina();
}

void inicializa_maquina() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("AJUSTANDO FiO2");
  lcd.setCursor(4, 1);
  lcd.print("AGUARDE");
  delay(1000);
  digitalWrite (saida_solenoide, 1);
  delay(tempo1);
  digitalWrite (saida_solenoide, 0);
  attachInterrupt(digitalPinToInterrupt(rotacao_in), int_injecao_02 , FALLING);
  while (flag = 1) {
    tela1();
    break;
  }
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
    lcd.setCursor(9, 0);
    lcd.print(" ");
  }
  if (pressao_cmH2O < 100) {
    lcd.setCursor(10, 0);
    lcd.print(" ");
    pressao_cmH2O;
  }
}

void int_injecao_02() {
  int_fiO2 = 1;
}

void injecao_fiO2() {
  int_fiO2 = 0;
  digitalWrite (saida_solenoide, 1);
  delay(tempo2);
  digitalWrite (saida_solenoide, 0);
}

void atualiza_FiO2() {
  key = keypad.read_key();

  switch (key) {
    case KEY_UP:
      if (FiO2 < FiO2_max ) {
        FiO2 += 1;
        delay(100);
        FiO2_out = FiO2;
      }
      break;

    case KEY_DOWN:
      if (FiO2 > FiO2_min) {
        FiO2 -= 1;
        delay(100);
        if (FiO2 < 100) {
          lcd.setCursor(10, 1);
          lcd.print(" ");
        }
        FiO2_out = FiO2;
      }
      break;
  }
  realtime_FiO2();
}
void realtime_FiO2() {
  tempo1 = 0;
  tempo2 = 0;
  in = 0;
  float vo2_1 = 0;
  float vo2_2 = 0;
  float t = 0;
  float perc = FiO2_out / 100.0 ;
  vo2_1 = vt * (perc - 0.21);
  t = vo2_1 / 333.0;
  tempo1 = t;

  vo2_2 = volume1 * (perc - 0.21);
  t = vo2_2 / 333.0;
  tempo2 = t;
  tempo1 = tempo1 * 1000;
  tempo2 = tempo2 * 1000;
}

void tela1() {
  flag = 0;
  lcd.clear();
  lcd.setCursor(11, 0);
  lcd.print ("cmH2O");
  lcd.setCursor(3, 0);
  lcd.print ("BPM");
  lcd.setCursor(4, 1);
  lcd.print ("ml");
  lcd.setCursor(11, 1);
  lcd.print ("%FiO2");
}

void loop() {
  while (key != KEY_NONE) {
    key = keypad.read_key();
    delay(100);
  }
  key = keypad.read_key();
  switch (key) {
    case KEY_SELECT:
      delay(100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PARANDO MAQUINA");
      lcd.setCursor(4, 1);
      lcd.print("AGUARDE");
      calibracao();
      break;
  }
  analogWrite(saida_motor, pwm_out);
  switch (int_fiO2) {
    case 1:
      injecao_fiO2();
      break;
  }
  atualiza_FiO2();
  converte_pressao();
  lcd.setCursor(8, 0);
  lcd.print(pressao_cmH2O);
  lcd.setCursor(0, 0);
  lcd.print(ipm_out);
  lcd.setCursor(0, 1);
  lcd.print(volume_out);
  lcd.setCursor(8, 1);
  lcd.print(FiO2_out);

  Serial.print("BPM = ");
  Serial.println(ipm_out);
  Serial.print("PRESSÃO = ");
  Serial.print(pressao_cmH2O); Serial.println("cmH2O");
  Serial.print("VOLUME = ");
  Serial.print(volume_out); Serial.println("ml");
  Serial.print("FiO2 = ");
  Serial.print(FiO2_out); Serial.println("%");

  while (pressao_cmH2O >= alarme_max) {
    digitalWrite (buzzer, 1);
    delay(1000);
    digitalWrite (buzzer, 0);
    break;
  }
}
