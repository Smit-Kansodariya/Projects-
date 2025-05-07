#include <HUSKYLENS.h>    // Include the Huskylens library for object tracking
#include <Servo.h>         // Include the Servo library for controlling servos

// Motor Pins
#define IN1   39    // Front Left Motor control pin
#define IN2   37    // Front Left Motor control pin
#define IN3   45    // Front Right Motor control pin
#define IN4   47    // Front Right Motor control pin
#define IN5   35    // Back Left Motor control pin
#define IN6   33    // Back Left Motor control pin
#define IN7   49    // Back Right Motor control pin
#define IN8   51    // Back Right Motor control pin

#define LENA  41    // PWM Pin for Front Left Motor speed control
#define RENA  43    // PWM Pin for Front Right Motor speed control
#define LENB  31    // PWM Pin for Back Left Motor speed control
#define RENB  53    // PWM Pin for Back Right Motor speed control

// Servo Pins for Arm Movement
#define SERVO_PIN_0 11   // Servo 0 control pin
#define SERVO_PIN_1 10   // Servo 1 control pin
#define SERVO_PIN_2 9    // Servo 2 control pin

// Ultrasonic Sensor Pins
#define TRIG_PIN 6  // Trigger pin for ultrasonic sensor
#define ECHO_PIN 5  // Echo pin for ultrasonic sensor

// Drilling Motor Pin
#define DRILL_PIN 12  // Pin for controlling drilling motor

// Create Servo objects for controlling the robotic arm
Servo myServo0, myServo1, myServo2;

// Huskylens setup
HUSKYLENS huskylens;                  // Initialize Huskylens object
HardwareSerial& huskySerial = Serial1; // Communication with Huskylens via Serial1

// Motor speeds variables (PWM values)
int speedFL = 255, speedFR = 255, speedBL = 255, speedBR = 255; // Default motor speeds (full speed)

// Setup function
void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  huskySerial.begin(9600);  // Initialize communication with Huskylens via Serial1

  // Initialize Huskylens
  if (!huskylens.begin(huskySerial)) {  // Check if Huskylens is initialized successfully
    Serial.println("Huskylens Initialization Failed");  // If failed, display error and stop
    while (1);  // Halt the program
  }
  
  // Initialize motor pins as OUTPUT
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  pinMode(LENA, OUTPUT);
  pinMode(RENA, OUTPUT);
  pinMode(LENB, OUTPUT);
  pinMode(RENB, OUTPUT);
  pinMode(DRILL_PIN, OUTPUT);  // Set drilling motor pin as OUTPUT

  // Initialize servo motors for arm
  myServo0.attach(SERVO_PIN_0);
  myServo1.attach(SERVO_PIN_1);
  myServo2.attach(SERVO_PIN_2);

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// Main loop
void loop() {
  // Move servos to initial positions for arm control
  myServo0.write(100);  // Position servo 0 at base position 
  myServo1.write(60);   // Position servo 1
  myServo2.write(130);  // Position servo 2

  // Measure the distance using ultrasonic sensor
  long distance = measure_distance();  
  if (distance < 40) {  // If obstacle detected (less than 40 cm)
    // Check if Huskylens detects an object
    if (huskylens.request()) {
      if (huskylens.isLearned()) {  // Check if target is learned by Huskylens
        HUSKYLENSResult result = huskylens.read();  // Read the result from Huskylens
        if (result.ID == 1) {  // Assuming object ID 1 is the target (e.g., red block)
          go_stop();  // Stop robot movement when target is detected
          // Adjust robot movement towards the target
          if ((80 < result.xCenter) && (result.xCenter < 240)) {
            go_stop();  // Stop robot movement when target is centered
          }
          else if ((240 - result.xCenter) > 0) {
            turn_left();  // Turn left if the target is on the right side of the robot
          }
          else if ((80 - result.xCenter) < 0) {
            turn_right();  // Turn right if the target is on the left side of the robot
          }
          // Adjust arm position based on object location
          myServo0.write(100);  // Position servo 0 at base position 
          myServo1.write(110);  // Position servo 1
          myServo2.write(160);  // Position servo 2
          delay(2000);  // Wait for servo movement to complete
          digitalWrite(DRILL_PIN, HIGH);  // Activate drilling motor
          delay(3000);  // Wait for 3 seconds to simulate drilling
          digitalWrite(DRILL_PIN, LOW);  // Deactivate drilling motor
          printResult(result);  // Output result to Serial monitor
        }
        delay(500);  // Short delay before next check
        if (result.ID != 1){  // If the detected object is not the target
          turn_right();  // Turn the robot to the right
        }
      }
    }
  }
  else {
    go_forward();  // Move forward if no obstacle detected
  }
}

// Function to move the robot forward
void go_forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);

  analogWrite(LENA, speedFL);  // Set speed for front left motor
  analogWrite(RENA, speedFR);  // Set speed for front right motor
  analogWrite(LENB, speedBL);  // Set speed for back left motor
  analogWrite(RENB, speedBR);  // Set speed for back right motor
}

// Function to move the robot backward
void go_backward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);

  analogWrite(LENA, speedFL);
  analogWrite(RENA, speedFR);
  analogWrite(LENB, speedBL);
  analogWrite(RENB, speedBR);
}

// Function to stop the robot
void go_stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);

  analogWrite(LENA, 0);  // Stop front left motor
  analogWrite(RENA, 0);  // Stop front right motor
  analogWrite(LENB, 0);  // Stop back left motor
  analogWrite(RENB, 0);  // Stop back right motor
}

// Function to turn the robot to the right
void turn_right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);

  analogWrite(LENA, speedFL);
  analogWrite(RENA, speedFR);
  analogWrite(LENB, speedBL);
  analogWrite(RENB, speedBR);
}

// Function to turn the robot to the left
void turn_left() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);

  analogWrite(LENA, speedFL);
  analogWrite(RENA, speedFR);
  analogWrite(LENB, speedBL);
  analogWrite(RENB, speedBR);
}

// Function to measure the distance using the ultrasonic sensor
long measure_distance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);  // Measure pulse duration
  long distance = duration * 0.034 / 2;  // Calculate distance in cm
  return distance;  // Return the distance
}

// Function to print the result from Huskylens
void printResult(HUSKYLENSResult result) {
    if (result.command == COMMAND_RETURN_BLOCK) {
        Serial.println(String() + F("Block: xCenter=") + result.xCenter + F(", yCenter=") + result.yCenter +
                       F(", width=") + result.width + F(", height=") + result.height + F(", ID=") + result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW) {
        Serial.println(String() + F("Arrow: xOrigin=") + result.xOrigin + F(", yOrigin=") + result.yOrigin +
                       F(", xTarget=") + result.xTarget + F(", yTarget=") + result.yTarget + F(", ID=") + result.ID);
    }
    else {
        Serial.println("Object unknown!");  // Handle unknown object case
    }
}
