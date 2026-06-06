// --- Pin Definitions ---
const int potPin = A0;
const int in1 = 6;
const int in2 = 7;
const int ena = 9;

const int stepPin = 4;
const int dirPin = 3;

const int togglePin = 2;        // Stepper direction toggle
const int limitSwitch = 10;     // Both switches on D10

// --- Variables ---
bool gearDirection = true;
bool lastLimitState = HIGH;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(togglePin, INPUT_PULLUP);
  pinMode(limitSwitch, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin);
  int pwmValue = map(potValue, 0, 1023, 0, 90);
  int stepDelay = map(potValue, 0, 1023, 1000, 1);  // 0 = slow, 1023 = fast

  // --- Stop both motors if pot is at min ---
  if (potValue < 50) {
    analogWrite(ena, 0);
  } else {
    analogWrite(ena, pwmValue);
  }

  // --- Gear Motor Direction Control ---
  if (digitalRead(limitSwitch) == LOW && lastLimitState == HIGH) {
    gearDirection = !gearDirection;
    delay(300); // Debounce delay
  }
  lastLimitState = digitalRead(limitSwitch);

  digitalWrite(in1, gearDirection ? HIGH : LOW);
  digitalWrite(in2, gearDirection ? LOW : HIGH);

  // --- Stepper Direction from Toggle Switch ---
  digitalWrite(dirPin, digitalRead(togglePin));

  // --- Stepper Control ---
  if (potValue >= 50) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);
  }
}
