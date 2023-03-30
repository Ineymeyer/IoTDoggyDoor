#include <Servo.h>



Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
Servo myservo1;
Servo myservo2;

int pos = 70;    // variable to store the servo position
int pos1 = 120;
int pos2 = 3;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 8 to the servo object
  myservo1.attach(8);
  myservo1.write(120);
  myservo.write(70);  
  myservo2.attach(10);
  myservo2.write(3);
  pinMode(7,INPUT_PULLUP);
  //pinMode(6,INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(3,OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(11, LOW);  
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
  for (pos = 3; pos <= 70; pos += 1) {
    myservo.write(pos);              
    delay(2);   
  } 
  for (pos1 = 35; pos1 <= 120; pos1 += 1) { 
      
    myservo1.write(pos1);              
    delay(2);   
  }                   
 
  for (pos2 = 63; pos2 >= 3; pos2 -= 1) { 
    myservo2.write(pos2);              
    delay(2);
  }
};

void unlockDoor(){
  for (pos = 70; pos >= 3; pos -= 1) { 
    myservo.write(pos);              
    delay(2);                       
  }
  for (pos1 = 120; pos1 >= 35; pos1 -= 1) { 
    myservo1.write(pos1);              
    delay(2);                       
  }
   for (pos2 = 3; pos2 <= 63; pos2 += 1) { 
    myservo2.write(pos2);              
    delay(2);
  }
};
