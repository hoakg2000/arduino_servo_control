#include <Servo.h>
#include <RCSwitch.h>

Servo myservo;
RCSwitch mySwitch = RCSwitch();

int state = 1; // Initial state

void setup() {
  myservo.attach(9);  // Attach servo to pin 9
  mySwitch.enableReceive(0);  // Receiver on interrupt 0
  Serial.begin(9600);
}

void loop() {
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    if (value == 0) {
      mySwitch.resetAvailable();
      return;
    }

    // Check the received button number
    switch (value) {
      case 1480481:
        state = 1;
        break;
      case 1480482:
        state = 2;
        break;
      case 1480484:
        state = 3;
        break;
      default:
        break;
    }

    // Perform action based on the state
    switch (state) {
      case 1:
        moveServo(0);
        break;
      case 2:
        moveServo(90);
        break;
      case 3:
        moveServoWithRange(90, 10);
        break;
      default:
        break;
    }

    mySwitch.resetAvailable();
  }
}

void moveServo(int position) {
  int currentPos = myservo.read();
  while (currentPos != position) {
    if (currentPos < position) {
      currentPos++;
    } else {
      currentPos--;
    }
    myservo.write(currentPos);
    delay(15);  // Adjust delay for slower movement
  }
}

void moveServoWithRange(int position, int range) {
  int targetPos = position;
  int currentPos = myservo.read();
  int step = (currentPos < targetPos) ? 1 : -1;

  while (currentPos != targetPos) {
    if (abs(currentPos - targetPos) <= range) {
      currentPos = targetPos;
    } else {
      currentPos += step;
    }
    myservo.write(currentPos);
    delay(15);  // Adjust delay for slower movement
  }
}
