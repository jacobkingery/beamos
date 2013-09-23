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

const int panMin = 20;    //set range of pan angles
const int panMax = 160;
const int panRes = 5;     //pan angle between each shot
const int tiltMin = 60;   //set range of tilt angles
const int tiltMax = 100;
const int tiltRes = 5;    //tilt angle between each shot

int panPos;    //define pan position
int tiltPos;   //define tilt position
int totDist;  //total distance of 5 shots
float aveDist;  //average distance of 5 shots
float realDist;  //voltage converted to real distance
 
void setup() { 
  Serial.begin(9600);  //initialize serial communication

  panServo.attach(panPin);  //attach the pan servo
  tiltServo.attach(tiltPin);  //attach the tilt servo
  
  panServo.write(panMin);  //turn pan servo to starting angle
  tiltServo.write(tiltMin);  //turn tilt servo to starting angle
  
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


void shoot() {  //take distance reading from IR sensor
  totDist = 0;
  delay(250);
  for (int i=0; i<5; i++) {  //take 5 shots
    totDist += analogRead(sensorPin);
    delay(50);
  }
  aveDist = totDist / 5.0;  //average reading to eliminate outliers
  
  float w = (aveDist - 280)/120; //convert IR signal to distance in inches
  realDist = .62*pow(w, 4) - 2.4*pow(w, 3) + 3.4*w*w - 7.3*w + 18;
  
  //data is spherical coordinates to be sent to python
  Serial.print(realDist);  //corrected distance from IR sensor
  Serial.print('@');
  Serial.print(panPos);  //pan angle
  Serial.print('@');  
  Serial.println(tiltPos);  //tilt angle
} 
 
void loop() {
  for (panPos=panMin; panPos<=panMax; panPos+=panRes) {
    panServo.write(panPos);
    if (panPos%2 == 0) {
      for (tiltPos=tiltMin; tiltPos<=tiltMax; tiltPos+=tiltRes) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
    else {
      for (tiltPos=tiltMax; tiltPos>=tiltMin; tiltPos-=tiltRes) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
  }
  
//  for (panPos=panMax; panPos>panMin; panPos-=panRes) {
//    panServo.write(panPos);
//   if (panPos%2 == 0) {
//      for (tiltPos=tiltMin; tiltPos<=tiltMax; tiltPos+=tiltRes) {
//        tiltServo.write(tiltPos);
//        shoot();
//      }
//    }
//    else {
//      for (tiltPos=tiltMax; tiltPos>=tiltMin; tiltPos-=tiltRes) {
//        tiltServo.write(tiltPos);
//        shoot();
//      }
//    }
//  }
  
}
