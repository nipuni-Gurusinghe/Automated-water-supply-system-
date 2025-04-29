#include <Keypad.h>
#include <Servo.h>

#define SERVO_PIN 10  
Servo gateServo;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

bool gateOpen = false;
unsigned long closeTime = 0;

String inputMinutes = ""; 

void openGate(int minutes) {
  gateServo.write(90);  
  gateOpen = true;
  closeTime = millis() + (minutes * 60000UL); 
  Serial.print("Gate Open for ");
  Serial.print(minutes);
  Serial.println(" minutes");
}

void closeGate() {
  gateServo.write(0);  
  gateOpen = false;
  Serial.println("Gate Closed");
}

void setup() {
  Serial.begin(9600);
  gateServo.attach(SERVO_PIN);
  gateServo.write(0); 
}

void loop() {
  char key = keypad.getKey();

  if (key) {  
    if (key >= '0' && key <= '9') {
      inputMinutes += key; 
      Serial.print("Input so far: ");
      Serial.println(inputMinutes);
    }
    else if (key == '#') {
      if (inputMinutes.length() > 0) {
        int minutes = inputMinutes.toInt();  
        if (minutes > 0) {
          openGate(minutes);
        } else {
          Serial.println("Invalid input!");
        }
        inputMinutes = ""; 
      }
    }
    else if (key == 'A' && gateOpen) {
      Serial.println("Emergency Close Activated");
      closeGate();
      inputMinutes = ""; 
    }
    else if (key == '*') {
      
      inputMinutes = "";
      Serial.println("Input cleared");
    }
  }

  
  if (gateOpen && millis() >= closeTime) {
    closeGate();
  }
}
