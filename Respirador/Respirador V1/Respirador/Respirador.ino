


/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 7
   LCD Enable pin to digital pin 6
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe
  modified 7 Nov 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <Adafruit_LiquidCrystal.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
Adafruit_LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned int Pot_in = A1;    // select the input pin for the potentiometer
unsigned int Press_in = A2;
unsigned int Press_value;
unsigned int Pot_value;
int pulse = A3;
int reset = A4;
unsigned int velocidade = 0;

void setup() {
  // set up the LCD's number of columns and rows
  pinMode(pulse, INPUT);      // sets the digital pin as output
  lcd.begin(16, 2); // Print a message to the
  lcd.clear;
  digitalWrite(10, HIGH);
  lcd.setCursor(1, 0);
  lcd.print("RESPIRADOR V1");
  Pot_value = analogRead(Pot_in);
  while (Pot_value != 0) {
    lcd.setCursor(1, 1);
    lcd.print("REGULE IPM A 0");
    Pot_value = analogRead(Pot_in);
  }
  lcd.setCursor(0, 1);
  lcd.print("         %IPM  ");
}
void atualizadisplay() {
  velocidade = Pot_value / 4;
  lcd.setCursor(0, 1);
  lcd.print("         ");
  lcd.setCursor(6, 1);
  lcd.print(velocidade = velocidade / 2.55);


}
void atualizapwm() {
  Pot_value = analogRead(Pot_in);
  analogWrite(11, Pot_value / 4);
}
void testepulso() {
  switch (digitalRead(pulse) == HIGH) { // sensor rotação esta em low?
    case 0:
      digitalWrite(0, LOW);     // indicate via LED
      delay(50);                   // del
      break;
    case 1:
      while (digitalRead (reset) == HIGH) {
        digitalWrite(0, HIGH);     // indicate via LED
        delay(500);                   // delay
        analogWrite(11, 0);
        lcd.setCursor(0,1);
        lcd.print("MOTOR DESLIGADO");
      }
      lcd.setCursor(0, 1);
      lcd.print("         %IPM  ");
      break;

  }
}
void loop() {

  testepulso();
  atualizapwm();
  atualizadisplay();


}
