#include <Servo.h>

Servo myservo[15];
// Servo myservo9;  

int state = 1;
int pos1 = 0;
int pos2 = 90;
int prefix = 10;
bool goleft = false;
bool inState3 = false;

int current = pos1;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(200);
  Serial.println("Application start");
  for(int i=3; i<= 13; i++){
    myservo[i].attach(i+3);
    myservo[i].write(20);
  }
  // myservo9.attach(9);
  // myservo9.write(90);
  delay(10000);
}

void loop() {
  // delay(1000);
  Serial.print("Current: ");
  Serial.println(current);
  Serial.print("State: ");
  Serial.println(state);

  if (Serial.available()){
    String message = Serial.readStringUntil('\n');
    Serial.print("Receive: ");
    Serial.println(message);
    state = message[message.length() - 1] - '0';
    Serial.println(state);
  }
  if (state == 1){
    inState3 = false;
    if (current!=pos1){
      if (current > pos1){
        current = current - 1;
      }
    }
  }
  if (state == 2){
    inState3 = false;
    if (current!= pos2){
      if (current < pos2){
        current = current+1;
      }else
      if (current > pos2){
        current = current-1;
      }
    }
  }
  if (state == 3){
    if (inState3 == false){
      if (current < pos2){
        current = current+1;
      }else
      if (current > pos2){
        current = current-1;
      }else
      if (current == pos2){
        inState3 = true;
        goleft = true;
      }
    }else{
      if (goleft == true){
        current = current+1;
        if (current == pos2 + prefix){
          goleft = false;
        }
      }else{
        current = current-1;
        if (current == pos2 - prefix){
          goleft = true;
        }
      }
    }
    for(int i=3; i<= 13; i++){
      myservo[i].write(current);
    }
    if (current < pos2 - prefix){
      delay(20);
    }else{
      delay(70);
    }
  }else{
    for(int i=3; i<= 13; i++){
      myservo[i].write(current);
    }
    delay(20);
  }
}
