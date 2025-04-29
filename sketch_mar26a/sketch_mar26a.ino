#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FLOW_SENSOR_PIN 2  
volatile int pulseCount = 0; 
float flowRate = 0;         
float totalLiters = 0;       
unsigned long lastTime = 0;  
const float pulsesPerLiter = 450; 
LiquidCrystal_I2C lcd(0x27, 16, 2); 


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
  if (currentMillis - lastTime >= 1000) { 
    detachInterrupt(FLOW_SENSOR_PIN); 
   
    float litersPassed = pulseCount / pulsesPerLiter;  
    totalLiters += litersPassed;

    
    flowRate = litersPassed * 60;

    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Flow: ");
    lcd.print(flowRate, 2); 
    lcd.print(" L/min");

    lcd.setCursor(0, 1);
    lcd.print("Total: ");
    lcd.print(totalLiters, 2); 
    lcd.print(" L");

    pulseCount = 0;  
    lastTime = currentMillis;
    
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);
  }
}
