#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FLOW_SENSOR_PIN 2  // Water flow sensor connected to pin 2

volatile int pulseCount = 0; // Pulse counter
float flowRate = 0;          // Flow rate in L/min
float totalLiters = 0;       // Total water volume in liters
unsigned long lastTime = 0;  // Timer for calculations
const float pulsesPerLiter = 450; // Adjust based on your sensor specs (YF-S201 = ~450 pulses per liter)

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C address

// Interrupt Service Routine (ISR) to count pulses
void pulseCounter() {
  pulseCount++;
}

void setup() {
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Water Flow Meter");

  lastTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastTime >= 1000) { // Update every second
    detachInterrupt(FLOW_SENSOR_PIN); // Temporarily stop counting pulses

    // Convert pulses to water volume (Liters)
    float litersPassed = pulseCount / pulsesPerLiter;  
    totalLiters += litersPassed;

    // Convert to flow rate (Liters per minute)
    flowRate = litersPassed * 60;

    // Display values on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Flow: ");
    lcd.print(flowRate, 2); // Two decimal places
    lcd.print(" L/min");

    lcd.setCursor(0, 1);
    lcd.print("Total: ");
    lcd.print(totalLiters, 2); // Two decimal places
    lcd.print(" L");

    pulseCount = 0;  // Reset pulse count
    lastTime = currentMillis;
    
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);
  }
}
