#define BLYNK_TEMPLATE_ID "TMPL6fn_nh7MO"
#define BLYNK_TEMPLATE_NAME "Bong Sen iot"
#define BLYNK_AUTH_TOKEN "e6MIYt7Oupn42ZsMuJM6y31Ei_VU_oFg"
// #define TX 12
// #define RX 11

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <Servo.h>
 
SoftwareSerial mySerial(3, 1);


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "T_BUNNY";
char pass[] = "thocondangyeu";
Servo myservo;  
int mode = 1;
int current = 0;

void setup() {
  mySerial.begin(9600);
  myservo.attach(5);
  Blynk.begin(auth, ssid, pass);
  myservo.write(0);
}

void loop() {
  Blynk.run();
  if (mode == 3){
    for (int i=-15; i<=15; i++){
      myservo.write(current+i);
      delay(50);
    }
    for (int i=15; i>-15; i--){
      myservo.write(current+i);
      delay(50);
    }
  }
}

BLYNK_WRITE(V3) {
  int check = mode;
  mode = 3;
  mySerial.write("3");
  if (check == 1)
    for (int i=0; i<=120; i++){
      myservo.write(i);
      delay(10);
    }
  current = 120;
}

BLYNK_WRITE(V1) {
  int check = mode;
  mySerial.write("1");
  mode = 1;
  if (check == 2 || check == 3)
    for (int i=120; i>=0; i--){
      myservo.write(i);
      delay(10);
    }
}

BLYNK_WRITE(V2) {
  int check = mode;
  mySerial.write("2");
  mode = 2;
  if (check == 1)
    for (int i=0; i<=120; i++){
      myservo.write(i);
      delay(10);
    }
  // mySerial.write("MODE2"); 
}
