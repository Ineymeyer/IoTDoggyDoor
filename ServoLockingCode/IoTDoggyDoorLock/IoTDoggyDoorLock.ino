#include <Servo.h>



Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
Servo myservo1;

int pos = 60;    // variable to store the servo position
int pos1 = 120;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 8 to the servo object
  myservo1.attach(8);
  myservo1.write(120);
  myservo.write(60);  
  pinMode(7,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(3,OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
}
boolean lock = false;
boolean changed = false;
boolean doorClosed = false;

void loop() {

  if(digitalRead(4) == HIGH){
    doorClosed = false;
    digitalWrite(3,HIGH);
  }
  else{
    doorClosed = true;
    digitalWrite(3,LOW);
  }

  if ((digitalRead(7) == HIGH) && (changed != true) && (digitalRead(4) == LOW)){
    delay(1000);
    if (digitalRead(4) == LOW){ // Used to make sure still door is still closed
      lockDoor();
      digitalWrite(11, HIGH);
      changed = true;
    }
  }
  else if((digitalRead(7) == LOW) && (changed == true)){
    unlockDoor();
    digitalWrite(11, LOW);
    changed = false; 
  }
};

void lockDoor(){
  for (pos = 3; pos <= 60; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);   
  } 
  for (pos1 = 25; pos1 <= 120; pos1 += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
    myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(5);   
  }                   // waits 15ms for the servo to reach the position
};

void unlockDoor(){
  for (pos = 60; pos >= 3; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  for (pos1 = 120; pos1 >= 25; pos1 -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
};
