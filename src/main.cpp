#include <Arduino.h>

const int MOTOR_DIR = 5;
const int MOTOR_PWM = 6;
const int ENCODER_PIN = 4;

String inputString = "";

// PID variables
float targetAngle = 0;
float currentAngle = 0;

float Kp = 2.0;
float Ki = 0.0;
float Kd = 0.1;

float error = 0;
float prevError = 0;
float integral = 0;

unsigned long lastTime = 0;
float getAngle();
void setMotor(int speed, bool forwardDirection);
void readSerial();
// ================= ENCODER =================
float getAngle() {
  // Read pulse widths (microseconds)
  uint32_t highTime = pulseIn(ENCODER_PIN, HIGH, 30000);
  uint32_t lowTime  = pulseIn(ENCODER_PIN, LOW, 30000);

  if (highTime == 0 || lowTime == 0) return NAN;

  float duty = (float)highTime / (highTime + lowTime);
  duty = constrain(duty, 0.0, 1.0);

  float angle = duty * 360.0;

  // Clamp to [0,360)
  if (angle >= 360.0) angle = fmod(angle, 360.0);
  if (angle < 0) angle = 0;

  return angle;
}

// ================= SERIAL INPUT =================
void readSerial() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      float newTarget = inputString.toFloat();

      // Wrap to 0–360
      newTarget = fmod(newTarget, 360.0);
      if (newTarget < 0) newTarget += 360.0;

      targetAngle = newTarget;
      integral = 0;

      Serial.print("New target: ");
      Serial.println(targetAngle);

      inputString = "";
    } else {
      inputString += c;
    }
  }
}

// ================= MOTOR =================
void setMotor(int speed, bool forwardDirection) {
  digitalWrite(MOTOR_DIR, forwardDirection ? HIGH : LOW);
  analogWrite(MOTOR_PWM, speed);
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(ENCODER_PIN, INPUT);

  delay(500);

  targetAngle = getAngle();
  if (isnan(targetAngle)) targetAngle = 0;

  Serial.print("Initial target: ");
  Serial.println(targetAngle);

  lastTime = millis();
}


// ================= MAIN LOOP =================
void loop() {
  readSerial();

  currentAngle = getAngle();

  if (isnan(currentAngle)) {
    Serial.println("Bad encoder reading");
    delay(20);
    return;
  }

  // Time step
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  if (dt <= 0) dt = 0.001;
  lastTime = now;

  // Wrap-around error (-180 to 180)
  error = targetAngle - currentAngle;
  if (error > 180) error -= 360;
  if (error < -180) error += 360;

  // PID
  integral += error * dt;
  integral = constrain(integral, -100, 100);

  float derivative = (error - prevError) / dt;
  float output = Kp * error + Ki * integral + Kd * derivative;

  prevError = error;

  // Motor command
  int pwm = constrain(abs(output), 0, 255);

  // Deadband
  if (pwm < 25) pwm = 0;

  bool forward = (output >= 0);
  setMotor(pwm, forward);

  // Debug
  Serial.print("Target: ");
  Serial.print(targetAngle, 2);
  Serial.print(" Current: ");
  Serial.print(currentAngle, 2);
  Serial.print(" Error: ");
  Serial.print(error, 2);
  Serial.print(" PWM: ");
  Serial.println(pwm);

  delay(20); // ~50 Hz loop
}
