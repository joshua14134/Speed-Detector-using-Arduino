#include <LiquidCrystal.h>

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int sen1 = 11;
const int sen2 = 12;
const int ledPin = 13; // LED pin for warning
unsigned long t1 = 0;
unsigned long t2 = 0; 
float velocity;
unsigned long resetTimer = 0; // Timer for reset
const unsigned long resetInterval = 10000; // 10 seconds

void setup() {
  lcd.begin(16, 2);
  pinMode(sen1, INPUT);
  pinMode(sen2, INPUT);
  pinMode(ledPin, OUTPUT); // Set LED pin as output
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print(" Speed Detector ");
}

void loop() {
  // Wait for the first sensor to be triggered
  while (digitalRead(sen1));
  while (digitalRead(sen1) == 0);
  t1 = millis(); // Record the time when the first sensor is triggered

  // Wait for the second sensor to be triggered
  while (digitalRead(sen2));
  while (digitalRead(sen2) == 0);
  t2 = millis(); // Record the time when the second sensor is triggered

  // Calculate velocity
  velocity = t2 - t1; // Time in milliseconds
  velocity = velocity / 1000; // Convert to seconds
  if (velocity > 0) { // Ensure velocity calculation is valid
    velocity = (5.0 / velocity) * 3600 / 1000; // Convert to Km/hr
  } else {
    velocity = 0; // Handle case where no time elapsed
  }

  // Display the velocity on the LCD
  for (int i = 5; i > 0; i--) {
    lcd.setCursor(3, 1);
    lcd.print(velocity);
    lcd.print(" Km/hr   ");
    delay(500);
    lcd.setCursor(3, 1);
    lcd.print("            "); // Clear previous velocity
    delay(500);
  }

  // Check if velocity exceeds 150 km/h
  if (velocity > 150) {
    digitalWrite(ledPin, HIGH); // Turn on LED
    lcd.setCursor(0, 1);
    lcd.print("Warning! Speed!");
  } else {
    digitalWrite(ledPin, LOW); // Turn off LED
    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear warning message
  }

  // Start the reset timer
  resetTimer = millis(); // Initialize reset timer
  
  // Wait for 10 seconds (10000 milliseconds)
  while (millis() - resetTimer < resetInterval) {
    // Check for new speed measurement during this period
    if (digitalRead(sen1) == LOW || digitalRead(sen2) == LOW) {
      break; // Exit if a new measurement is detected
    }
  }

  // Reset the display and LED after 10 seconds
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear display
  digitalWrite(ledPin, LOW); // Ensure LED is off
}
