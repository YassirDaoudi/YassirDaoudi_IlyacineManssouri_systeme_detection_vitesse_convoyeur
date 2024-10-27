#define PIN_TRIG 3
#define PIN_ECHO 2
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  lcd.begin(16, 2);
}

long getDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Calculate distance
  long duration = pulseIn(PIN_ECHO, HIGH);
  long distance = duration / 58;  // distance in cm
  return distance;
}

void loop() {
  static long previousDistance = 0;
  static long previousMillis = 0;
  
  long currentMillis = millis();
  long currentDistance = getDistance();
  
  // Calculate time and distance difference
  long deltaT = currentMillis - previousMillis;
  long deltaD = currentDistance - previousDistance;
  
  // Update previous values
  previousDistance = currentDistance;
  previousMillis = currentMillis;

  // Calculate speed in cm/s
  if (deltaT > 0) {
    float speed = (deltaD * 1000.0) / deltaT; // speed in cm/s
    Serial.print("Speed in cm/s: ");
    Serial.println(abs(speed));

    lcd.clear();
    lcd.print(abs(speed));
    lcd.print(" cm/s");

    // LED indicator for high speed
    if (abs(speed) > 50) {
      digitalWrite(13, HIGH);
      tone(4,500,500);
    } else {
      digitalWrite(13, LOW);
    }

  }

  delay(500);  // Delay for stability
}
