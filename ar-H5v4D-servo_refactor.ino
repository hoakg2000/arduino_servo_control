
#include <RCSwitch.h>
#include <Servo.h>

RCSwitch mySwitch = RCSwitch();
Servo myservo[15];
// Servo myservo9;  

int state = 1;
unsigned long num ;
int pos1 = 0;
int pos2 = 60;
int prefix = 15;
bool goleft = false;
bool inState3 = false;

int current = pos1;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(200);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

  Serial.println("Application start");
  for(int i=3; i<= 13; i++){
    myservo[i].attach(i+3);
  }
}

void loop() {
  // delay(1000);
  Serial.print("Current: ");
  Serial.println(current);
  Serial.print("State: ");
  Serial.println(state);

  if (mySwitch.available()) {
    
    Serial.print("Received ");
    Serial.println( mySwitch.getReceivedValue() );
    num=mySwitch.getReceivedValue();
    if(num==1480481) state = 1;
    else
    if(num==1480482) state = 2;
    else 
    if(num==1480483) state = 3;
    mySwitch.resetAvailable();
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
      delay(10);
    }else{
      delay(50);
    }
  }else{
    for(int i=3; i<= 13; i++){
      myservo[i].write(current);
    }
    delay(10);
  }
}
