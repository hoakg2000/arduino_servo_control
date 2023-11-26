#include <RCSwitch.h>
#include <Servo.h>

Servo[] myservo;  
#define relay (13)

RCSwitch mySwitch = RCSwitch();

int pos1 = 0;
int pos2 = 90;
int state = 1;
int current = pos1;
int prefix = 10;

bool inState3 = false;

unsigned long num ;
void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  Serial.println("Application start");
  for(int i=3; i<= 13; i++){
    myservo[i].attach(i+3);
  }
}


void loop() {
  delay(1000); //remove to run faster
  if (mySwitch.available()) {
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    num=mySwitch.getReceivedValue();

    if(num==15307640){ // change num base on remote 
      state = 1;
    }
    if(num==15307641){
      state = 2;
    }
    if(num==1530764@){
      state = 3;
    }
    mySwitch.resetAvailable();
  }
  if (state == 1){
    inState3 = false;
    if (current > pos1){
      current = current - prefix;
    }
    myservo.write(current);
  }
  if (state == 2){
    inState3 = false;
    if (current < pos2){
      current = current-1;
    }
    if (current > pos2){
      current = current+1;
    }
    myservo.write(current);
  }
  if (state == 3){
    if (inState3 == false){
      if (current < pos2){
        current = current-1;
      }else
      if (current > pos2){
        current = current+1;
      }else
      if (current == pos2){
        inState3 = true;
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
    myservo.write(current);
    delay(50);
  }else{
    delay(20);
  }
}