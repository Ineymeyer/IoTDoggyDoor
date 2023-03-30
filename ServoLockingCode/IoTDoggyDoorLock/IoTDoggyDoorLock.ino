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
  pinMode(7,INPUT_PULLUP); // From pi to unlock or lock
  pinMode(6,INPUT); // Input from esp32
  pinMode(4, INPUT_PULLUP); // Input from Mag Strip
  pinMode(3,OUTPUT); // Magnetic strip LED
  pinMode(11, OUTPUT); // Tells pi if door is locked or unlocked
  pinMode(12, INPUT_PULLUP); // From pi to arduino to say if bluetooth tracking is enabled
  digitalWrite(11, LOW);  
}
boolean lock = true;
boolean changed = false;
boolean changedBT = false;
boolean doorClosed = false;
boolean bluetoothTrack = false;

void loop() {

  if(digitalRead(4) == HIGH){
    doorClosed = false;
    digitalWrite(3,HIGH);
  }
  else{
    doorClosed = true;
    digitalWrite(3,LOW);
  }
//  if(digitalRead(12) == HIGH){
//    bluetoothTrack = true;
//  }
//  else{
//    bluetoothTrack = false; 
//  }
//
//
//  if (bluetoothTrack){
//    if ((digitalRead(6) == LOW) && (digitalRead(4) == LOW) && (changedBT != true) && (lock != true)){
//      delay(1000);
//      if ((digitalRead(4) == LOW) && (digitalRead(6) == LOW)){
//        lockDoor();
//        digitalWrite(11, HIGH);
//        changedBT = true;
//      }
//    }
//    else if((digitalRead(6) == HIGH) && (changed = true) && (lock == true)){
//      unlockDoor();
//      digitalWrite(11, LOW);
//      changedBT = false;
//    }
//  }
  if(((digitalRead(7) == HIGH) && (lock != true) && (digitalRead(4) == LOW) && (digitalRead(12) == LOW)) || ((digitalRead(12) == HIGH) && (digitalRead(6) == LOW) && (lock != true) && (digitalRead(4) == LOW))){
    delay(1000);
    if (digitalRead(4) == LOW){ // Used to make sure still door is still closed
      lockDoor();
      digitalWrite(11, HIGH);
      changed = true;
    }
  }
  else if(((digitalRead(7) == LOW) && (lock == true) && (digitalRead(12) == LOW)) || ((digitalRead(12) == HIGH) && (digitalRead(6) == HIGH) && (lock == true))){
    unlockDoor();
    digitalWrite(11, LOW);
    changed = false; 
  }
};

void lockDoor(){
  lock = true;
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
  lock = false;
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
