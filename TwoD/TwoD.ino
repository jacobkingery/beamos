
#include <Servo.h> 
 
Servo panServo;  //create servo object for the pan servo 


const int panPin = 9; 
const int sensorPin = A0;

int pos;    //servo position 
int posMin = 5;
int posMax = 175;
int totDist;
float aveDist;
float realDist;
 
void setup() { 
  Serial.begin(9600);  //initialize serial communication

  panServo.attach(panPin);  //attach the pan servo
  panServo.write(posMin);  //turn servo to 5 degrees
  
  establishConnection();
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
  
  float w = (aveDist - 280)/120; //convert IR signal to distance in inches
  realDist = .62*pow(w, 4) - 2.4*pow(w, 3) + 3.4*w*w - 7.3*w + 18;
  
  Serial.print(realDist);
  Serial.print('@');
  Serial.println(pos);
} 
 
void loop() {
  for (pos=posMin; pos<posMax; pos++) {
    panServo.write(pos);
    shoot();
  }
    for (pos=posMax; pos>posMin; pos--) {
    panServo.write(pos);
    shoot();
  }
  
}
