#include <Servo.h>
// #include <Adafruit_LSM303DLH_Mag.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>

// Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

#define LDR_LEFT A0
#define LDR_RIGHT A1

Servo myServo;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(6);
  Serial.begin(9600);

  pinMode(LDR_LEFT, INPUT);
  pinMode(LDR_RIGHT, INPUT);

  // if (!mag.begin()) {
  //   /* There was a problem detecting the LSM303 ... check your connections */
  //   // Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  //   while (1)
  //     ;
  // }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int angle = 0;

#define SENSITIVITY 5
#define BIAS -60

void loop() {
  // put your main code here, to run repeatedly:
  int left = analogRead(LDR_LEFT);
  int right = analogRead(LDR_RIGHT);

  float result = (float)(left-right-BIAS)/(float)(right+left-abs(BIAS)) * SENSITIVITY;
  result = mapfloat(result, -1, 1, 0, 180);

  // sensors_event_t event;
  // mag.getEvent(&event);

  // float Pi = 3.14159;

  // // Calculate the angle of the vector y,x
  // float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / Pi;

  // // Normalize to 0-360
  // if (heading < 0) {
  //   heading = 360 + heading;
  // }

  // event.

  // if (heading > 179) heading = 179;

  // myServo.write((int)heading);

  static int mult = 1;

  angle += 5 * mult;
  if (angle == 180 || angle == 0) mult *= -1;
  myServo.write((int)result);
  // digitalWrite(13, HIGH);
  // delay(2);
  // digitalWrite(13, LOW);

  Serial.print(" Left: ");
  Serial.print(left);
  Serial.print(" Right: ");
  Serial.print(right);

  Serial.println();

  delay(250);
}
