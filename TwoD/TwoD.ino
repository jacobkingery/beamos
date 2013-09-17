
#include <Servo.h> 
 
Servo panServo;  //create servo object for the pan servo 


const int panPin = 9; 
const int sensorPin = A0;

int pos = 0;    //servo position 
int dist[5];
float aveDist;
 
void setup() { 
  Serial.begin(9600);  //initialize serial communication

  panServo.attach(panPin);  //attach the pan servo
  panServo.write(5);  //turn servo to 5 degrees
  
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
  Serial.println(pos);
} 
 
void loop() {
  for (pos=5; pos<175; pos++) {
    panServo.write(pos);
    shoot();
  }
    for (pos=175; pos>5; pos--) {
    panServo.write(pos);
    shoot();
  }
  
}
