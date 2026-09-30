#include <ESP32Servo.h>

Servo myServo;  

const int servoPin = 14; 
const int trigPin = 25; 
const int echoPin = 32; 

bool isOpen = false; 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Initialize to closed position on startup
  myServo.attach(servoPin); 
  myServo.write(0); 
  delay(1500);       // Give it 1.5 seconds to fully close
  myServo.detach();  // Cut power (gravity keeps it closed)
}

void loop() {
  // 1. Send the ultrasonic ping
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 2. Measure the echo
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; 

  // 3. Smart battery-saving logic
  if (distance > 0 && distance <= 14) {
    
    // Only trigger if the lid is currently closed
    if (isOpen == false) { 
      myServo.attach(servoPin); // Wake up the motor
      delay(50);                // Give ESP32 timer 50ms to stabilize
      myServo.write(150);       // Snap open to 150 degrees
      
      // Motor stays attached to hold the heavy lid open against gravity
      isOpen = true;            
    }
    
  } else {
    
    // Only trigger if the lid is currently open
    if (isOpen == true) { 
      delay(2000);              // Keep it open for 2 seconds after hand leaves
      myServo.write(0);         // Snap shut to 0 degrees
      delay(1500);              // Give it 1.5 full seconds to physically close
      myServo.detach();         // Cut power to the motor to save battery
      isOpen = false;           
    }
    
  }
  
  delay(50); // Short pause before taking the next measurement
}