
#include <Servo.h>

Servo ESC;     // create servo object to control the ESC

int JOY_Y;  // value from the analog pin

void setup() {
    // Attach the ESC on pin 9
    ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
}

void loop() {
    weaponControl(JOY_Y);
}

void weaponControl(int joyValue) {

    motorValue = map(joyValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
    ESC.write(motorValue);    // Send the signal to the ESC
}