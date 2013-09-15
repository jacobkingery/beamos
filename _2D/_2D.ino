
#include <Servo.h> 
 
Servo panServo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created

const int panPin = 9; 
const int sensorPin = A0;

int pos = 0;    // variable to store the servo position 
 
void setup() { 
  Serial.begin(9600);
  panServo.attach(panPin);  // attaches the servo on pin 9 to the servo object 

} 
 
 
void shoot() {
  analogRead(sensorPin);
  delay(5);
  Serial.println(data);
  
}
 
 
void loop() { 
  for(pos = 0; pos < 180; pos += 1) {                                
    panServo.write(pos);
    delay(15);
 
    shoot();
  } 

} 
