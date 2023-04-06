#include <Servo.h>

  /************ I/O pins ************
  | Pin # |                Function                       |         Logic high         |       Logic low
  |   3   | Signal going to test LED                      | When door is open          | When door is closed
  |   4   | Signal from the magnetic strip on door        | If the door is open        | If the door is closed
  |   6   | Signal from the ESP32 Module                  | If the tag is within range | If it is out of range
  |   7   | Signal from the Raspberry Pi                  | To lock the door           | To unlock the door
  |   8   | Servo on the bottom of the door               | N/A                        | N/A
  |   9   | Servo on the right side of the door           | N/A                        | N/A
  |   10  | Servo on the left side of the door            | N/A                        | N/A
  |   11  | Signal going to Raspberry Pi with lock status | If it is locked            | If it is unlocked
  |   12  | Signal from Raspberry Pi with tracking status | If it is enabled           | If it is disabled
  |   13  | Signal going to pi to tell if door is open    | N/A                        | NA
  */

// Creating new servo motor objects
Servo myservo,
      myservo1,
      myservo2;

// Variables to store the starting positions for each servo
int pos = 70,
    pos1 = 120,
    pos2 = 3;

// Boolean status variables used in the loop
boolean lock = true,
        changed = false,
        doorClosed = false;

void setup() {
  // Right servo configuration
  myservo.attach(9);
  myservo.write(70);

  // Bottom servo configuration
  myservo1.attach(8);
  myservo1.write(120);

  // Left servo configuration
  myservo2.attach(10);
  myservo2.write(3);

  // I/O Pin configuration
  pinMode(3,OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(6,INPUT);
  pinMode(7,INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);
}

void loop() {
  // If the door is open, update the boolean and turn on the test LED, otherwise update the boolean and turn the LED off
  if(digitalRead(4) == HIGH){
    doorClosed = false;
    digitalWrite(3,HIGH);
    digitalWrite(13, HIGH);
  } else {
    doorClosed = true;
    digitalWrite(3,LOW);
    digitalWrite(13, LOW);
  }

  /* Locks the door if...
       The user clicked lock on the website while the door is unlocked and closed and BT tracking is disabled
       The door is closed and unlocked with BT tracking enabled and the tag out of range

     Unlocks the door if...
       The user clicked unlock on the website while the door is locked and BT tracking is disabled
       The door is locked and the tag comes within range with BT tracking enabled
  */
  if (((digitalRead(7) == HIGH) && (lock != true) && (digitalRead(4) == LOW) && (digitalRead(12) == LOW)) || ((digitalRead(12) == HIGH) && (digitalRead(6) == LOW) && (lock != true) && (digitalRead(4) == LOW))) {
    delay(1000);

    // If the door is still closed, lock the door, signal to the pi that the door is locked so it can update the website, and update the boolean
    if (digitalRead(4) == LOW){
      lockDoor();
      digitalWrite(11, HIGH);
      changed = true;
    }
  } else if (((digitalRead(7) == LOW) && (lock == true) && (digitalRead(12) == LOW)) || ((digitalRead(12) == HIGH) && (digitalRead(6) == HIGH) && (lock == true))) {
    // Unlock the door, signal to the pi that the door is unlocked so it can update the website, and update the boolean
    unlockDoor();
    digitalWrite(11, LOW);
    changed = false; 
  }
};

// Function that moves each servo motor to their locked position
void lockDoor(){
  // Update the boolean
  lock = true;

  // Move the right servo
  for (pos = 3; pos <= 70; pos += 1) {
    myservo.write(pos);              
    delay(2);   
  } 

  // Move the bottom servo
  for (pos1 = 35; pos1 <= 120; pos1 += 1) {     
    myservo1.write(pos1);              
    delay(2);   
  }                   

  // Move the left servo
  for (pos2 = 63; pos2 >= 3; pos2 -= 1) { 
    myservo2.write(pos2);              
    delay(2);
  }
};

// Function that moves each servo motor to their unlocked position
void unlockDoor(){
  // Update the boolean
  lock = false;

  // Move the right servo
  for (pos = 70; pos >= 3; pos -= 1) { 
    myservo.write(pos);              
    delay(2);                       
  }

  // move the bottom servo
  for (pos1 = 120; pos1 >= 35; pos1 -= 1) { 
    myservo1.write(pos1);              
    delay(2);                       
  }
  
  // Move the left servo
  for (pos2 = 3; pos2 <= 63; pos2 += 1) { 
    myservo2.write(pos2);              
    delay(2);
  }
};





// Code im not sure if we still need or not

//  boolean changedBT = false;
//  boolean bluetoothTrack = false;

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
