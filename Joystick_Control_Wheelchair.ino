//Project name: Joystick control wheelchair using Arduino Board 

// Pin definitions
#define VRX A0 // Define Joystick for X-axis
#define VRY A1 // Define Joystick for Y-axis
#define enA 5
#define in1 6
#define in2 7
#define in3 8
#define in4 9
#define enB 10

int motorSpeed1 = 0;
int motorSpeed2 = 0;

void setup() {
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  int xAxis = analogRead(VRX); // Read joystick for X-axis
  int yAxis = analogRead(VRY); // Read joystick for Y-axis

  motorSpeed1 = 0;
  motorSpeed2 = 0;

  // Joystick values range from 0–1023 and 512 is the center.
  // Dead zone in center that Prevents motors from moving when joystick is near the center
  if (xAxis > 470 && xAxis < 550 && yAxis > 470 && yAxis < 550) {
    Stop();
  } 
  else if (xAxis > 470 && xAxis < 550) {
    // Left or Right turning
    if (yAxis < 470) {
      turnRight();
      int speed = map(yAxis, 470, 0, 0, 255);
      motorSpeed1 = speed;
      motorSpeed2 = speed;
    } 
    else if (yAxis > 550) {
      turnLeft();
      int speed = map(yAxis, 550, 1023, 0, 255);
      motorSpeed1 = speed;
      motorSpeed2 = speed;
    }
  } 
  else {
    // Forward or Backward and turning
    if (xAxis < 470) {
      forword();
      motorSpeed1 = map(xAxis, 470, 0, 0, 255);
      motorSpeed2 = motorSpeed1;
    } 
    else if (xAxis > 550) {
      backword();
      motorSpeed1 = map(xAxis, 550, 1023, 0, 255);
      motorSpeed2 = motorSpeed1;
    }

    // Fine steering adjustments with Y-axis
    if (yAxis < 470) {
      int yMapped = map(yAxis, 470, 0, 0, 255);
      motorSpeed1 += yMapped;
      motorSpeed2 -= yMapped;
    } 
    else if (yAxis > 550) {
      int yMapped = map(yAxis, 550, 1023, 0, 255);
      motorSpeed1 -= yMapped;
      motorSpeed2 += yMapped;
    }

    // Clamp motor speeds
    motorSpeed1 = constrain(motorSpeed1, 0, 255);
    motorSpeed2 = constrain(motorSpeed2, 0, 255);
  }

  // Prevent buzzing: ignore very low PWM values
  if (motorSpeed1 < 70) motorSpeed1 = 0;
  if (motorSpeed2 < 70) motorSpeed2 = 0;

  analogWrite(enA, motorSpeed1);
  analogWrite(enB, motorSpeed2);

  delay(10);
}

// Movement functions
void forword() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void backword() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}