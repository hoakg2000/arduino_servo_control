#include <RCSwitch.h>
#define svpin (9)
RCSwitch mySwitch = RCSwitch();
unsigned long num ;

#include <Servo.h>
Servo sv1;

int position=0;
int current = 60;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

 sv1.attach(svpin);
 
}


void loop() {
  if (mySwitch.available()) {
    
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    num=mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
  }
  
  if(num==1480481){
    for (position=0; position<60;position++){
      sv1.write(position);
      delay(70);
    };
    num=0;
  } //thay đổi số tương ứng
  
  
  if(num==1480482){
    for (int i=0; i<=15; i++){
      sv1.write(current+i);
      delay(50);
    }
    for (int i=15; i>-15; i--){
      sv1.write(current+i);
      delay(50);
    }
    for (int i=-15; i<0; i++){
      sv1.write(current+i);
      delay(50);
    }
  }//thay đổi số tương ứng
  
  if(num==1480484){
    for (int i=170; i>0; i--){
      sv1.write(i);
      delay(10);
    };
    num=0;
  }//thay đổi số tương ứng
}