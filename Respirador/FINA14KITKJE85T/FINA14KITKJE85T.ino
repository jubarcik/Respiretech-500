#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>

LiquidCrystal lcd(8,9,4,5,6,7); 
DFR_LCD_Keypad keypad(A0, &lcd);

int led=13;
int last_key, key;

void setup()
{
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("PRESS KEYS NOW");
  pinMode(led, OUTPUT);
  delay(1000);
  lcd.clear();
}

void loop()
{
  last_key = keypad.get_last_key();
  key      = keypad.read_key();
  

  if (key != last_key) {
    
    lcd.clear();
    lcd.setCursor(0,0);
    
    switch (key) {
      case KEY_RIGHT:
        lcd.print("RIGHT");
        break;
        
      case KEY_UP:
        lcd.print("UP");
        break;
        
      case KEY_DOWN:
        lcd.print("DOWN");
        break;
        
      case KEY_LEFT:
        lcd.print("LEFT");
        break;

      case KEY_SELECT:
        lcd.print("SELECT");
        digitalWrite(led, HIGH);
        break;
        
      case KEY_NONE:
      default:
        lcd.print("NO KEYS PRESSED");
        lcd.setCursor(0,1);
        lcd.print("PRESS KEYS NOW");
        digitalWrite(led, LOW);
        break;
    }
  }
  delay(10); 
}
