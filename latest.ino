
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
int pos2 = 30;
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
  for (int j=8; j<=10;j++){ // attach servo vào pin 8, 9, 10
    myservo[idx].attach(j);
    // myservo[idx].write(45);
    // delay(1000);
    if (myservo[idx].attached()){
      myservo[idx].write(0); 
      idx++;
    } 
  }
  servoCount = idx;
  Serial.print("Attachded: ");
  Serial.println(servoCount);
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
  delay(30); //delay chuyển từ 2, 3 sang 1
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
    delay(50);//delay rung state 2
  }
}

void moveToPos(int pos){
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
  delay(30); //delay chuyển từ state 1, 3 sang 2
}

void stateThreeMode(){
  if (!instate3){
    instate3 = true;
    int l = 0;
    int r = servoCount - 1;
    servoState[l] = true;
    servoState[r] = true;
    int reach[servoCount];
    reach[l] = pos2-prefix3;
    reach[r] = pos2-prefix3;

    l++;
    r--;
    while (l <= r){
      reach[l] = reach[l-1] + 3;
      reach[r] = reach[r+1] + 3;
      servoState[l] = true;
      servoState[r] = true;
      l++;
      r--;
    }
      // Serial.println(reach[1]);    

    for (int i=0 ;i< servoCount; i++){
      // Serial.println(myservo[i].read());    
      int cur = myservo[i].read();
      if (reach[i] == cur){

      }else{
        if (reach[i] > cur){
          cur++;
        }else{
          cur--;
        }
        myservo[i].write(cur);
        instate3 = false;
      }
    }
    // Serial.println();
    // delay(1000);
    
    delay(30);// delay chuyển từ state 1, 2 sang 3
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
      // Serial.println(myservo[i].read());
    }
    delay(70); // delay tăng giảm state 3
  }
}
