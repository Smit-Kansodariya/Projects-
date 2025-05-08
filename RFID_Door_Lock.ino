// Project Title: Smart RFID Door Access System using Arduino & RC522

#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcdDisplay(0x27, 16, 2); // 16 columns and 2 rows

// Pin definitions
#define RFID_SS 10
#define RFID_RST 9
#define GREEN_LED 4
#define RED_LED 5
#define RELAY_PIN 3
#define manualOverride 6

// Delay constants
#define ACCESS_DURATION 3000
#define BLINK_DELAY 500

MFRC522 rfid(RFID_SS, RFID_RST);

byte authorizedUID[4] = {0x8A, 0x7E, 0x55, 0xC4};  // Change this to your card/tag UID

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(manualOverride, INPUT);

  lcdDisplay.init();
  lcdDisplay.backlight();
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("Tap RFID to Unlock");
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print("Status: Locked");

  Serial.println("System Ready. Awaiting RFID...");
}

void loop() {
  // Manual button unlock
  if (digitalRead(manualOverride) == HIGH) {
    grantAccess();
  }

  // Wait for card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Show UID
  Serial.print("Scanned UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Compare UID
  if (memcmp(rfid.uid.uidByte, authorizedUID, 4) == 0) {
    grantAccess();
  } else {
    Serial.println("Unauthorized Card!");
    lcdDisplay.setCursor(0, 1);
    lcdDisplay.print(" Access Denied  ");
    for (int i = 0; i < 3; i++) {
      digitalWrite(RED_LED, HIGH);
      delay(BLINK_DELAY);
      digitalWrite(RED_LED, LOW);
      delay(100);
    }
    lcdDisplay.setCursor(0, 1);
    lcdDisplay.print("Status: Locked  ");
  }

  delay(100);
}

void grantAccess() {
  Serial.println("Access Approved");
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RELAY_PIN, HIGH);
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print("Access Granted  ");
  delay(ACCESS_DURATION);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print("Status: Locked  ");
}
