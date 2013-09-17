
#include <Servo.h> 
 
Servo panServo;  //create servo object for the pan servo 
Servo tiltServo;  //create servo object for the tilt servo


const int panPin = 9; 
const int tiltPin = 10;
const int sensorPin = A0;

int panPos = 5;    //servo pan position 
int tiltPos = 45;
float dist[5];
float aveDist;
 
void setup() { 
  Serial.begin(9600);  //initialize serial communication

  panServo.attach(panPin);  //attach the pan servo
  tiltServo.attach(tiltPin);  //attach the tilt servo
  panServo.write(panPos);  //turn pan servo to 5 degrees
  tiltServo.write(tiltPos);  //turn tilt servo to 45 degrees
  
  establishConnection();
} 
 
void establishConnection() {
  delay(500);  //wait to ensure that serial connection is made
  Serial.println("Arduino ready!");  //send ready signal to python script
  while (!Serial.available()) {  //wait for python script to send ready signal
  }
  while (Serial.available()) {
    Serial.read();
  }  
  delay(500);
}


void shoot() {
  delay(50);
  for (int i=0; i<5; i++) {
    dist[i] = analogRead(sensorPin);
    delay(50);
  }
  aveDist = (dist[0] + dist[1] + dist[2] + dist[3] + dist[4]) / 5.0;
  Serial.print(aveDist);
  Serial.print('@');
  Serial.print(panPos);
  Serial.print('@');  
  Serial.println(tiltPos);
} 
 
void loop() {
  for (panPos=5; panPos<175; panPos++) {
    panServo.write(panPos);
    if (panPos%2 != 0) {
      for (tiltPos=45; tiltPos<=100; panPos++) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
    else {
      for (tiltPos=100; tiltPos>=45; panPos--) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
  }
  
  for (panPos=175; panPos>5; panPos--) {
    panServo.write(panPos);
    if (panPos%2 != 0) {
      for (tiltPos=45; tiltPos<=100; panPos++) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
    else {
      for (tiltPos=100; tiltPos>=45; panPos--) {
        tiltServo.write(tiltPos);
        shoot();
      }
    }
  }
  
}
