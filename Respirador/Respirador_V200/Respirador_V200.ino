






//RESPIRADOR V2 ARDUINO


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

//configura entradas
boolean flag_volta, flag_menu;
unsigned int pwm1 = 0;
int last_key, key, teste ;
unsigned int pressao, pwm, pwm2, pwm_min, pwm_max, pwm_out;
unsigned int rotacao;
const int rotacao_in = 2;


void setup() {
  // set up the LCD's number of columns and rows
  pinMode(rotacao_in, INPUT_PULLUP);     // sets the digital pin as input pullup
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("RESPIRADOR V300");
  lcd.setCursor(1, 1);
  lcd.print("APERTE SELECT");
  key = 0;
  delay(400);
  while (key != KEY_SELECT) {
    key = keypad.read_key();
  }
  menu_1();
}

void menu_1() {
  analogWrite(3, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("   <AJUSTE>     ");
  flag_menu =  HIGH;
  delay(300);
  key = 0;
  while (key != KEY_SELECT) {
    key = keypad.read_key();

    if (key == KEY_RIGHT) {
      lcd.setCursor(1, 1);
      lcd.print("  <AJUSTE>     ");
      flag_menu =  HIGH;
    }

    if (key == KEY_LEFT) {
      lcd.setCursor(1, 1);
      lcd.print("  <PACIENTE>   ");
      flag_menu =  LOW;
    }
  }
  if (flag_menu == 1) {
    ajuste();
  }
  else {
    seleciona();
  }
}

void atualiza() {

  delay(400);
  if (keypad.read_key() == KEY_SELECT) {

    menu_1();
  }
  if (keypad.read_key() == KEY_UP) {
    if (pwm < pwm_max)
      pwm ++;
  }
  if (keypad.read_key() == KEY_DOWN) {
    if (pwm > pwm_min)
      pwm --;
  }
  lcd.setCursor(0, 1);
  lcd.print(pwm_min);
  lcd.setCursor(2, 1);
  lcd.print("/");
  lcd.setCursor(3, 1);
  lcd.print(pwm_max);
  lcd.setCursor(10, 1);
  lcd.print (pwm);
  pwm1 = pwm;
  pwm_out = pwm1 * 6.38 + 26;
  analogWrite(3, pwm_out);
}

void ajuste() {
  analogWrite(3, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("   <saiu>    ");
}
void seleciona() {
  analogWrite(3, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  lcd.setCursor(1, 1);
  lcd.print("  <ADULTO>     ");
  pwm_min = 255 / 4 / 6;
  pwm_max = 255 / 4 / 3.1;
  pwm = pwm_min;
  delay(300);
  key = 0;
  while (key != KEY_SELECT) {
    key = keypad.read_key();
    if (key == KEY_RIGHT) {
      lcd.setCursor(1, 1);
      lcd.print("  <ADULTO>     ");
      pwm_min = 255 / 4 / 6;
      pwm_max = 255 / 4 / 3.1;
      pwm = pwm_min;
    }
    if (key == KEY_LEFT) {
      lcd.setCursor(1, 1);
      lcd.print("  <CRIANCA>   ");
      pwm_min = 255 / 4 / 3.1;
      pwm_max = 255 / 4 / 1.71;
      pwm = pwm_min;
    }
    if (key == KEY_DOWN) {
      lcd.setCursor(1, 1);
      lcd.print("  <PADRAO>    ");
      pwm_min = 255 / 4 / 6;
      pwm_max = 255 / 4 / 1.71;
      pwm = pwm_min;
    }
  }
  lcd.setCursor(0, 1);
  lcd.print("      || <  >IPM");
  digitalWrite(LED_BUILTIN, LOW);

}
void loop() {
  atualiza();
  Serial.println(pwm_out);
  Serial.println(pwm_min);
  Serial.println(pwm_max);
  Serial.println(pwm);
  Serial.println(pwm1);
  Serial.println(teste);
}
