// include the library code:
#include <LiquidCrystal.h>
#include <SPI.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
const int LED1 = 2, LED2 = 13;
const int BUZZER = 3;
const int TILT = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {


  // Serial.begin(9600);


  // Serial.println("TEST");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // lcd.print("TEST");
  // digitalWrite(LED1, HIGH);
  // digitalWrite(LED2, HIGH);
  pinMode(TILT, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}
void loop() {
  if (digitalRead(TILT) == HIGH) { // Inactive
    // digitalWrite(BUZZER, LOW);
    noTone(BUZZER);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    lcd.clear();
    lcd.print("    INACTIVE    ");
    lcd.setCursor(0, 1);
    lcd.print("TILT TO ACTIVATE");
    delay(800);
  } else { // Active
    lcd.clear();
    lcd.print("SMOG DETECTED");
    lcd.setCursor(0, 1);
    lcd.print("LEVELS EXCEEDED");

    // analogWrite(BUZZER, 512);
    // digitalWrite(BUZZER, LOW);
    tone(BUZZER, 300);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);

    delay(400);

    // analogWrite(BUZZER, 256);
    // digitalWrite(BUZZER, HIGH);
    tone(BUZZER, 500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);

    delay(400);
  }
}