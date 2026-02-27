#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

// =======================
// PCA9685 setup
// =======================
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo pulse range (adjust if needed)
#define SERVOMIN 120
#define SERVOMAX 600

// =======================
// Current angles for 3 logical axis groups (X, Y, Z starts at 70)
// =======================
int angles[3] = {90, 90, 70};

// Pin mappings for pairs (AxisIdx -> PinA, PinB)
const int clusterA[3] = {0, 4, 8};
const int clusterB[3] = {1, 5, 9};
const int clusterC[3] = {2, 6, 10};
const int clusterD[3] = {3, 7, 11};
// const int clusterA[3] = {0, 6, 10};
// const int clusterB[3] = {1, 7, 11};

// =======================
// Smoothness
// =======================
int smoothnessFast = 90;
int smoothnessSlow = 90;

// =======================
// Convert angle → PWM
// =======================
uint16_t angleToPulse(int angle) {
  return map(angle, 0, 270, SERVOMIN, SERVOMAX);
}

// =======================
// Write servo via I2C (Dual Pins)
// =======================
void writeServoPair(int axisIdx, int angle) {
  angle = constrain(angle, 0, 270);
  uint16_t pulse = angleToPulse(angle);

  // Write to both pins in the cluster
  pwm.setPWM(clusterA[axisIdx], 0, pulse);
  pwm.setPWM(clusterB[axisIdx], 0, pulse);
  pwm.setPWM(clusterC[axisIdx], 0, pulse);
  pwm.setPWM(clusterD[axisIdx], 0, pulse);
}

// =======================
// Smooth eased move (Absolute Position)
// =======================
void moveServoSmooth(int axisIdx, int target, int totalDelay, int smoothPct, bool skipHold = false) {
  target = constrain(target, 0, 270);
  int from = angles[axisIdx];
  angles[axisIdx] = target;

  long moveDuration = (long)totalDelay * smoothPct / 100;
  long holdDuration = totalDelay - moveDuration;

  int steps = max(1, (int)(moveDuration / 10));

  for (int s = 0; s <= steps; s++) {
    float t = (float)s / steps;
    float e = t < 0.5 ? 4 * t * t * t : 1.0 - pow(-2 * t + 2, 3) / 2.0;
    int a = (int)(from + (target - from) * e);

    writeServoPair(axisIdx, a);
    delay(moveDuration / steps);
  }

  if (!skipHold && holdDuration > 0)
  delay(holdDuration);

  Serial.print("Axis ");
  Serial.print(axisIdx);
  Serial.print(" moved to ");
  Serial.println(target);
}

// =======================
// Setup
// =======================
void setup() {
  Serial.begin(9600);

  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50); // servo frequency

  delay(500);

  // Center all 6 servos (3 pairs)
  // Axis 0 & 1 -> 90, Axis 2 -> 70
  // moveServoSmooth(0, 90, 500, 100);
  // moveServoSmooth(1, 90, 500, 100);
  // moveServoSmooth(2, 145, 500, 100);

  Serial.println("6-Servo System Ready (Absolute Mode)");
}

// =======================
// Loop (Absolute Positions)
// =======================
void loop() {
  delay(1000);

  moveServoSmooth(2, 0, 1900, 100, true);
  moveServoSmooth(2, 230, 1250, smoothnessFast);

  moveServoSmooth(0, 220, 600, smoothnessSlow);
  moveServoSmooth(1, 70, 600, smoothnessSlow);
  moveServoSmooth(1, 120, 600, smoothnessSlow);
  moveServoSmooth(1, 170, 600, smoothnessSlow);
  moveServoSmooth(1, 120, 600, smoothnessSlow);

  moveServoSmooth(0, 160, 600, smoothnessSlow);
  moveServoSmooth(1, 70, 600, smoothnessSlow);
  moveServoSmooth(1, 120, 600, smoothnessSlow);
  moveServoSmooth(1, 170, 600, smoothnessSlow);
  moveServoSmooth(1, 120, 600, smoothnessSlow);

  moveServoSmooth(0, 130, 750, smoothnessSlow);
  moveServoSmooth(1, 70, 600, smoothnessSlow);
  moveServoSmooth(1, 120, 600, smoothnessSlow);
  moveServoSmooth(1, 170, 600, smoothnessSlow);
  moveServoSmooth(1, 120, 600, smoothnessSlow);

  moveServoSmooth(0, 160, 600, smoothnessSlow);


  



  // moveServoSmooth(0, 160, 1000, smoothnessSlow);
  // moveServoSmooth(1, 120, 1000, smoothnessSlow);

  // These two lines alligns eyeball to absolute centers uncomment them and comment rest of the function calls to first establish centre then proceed further.

  
}
