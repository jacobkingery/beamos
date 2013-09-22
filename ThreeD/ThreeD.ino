/*
Principles of Engineering, Fall 2013
Lab 1:  DIY LIDAR
Jacob Kingery and Myles Cooper
*/

#include <Servo.h> 
 
Servo panServo;  //create servo object for the pan servo 
Servo tiltServo;  //create servo object for the tilt servo


const int panPin = 9;  //pan servo pin
const int tiltPin = 10;  //tilt servo pin
const int sensorPin = A0;  //IR range sensor pin

int panPos = 5;  //pan position 
int tiltPos = 50;  //tilt position
int totDist;  //total distance of 5 shots
float aveDist;  //average distance of 5 shots
float realDist;  //voltage converted to real distance
 
void setup() { 
  Serial.begin(9600);  //initialize serial communication

  panServo.attach(panPin);  //attach the pan servo
  tiltServo.attach(tiltPin);  //attach the tilt servo
  
  panServo.write(panPos);  //turn pan servo to 5 degrees
  tiltServo.write(tiltPos);  //turn tilt servo to 45 degrees
  
  establishConnection();  //establish connection with python script
} 
 
void establishConnection() {
  Serial.println("Arduino ready!");  //send ready signal to python script
  while (!Serial.available()) {  //wait for python script to send ready signal
  }
  while (Serial.available()) {
    Serial.read();
  }  
  delay(500);  //wait a bit before starting to stream data
}


void shoot() {
  totDist = 0;
  delay(250);
  for (int i=0; i<5; i++) {
    totDist += analogRead(sensorPin);
    delay(50);
  }
  aveDist = totDist / 5.0;
  
  float w = (aveDist - 280)/120;
  realDist = .62*pow(w, 4) - 2.4*pow(w, 3) + 3.4*w*w - 7.3*w + 18;
  
  Serial.print(realDist);
  Serial.print('@');
  Serial.print(panPos);
  Serial.print('@');  
  Serial.println(tiltPos);
} 
 
void loop() {
  for (panPos=5; panPos<175; panPos+=5) {
    panServo.write(panPos);
    if (panPos%2 != 0) {
      for (tiltPos=50; tiltPos<=100; tiltPos+=10) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
    else {
      for (tiltPos=100; tiltPos>=50; tiltPos-=10) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
  }
  
  for (panPos=175; panPos>5; panPos-=5) {
    panServo.write(panPos);
    if (panPos%2 != 0) {
      for (tiltPos=50; tiltPos<=100; tiltPos+=10) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
    else {
      for (tiltPos=100; tiltPos>=50; tiltPos-=10) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
  }
  
}
