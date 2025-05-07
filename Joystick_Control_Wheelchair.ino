// Project Title: Arduino-Based Wheelchair Control Using Joystick Module

// === Pin Assignments ===
#define JOY_X A0       // Joystick horizontal axis
#define JOY_Y A1       // Joystick vertical axis

#define MOTOR_A_PWM 5
#define MOTOR_A_IN1 6
#define MOTOR_A_IN2 7

#define MOTOR_B_IN1 8
#define MOTOR_B_IN2 9
#define MOTOR_B_PWM 10

// === Movement Constants ===
const int DEAD_ZONE = 40;     // Neutral zone threshold around center (512)
const int PWM_THRESHOLD = 70; // Minimum speed to trigger motor movement

int speedLeft = 0;
int speedRight = 0;

// === Movement Types Enum ===
enum MotionType { FORWARD, BACKWARD, TURN_LEFT, TURN_RIGHT };

void setup() {
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);

  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_B_PWM, OUTPUT);

  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_B_IN1, OUTPUT);
  pinMode(MOTOR_B_IN2, OUTPUT);
}

void loop() {
  int joyX = analogRead(JOY_X);  // Read horizontal input
  int joyY = analogRead(JOY_Y);  // Read vertical input

  // Reset speeds before logic
  speedLeft = 0;
  speedRight = 0;

  // Center check for stop (includes dead zone)
  if (abs(joyX - 512) < DEAD_ZONE && abs(joyY - 512) < DEAD_ZONE) {
    holdMotors();
  } else {
    computeMotorSpeeds(joyX, joyY);

    // Apply minimum threshold to avoid motor jitter
    if (speedLeft < PWM_THRESHOLD) speedLeft = 0;
    if (speedRight < PWM_THRESHOLD) speedRight = 0;

    analogWrite(MOTOR_A_PWM, speedLeft);
    analogWrite(MOTOR_B_PWM, speedRight);
  }

  delay(10);
}

// === Motion Logic ===
void computeMotorSpeeds(int x, int y) {
  if (abs(x - 512) < DEAD_ZONE) {
    // Turning only (Y-axis movement)
    if (y < 512 - DEAD_ZONE) {
      activateMotors(TURN_RIGHT);
      int mappedSpeed = map(y, 472, 0, 0, 255);
      speedLeft = mappedSpeed;
      speedRight = mappedSpeed;
    } else if (y > 512 + DEAD_ZONE) {
      activateMotors(TURN_LEFT);
      int mappedSpeed = map(y, 552, 1023, 0, 255);
      speedLeft = mappedSpeed;
      speedRight = mappedSpeed;
    }
  } else {
    // Forward or backward
    if (x < 512 - DEAD_ZONE) {
      activateMotors(FORWARD);
      speedLeft = map(x, 472, 0, 0, 255);
      speedRight = speedLeft;
    } else if (x > 512 + DEAD_ZONE) {
      activateMotors(BACKWARD);
      speedLeft = map(x, 552, 1023, 0, 255);
      speedRight = speedLeft;
    }

    // Fine steering using Y-axis
    if (y < 512 - DEAD_ZONE) {
      int adjust = map(y, 472, 0, 0, 255);
      speedLeft += adjust;
      speedRight -= adjust;
    } else if (y > 512 + DEAD_ZONE) {
      int adjust = map(y, 552, 1023, 0, 255);
      speedLeft -= adjust;
      speedRight += adjust;
    }

    speedLeft = constrain(speedLeft, 0, 255);
    speedRight = constrain(speedRight, 0, 255);
  }
}


void activateMotors(MotionType motion) {
  switch (motion) {
    case FORWARD:
      digitalWrite(MOTOR_A_IN1, HIGH);
      digitalWrite(MOTOR_A_IN2, LOW);
      digitalWrite(MOTOR_B_IN1, LOW);
      digitalWrite(MOTOR_B_IN2, HIGH);
      break;
    case BACKWARD:
      digitalWrite(MOTOR_A_IN1, LOW);
      digitalWrite(MOTOR_A_IN2, HIGH);
      digitalWrite(MOTOR_B_IN1, HIGH);
      digitalWrite(MOTOR_B_IN2, LOW);
      break;
    case TURN_LEFT:
      digitalWrite(MOTOR_A_IN1, LOW);
      digitalWrite(MOTOR_A_IN2, HIGH);
      digitalWrite(MOTOR_B_IN1, LOW);
      digitalWrite(MOTOR_B_IN2, HIGH);
      break;
    case TURN_RIGHT:
      digitalWrite(MOTOR_A_IN1, HIGH);
      digitalWrite(MOTOR_A_IN2, LOW);
      digitalWrite(MOTOR_B_IN1, HIGH);
      digitalWrite(MOTOR_B_IN2, LOW);
      break;
  }
}

void holdMotors() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, LOW);
}
