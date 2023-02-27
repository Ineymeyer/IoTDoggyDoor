#include <Servo.h>



Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(8);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  pinMode(7,INPUT);
  pinMode(6,INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(3,OUTPUT);
}
boolean lock = false;
boolean changed = false;

void loop() {

  if(digitalRead(4) == HIGH){
    digitalWrite(3,HIGH);
  }
  else{
    digitalWrite(3,LOW);
  }

  if ((digitalRead(7) == HIGH) && (changed != true)){
    lockDoor();
    changed = true;
  }
  else if((digitalRead(7) == LOW) && (changed == true)){
    unlockDoor();
    changed = false; 
  }
};

void lockDoor(){
  for (pos = 0; pos <= 70; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);   
  }                    // waits 15ms for the servo to reach the position
};

void unlockDoor(){
  for (pos = 70; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
};

