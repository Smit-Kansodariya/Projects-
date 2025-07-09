# Embedded Projects

This repository contains a collection of three embedded systems projects that demonstrate real-world applications using microcontrollers, sensors, and actuators. Each project was developed in C/C++ using Arduino platform, and showcases integration of hardware components with embedded software logic.

## Projects Overview

### 1. Joystick Controlled Wheelchair

**Description**:  
An assistive mobility solution that allows a user to control a wheelchair using a joystick. It includes obstacle detection using ultrasonic sensors for enhanced safety.

**Features**:
- Manual control using a joystick
- Obstacle avoidance via ultrasonic sensors
- Bidirectional motor control

**Technologies and Components**:
- Language: C / C++
- Platform: AVR Controller
- Input: Joystick module
- Sensors: Ultrasonic sensor
- Actuation: L298N Motor Driver

### 2. RFID-Based Door Lock System

**Description**:  
A secure and automated door locking system that authenticates users through RFID tags. Upon successful authentication, a solenoid motor unlocks the door. Status is displayed via an LCD.

**Features**:
- RFID authentication
- Real-time status display on LCD
- Electric solenoid-based lock actuation

**Technologies and Components**:
- Language: C / C++
- Platform: Arduino Uno
- Modules: RFID Reader (MFRC522), Solenoid Motor
- Display: LiquidCrystal I2C
- Libraries Used:
  ```cpp
  #include <Wire.h>
  #include <SPI.h>
  #include <MFRC522.h>
  #include <LiquidCrystal_I2C.h>


### 3. Huskylense Controlled Autonomous Drilling Robot

**Description**:  
An intelligent robotic drilling system that uses computer vision (Huskylens) to autonomously detect targets and execute precise drilling actions using actuators.

**Features**:
- Target detection via Huskylens AI Vision Sensor
- Automated drilling using actuator and servo based robotic arm mechanisms
- Controlled via Arduino Mega for higher I/O capacity

**Technologies and Components**:
- Language: C / C++
- Platform: Arduino Mega
- Sensors: Huskylens AI Vision Sensor, Ultrasonic sensor

Actuation: Motor Driver, Servo Motor, Drilling Actuator
- Libraries Used: 
  ```cpp
  #include <HUSKYLENS.h>
  #include <Servo.h>

## How to Run the code: 
- Clone the repository
- Open the desired project in Arduino IDE
- Install necessary libraries from the Library Manager (for Arduino)
- Connect the hardware as per the schematic
- Upload the code and power on the system

