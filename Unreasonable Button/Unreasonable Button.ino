// include the library code:
#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <SPI.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define RST_PIN         8          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

const String VALID_ID = "438949E5";

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

enum STATE {
  OFF,
  ON,
  AUTH,
  SUCCESS,
  FAIL
};

STATE state = OFF;
STATE authGoal = ON;

long stateTime = 0;

void setup() {

  pinMode(9, INPUT_PULLUP);
  pinMode(1, OUTPUT);

  digitalWrite(1, LOW);  

  // Serial.begin(9600);


  // Serial.println("TEST");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
}

long lastMillis;

int buttonLastRecorded = LOW;

void loop() {

  unsigned long time = millis();
  int button = digitalRead(9);
  bool buttonJustPressed = (button == LOW && buttonLastRecorded == HIGH);
  
  stateTime += time - lastMillis;

  if (state == SUCCESS) Serial.println(state);
  
  if (state == OFF) {
      // Serial.println("OFF STATE REACHED");
      if (buttonJustPressed) {
        lcd.clear();
        lcd.print("Authentication");
        lcd.setCursor(0,1);
        lcd.print("Required");
        state = AUTH;
        authGoal = ON;
        stateTime = 0;
      }
  } else if (state == ON) {
      // Serial.println("ON STATE REACHED");
      if (buttonJustPressed) {
        lcd.clear();
        lcd.print("Authentication");
        lcd.setCursor(0,1);
        lcd.print("Required");
        state = AUTH;
        authGoal = OFF;
        stateTime = 0;
      }
  } else if (state == AUTH) {
      // Serial.println("AUTH STATE REACHED");
      String cardId = checkUid();
      if (!cardId.equalsIgnoreCase("")) {
      if (cardId.equalsIgnoreCase(VALID_ID)) {
        state = SUCCESS;
        stateTime = 0;
        Serial.println("SUCCESS STATE");
      } else {
        state = FAIL;
        stateTime = 0;
        Serial.println("FAIL STATE");
      }
      }
  } else if (state == SUCCESS) {
      // Serial.println("SUCCESS STATE REACHED");
      lcd.clear();
      lcd.print("Authorized");
      delay(2000);
      lcd.clear();
      if (authGoal == ON) {
        lcd.print("ACTIVATING LED");
      } else {
        lcd.print("DEACTIVATING LED");
      }
      lcd.setCursor(0,1);
      lcd.print("Stand Clear");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print(("                "));
      for (int i = 5; i >= 1; i--) {
        lcd.setCursor(0, 1);
        for (int j = 0; j < 5-i; j++) {
          lcd.print("  ");
        }
        lcd.print(i);
        delay(1000);
      }
      digitalWrite(1, authGoal == ON ? HIGH : LOW);
      lcd.clear();
      state = authGoal;
      stateTime = 0;
  } else if (state == FAIL) {
      // Serial.println("FAIL STATE REACHED");
      lcd.clear();
      lcd.print("AUTH FAILED");
      delay(3000);

      lcd.clear();
      lcd.print("LED Locked For");
      for (int i = 30; i >= 1; i--) {
        lcd.setCursor(0, 1);
        lcd.print("   ");
        lcd.setCursor(0, 1);
        lcd.print(i);
        delay(1000);
      }
      state = authGoal == ON ? OFF : ON;
      stateTime = 0;
      lcd.clear();
      
  } else {
      // Serial.println("DEFAULT FOR SOME REASON");
  }

  

  lastMillis = time;
  buttonLastRecorded = button;
}

String checkUid() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return "";
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return "";
	}

  String uid = String(mfrc522.uid.uidByte[0], HEX) + 
            String(mfrc522.uid.uidByte[1], HEX) + 
            String(mfrc522.uid.uidByte[2], HEX) + 
            String(mfrc522.uid.uidByte[3], HEX);
  return uid;
}