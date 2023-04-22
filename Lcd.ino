// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int current_slct = 0; 

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  analogWrite(6, 150); //contrast
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  current_slct = analogRead(A3);
  if((current_slct/520)>0) {
    lcd.print("Time Dripping:   ");
    lcd.setCursor(0, 1);
    lcd.print("100        ");
  }
  else {
    lcd.print("Water Used:   ");
    lcd.setCursor(0, 1);
    lcd.print("10        ");
  }
  
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
}

