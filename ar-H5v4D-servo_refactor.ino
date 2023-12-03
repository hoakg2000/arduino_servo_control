
#include <RCSwitch.h>
#include <Servo.h>

RCSwitch mySwitch = RCSwitch();
Servo myservo[30];
bool servoState[30];
// Servo myservo9;  

int state = 1;
unsigned long num ;
int pos1 = 0;
int pos2 = 60;
int prefix = 15;
bool goleft = false;
bool inState3 = false;
int previous = 0;

int current = pos1;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

  Serial.println("Application start");
  for(int i=0; i<= 20; i++){
    myservo[i].attach(i+5);
    myservo[i].write(0);
    // delay(2000);
    // myservo[i].writeMicroseconds(1000);
  }

}

void loop() {
  // return;
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
    if(num==1480484) state = 3;
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
    // if (current!= pos2){
    //   if (current < pos2){
    //     current = current+1;
    //   }else
    //   if (current > pos2){
    //     current = current-1;
    //   }
    // }
    if (previous != 2){
      myservo[0].write(40);
      servoState[0] = false;
      for(int i=1; i<= 10; i++){
        int cur = myservo[i-1].read() + 5;
        // Serial.println(cur);
        myservo[i].write(cur);
        servoState[i] = false;
      }
    }else{
      for(int i=0; i<= 10; i++){
        int cur = myservo[i].read();
        // Serial.println(cur);
        if (cur >= 80){
          servoState[i] = true;
        }
        if (cur <= 40){
          servoState[i] = false;
        }
        if (servoState[i]){
          cur--;
        }else{
          cur++;
        }
        myservo[i].write(cur);
      }
    }
    // delay(1000);
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
    for(int i=0; i<= 13; i++){
      if (abs(60-current <= 15)){
        if (i%2 == 0){
          myservo[i].write(120-current);
        }else
          myservo[i].write(current);
      }
      else{
        myservo[i].write(current);
      }
    }
    if (current < pos2 - prefix){
      delay(10);
    }else{
      delay(50);
    }
  }else{
    if (state != 2){
    for(int i=0; i<= 13; i++){
      myservo[i].write(current);
    }
    delay(10);
    }
  }

  previous = state;
}
