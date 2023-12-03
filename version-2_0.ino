
#include <RCSwitch.h>
#include <Servo.h>

RCSwitch mySwitch = RCSwitch();
Servo myservo[30];
// Servo ser;
bool servoState[30];
// Servo myservo9;  

int state = 1;
unsigned long num ;

int pos1 = 0;
int pos2 = 60;
int prefix1 = 1;
int prefix2 = 15;
int prefix3 = 20;
int servoCount = -1;
bool instate2 = false;
bool instate3 = false;
int previousState = -1;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  Serial.println();
  Serial.println("Application start");
  
  attachServo();

}

void attachServo(){
  int idx = 0;
  for (int j=0; j<20;j++){
    myservo[idx].attach(idx+4);
    myservo[idx].write(45);
    if (myservo[idx].read() == 45){
      myservo[idx].write(90); 
      idx++;
    } 
  }
  servoCount = idx;
  Serial.println("Attachded");
}

void loop() {
  handleReceiveRemoteSignal();
  choseState();
  previousState = state;
}

void handleReceiveRemoteSignal(){
  if (mySwitch.available()) {
    num=mySwitch.getReceivedValue();
    if(num==1480481) state = 1;
    else
    if(num==1480482) state = 2;
    else 
    if(num==1480484) state = 3;
    mySwitch.resetAvailable();
    
    Serial.print("Received ");
    Serial.println( num );

    Serial.print("State ");
    Serial.println(state );
  }
}

void choseState(){
  if(state == 1){
    instate2 = false;
    instate3 = false;
    stateOneMode();
    return;
  }
  if(state == 2){
    instate3 = false;
    stateTwoMode();
    return;
  }
  if(state == 3){
    instate2 = false;
    stateThreeMode();
    return;
  }
}

void stateOneMode(){
  for (int i=0; i< servoCount; i++){
    int cur = myservo[i].read();
    if (cur== pos1){
      continue;
    }
    if (cur > pos1){
      cur--;
    }else{
      cur++;
    }
    myservo[i].write(cur);
  }
  delay(10);
}
void stateTwoMode(){
  if (!instate2){
    moveToPos(pos2);
  }else{
    for (int i=0; i< servoCount; i++){
      int cur = myservo[i].read();
      if (servoState[i]){
        cur++;
      }else{
        cur--;
      }
      if (cur == pos2 - prefix2){
        servoState[i] = true;
      }
      if (cur == pos2 + prefix2){
        servoState[i] = false;
      }
      myservo[i].write(cur);
    }
    delay(30);
  }
}

void moveToPos(int pos){
  Serial.println("move to pos");
  bool kt = true;
  for (int i=0; i< servoCount; i++){
    int cur = myservo[i].read();
    if (cur < pos){
      cur++;
      kt = false;
    }else{
      if (cur > pos){
        cur--;
        kt = false;
      }else{
        servoState[i] = i % 2 == 0;
      }
    }
    myservo[i].write(cur);
  }
  instate2 = kt;
  delay(10);
}

void stateThreeMode(){
  if (!instate3){
    myservo[0].write(pos2 - prefix3);
    servoState[0] = true;
    for (int i=1; i< servoCount; i++){
      int next = myservo[i-1].read() + 5;
      if (next > pos2+prefix3){
        next = pos2 + prefix3 - (next - (pos2 + prefix3));
      }
      myservo[i].write(myservo[i-1].read() + 3);
      servoState[i] = true;
    }
    instate3 = true;
  }else{
    for (int i=0; i<servoCount; i++){
      int cur = myservo[i].read();
      if (servoState[i]){
        cur++;
      }else{
        cur--;
      }
      if (cur == pos2 - prefix3){
        servoState[i] = true;
      }
      if (cur == pos2 + prefix3){
        servoState[i] = false;
      }
      myservo[i].write(cur);
    }
    delay(30);
  }
}
