#include <Servo.h>
long jumpActionArray[200] = {};//werkt tot 200 jumps
int jumpActionArrayLength = 0;
int currentJumpActionIndex = 0;
int currentMoveBackActionIndex = 0;
long moveBackArray[200] = {}; //tot 200 keer weer terug
int moveBackActionLength = 0;
Servo serv;
int pos = 0;
float dely = 50;
int lastLDR = 0;
int LDR = 0;
int nextmove = 0;
boolean wasLastLoopAction = false;

void setup() {
  pinMode(0, INPUT);
  Serial.begin(9600);
  serv.attach(9);
  serv.write(3); //spatie uit
  lastLDR = analogRead(0);
}

void loop() {

  LDR = analogRead(0); //lees LDR waard
  //Serial.println("LDR value: " + LDR);

  if (wasLastLoopAction == false && LDR < lastLDR - 5) {//anti bounce
    wasLastLoopAction = true;
    jumpActionArray[jumpActionArrayLength] = millis() + dely; // opdracht queuen
    jumpActionArrayLength++;
    dely = dely - 1.25; // delay telkens verminderen
    Serial.println("queue jump");
  } else {
    wasLastLoopAction = false;
  }
  lastLDR = LDR;
  Serial.println(currentJumpActionIndex);
  Serial.println(jumpActionArray[currentJumpActionIndex]);
  if (jumpActionArray[currentJumpActionIndex] != 0 && millis() > jumpActionArray[currentJumpActionIndex]) { // als huidige instructie tijd op de stack geweest is (jump)
    Serial.println("run jump");
    serv.write(15); //spatie in
    currentJumpActionIndex++; //de volgende actie klaarzetten

    moveBackArray[moveBackActionLength] = millis() + 200; //queue move back
    moveBackActionLength++;
  }

  //Serial.println(moveBackArray[currentMoveBackActionIndex] + " < next current > " + millis());

  if (moveBackArray[currentMoveBackActionIndex] != 0 && millis() > moveBackArray[currentMoveBackActionIndex]) { //als huidige instructie tijd op stack geweestr is (moveback)
    Serial.println("move back");
    currentMoveBackActionIndex++; //wacht op volgende move back
    serv.write(3); //spatie uit
  }

  delay(100);//anders is LDR te specifiek, hier nog wat op vinden?



}
