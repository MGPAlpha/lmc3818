int columns[] = {A2, 3, 4, 11, 6, 10, A4, A5};
int rows[] = {12, A3, 8, 9, 1, 7, 2, 5};

int matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int cursor[] = {0,0};

int lastJoystick[] = {0,0};

// int joystickTimer = 0;
long lastInputChange = millis();

void setup() {
  // put your setup code here, to run once:
  pinMode(13, INPUT_PULLUP);
  // Serial.begin(9600);

  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    pinMode(columns[i], OUTPUT);
    digitalWrite(rows[i], LOW);
    digitalWrite(columns[i], HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(13) == LOW) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        matrix[i][j] = 0;
      }
    }
    // cursor[0] = 0;
    // cursor[1] = 0;
    lastJoystick[0] = 0;
    lastJoystick[1] = 0;
    lastInputChange = millis();

    // Serial.println("Vib detected");
  }

  // Serial.print("Joystick pos: ");
  // Serial.print(lastJoystick[0]);
  // Serial.print(", ");
  // Serial.print(lastJoystick[1]);
  // Serial.println();
  delay(1);

  int rawJoystick[] = {analogRead(A0), analogRead(A1)};

  int currJoystick[] = {
    (rawJoystick[0] < 100) ? -1 : ((rawJoystick[0] > 923) ? 1 : 0),
    (rawJoystick[1] < 100) ? -1 : ((rawJoystick[1] > 923) ? 1 : 0)
  };

  if (currJoystick[0] != lastJoystick[0] || currJoystick[1] != lastJoystick[1]) {
    lastInputChange = millis();
  }

  if (millis() - lastInputChange > 300) {
    lastInputChange = millis();
    cursor[0] += currJoystick[0];
    cursor[1] += currJoystick[1];
    cursor[0] = min(7, max(0, cursor[0]));
    cursor[1] = min(7, max(0, cursor[1]));
  }

  matrix[cursor[1]][cursor[0]] = 1;

  lastJoystick[0] = currJoystick[0];
  lastJoystick[1] = currJoystick[1];
  

  for (int i = 0; i < 8; i++) {
    digitalWrite(columns[i], LOW);

    for (int j = 0; j < 8; j++) {
      if (j == cursor[1] && i == cursor[0]) {
      } else {
        digitalWrite(rows[j], matrix[j][i]);
      }
    }
    for (int j = 0; j < 8; j++) {
      digitalWrite(rows[j], LOW);
    }

    digitalWrite(columns[i], HIGH);
  }

  if (millis() % 400 < 300) {


    digitalWrite(columns[cursor[0]], LOW);
    digitalWrite(rows[cursor[1]], HIGH);
    digitalWrite(columns[cursor[0]], HIGH);
    digitalWrite(rows[cursor[1]], LOW);
  }


}

