int lights[2][3] = {{3,5,6}, {9,10,11}};

int happyMinimums[] = {75,95,128};
int happyMaximums[] = {150,190,255};
int sadMinimums[] = {0,0,128};
int sadMaximums[] = {20,20,255};
int testValues[] = {20,20,255};



long blinkTimer = 0;
long happyBlinkTime = 500;
long sadBlinkTime = 5000;
long sadTime = 60000;
long sadTimer = 0;
long fadeTimers[] = {0,0};
int activeColors[2][3] = {{0,0,0},{0,0,0}};
int nextBlinkIndex = 0;

long lastMillis = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(12, INPUT_PULLUP);
  setLight(0, testValues);
  randomSeed(analogRead(5));
  sadTimer = sadTime;
  lastMillis = millis();

  Serial.begin(9600);
}

void loop() {

  // put your main code here, to run repeatedly:
  long time = millis();
  long delta = time - lastMillis;

  sadTimer -= delta;
  sadTimer = max(sadTimer,0);
  blinkTimer -= delta;
  fadeTimers[0] -= delta;
  fadeTimers[0] = max(fadeTimers[0], 0);
  fadeTimers[1] -= delta;
  fadeTimers[1] = max(fadeTimers[1], 0);


  float happinessPercent = (float)sadTimer / sadTime;

  if (blinkTimer <= 0) {
    
    int happyColor[3];
    randomizeColor(happyMinimums, happyMaximums, happyColor);
    Serial.print("Chose happy color ");
    Serial.print(happyColor[0]);
    Serial.print(",");
    Serial.print(happyColor[1]);
    Serial.print(",");
    Serial.println(happyColor[2]);
    int sadColor[3];
    randomizeColor(sadMinimums, sadMaximums, sadColor);
    lerpColor(sadColor, happyColor, happinessPercent, activeColors[nextBlinkIndex]);

    setLight(nextBlinkIndex, activeColors[nextBlinkIndex]);


    nextBlinkIndex++;
    nextBlinkIndex %= sizeof(fadeTimers)/sizeof(long);
    blinkTimer = lerp(sadBlinkTime, happyBlinkTime, happinessPercent);
    Serial.println(blinkTimer);

  }

  if (digitalRead(12) == LOW) {
    sadTimer = sadTime;
    blinkTimer = 0;
    Serial.println("RESET");    
  }

  lastMillis = time;
}

void lerpColor(int* a, int* b, float t, int* dest) {
  for (int i = 0; i <= 2; i++) {
    dest[i] = lerp(a[i], b[i], t);
  }
}

int lerp(int a, int b, float t) {
  return (int)(a+(b-a)*t);
}

void randomizeColor(int* min, int* max, int* dest) {
  for (int i = 0; i <= 2; i++) {
    dest[i] = random(min[i], max[i]);
  }
}

void setLight(int index, int* color) {
  for (int i = 0; i <= 2; i++) {
    analogWrite(lights[index][i], color[i]);
  }
}
