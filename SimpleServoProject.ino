#include <Servo.h>

// =======================
// Servo objects
// =======================
Servo servos[3];

// =======================
// Current angles (start at 90° — centred)
// =======================
int angles[3] = {90, 90, 90};

// =======================
// Smoothness variables
// 0 = instant snap, 100 = ease over entire delay
// =======================
int smoothnessFast = 40;  // Servo 2
int smoothnessSlow = 70;  // Servo 0 & 1

// =======================
// Smooth eased move
// =======================
void moveServoSmooth(int idx, int delta, int totalDelay, int smoothPct) {
  int target = angles[idx] + delta;
  int from   = angles[idx];
  angles[idx] = target;

  long moveDuration = (long)totalDelay * smoothPct / 100;
  long holdDuration = totalDelay - moveDuration;

  int steps = max(1, (int)(moveDuration / 10)); // ~10ms per tick
  for (int s = 0; s <= steps; s++) {
    float t = (float)s / steps;
    // Ease in-out cubic
    float e = t < 0.5 ? 4*t*t*t : 1.0 - pow(-2*t + 2, 3) / 2.0;
    int   a = (int)(from + (target - from) * e);
    servos[idx].write(constrain(a, 0, 180));
    delay(moveDuration / steps);
  }

  if (holdDuration > 0) delay(holdDuration);

  Serial.print("Servo ");
  Serial.print(idx);
  Serial.print(" -> ");
  Serial.println(target);
}

// =======================
// Setup
// =======================
void setup() {
  Serial.begin(9600);

  servos[0].attach(9);
  servos[1].attach(10);
  servos[2].attach(11);

  for (int i = 0; i < 3; i++) {
    servos[i].write(90);
    delay(50);
  }

  Serial.println("Multi-servo controller ready");
}

// =======================
// Loop
// =======================
void loop() {
  delay(1500);

  moveServoSmooth(2, +25, 2000, smoothnessFast);  // Servo 2
  moveServoSmooth(2, -25, 2000, smoothnessFast);  // Servo 2

  // Move up
  moveServoSmooth(0, +25, 2000, smoothnessSlow);
  moveServoSmooth(1, -25, 2000, smoothnessSlow);
  moveServoSmooth(1, +25, 2000, smoothnessSlow);
  moveServoSmooth(1, +25, 2000, smoothnessSlow);
  moveServoSmooth(1, -25, 2000, smoothnessSlow);

  // Move Middle
  moveServoSmooth(0, -25, 2000, smoothnessSlow);
  moveServoSmooth(1, -25, 2000, smoothnessSlow);
  moveServoSmooth(1, +25, 2000, smoothnessSlow);
  moveServoSmooth(1, +25, 2000, smoothnessSlow);
  moveServoSmooth(1, -25, 2000, smoothnessSlow);

  // Move to bottom
  moveServoSmooth(0, -25, 2000, smoothnessSlow);
  moveServoSmooth(1, -25, 2000, smoothnessSlow);
  moveServoSmooth(1, +25, 2000, smoothnessSlow);
  moveServoSmooth(1, +25, 2000, smoothnessSlow);
  moveServoSmooth(1, -25, 2000, smoothnessSlow);

  // Move to middle
  moveServoSmooth(0, +25, 2000, smoothnessSlow);
}
