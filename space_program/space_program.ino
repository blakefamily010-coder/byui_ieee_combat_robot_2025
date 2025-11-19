
#include <Servo.h>

Servo ESC;     // create servo object to control the ESC

int JOY2_Y;  // value from the analog pin

int JOY1_X;         // left/right
int JOY1_Y;         // forward/back

int R_speed = 5;    // Pins subject to change.
int R_dir1 = 4;
int R_dir2 = 3;

int L_speed = 6;
int L_dir1 = 7;
int L_dir2 = 8;

int throttle;       // forward/back
int steering;       // left/right
int leftCmd;        // combined command
int rightCmd;       // combined command

void setup() {
    // Attach the ESC on pin 9
    ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds)

    pinMode(L_speed, OUTPUT);
    pinMode(L_dir1, OUTPUT);
    pinMode(L_dir2, OUTPUT);

    pinMode(R_speed, OUTPUT);
    pinMode(R_dir1, OUTPUT);
    pinMode(R_dir2, OUTPUT);

    pinMode(joy1_Y, INPUT);
    pinMode(joy1_X, INPUT);
}

void loop() {
    weaponControl(JOY2_Y);
    movementControl(JOY1_X, JOY1_Y);
}

void weaponControl(int joy2Value) {

    motorValue = map(joy2Value, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
    ESC.write(motorValue);    // Send the signal to the ESC
}

void movementControl(int joy1_X_Value, int joy1_Y_Value) {
    // Convert to -255..255 ranges
    throttle = map(joy1_Y_Value, 0, 1023, -255, 255);   // forward/back
    steering = map(joy1_X_Value, 0, 1023, -255, 255);   // left/right

    // Differential motor mixing
    leftCmd  = throttle + steering;
    rightCmd = throttle - steering;

    // Constrain output to motor-safe range
    leftCmd  = constrain(leftCmd,  -255, 255);
    rightCmd = constrain(rightCmd, -255, 255);

    // Send commands to motors
    setMotor(L_speed, L_dir1, L_dir2, leftCmd);
    setMotor(R_speed, R_dir1, R_dir2, rightCmd);
}

void setMotor(int pwmPin, int d1, int d2, int cmd) {
    int speedVal = abs(cmd);

    if (cmd > 0) {// Forward
        digitalWrite(d1, HIGH);
        digitalWrite(d2, LOW);
    } 
    else if (cmd < 0) {// Reverse
        digitalWrite(d1, LOW);
        digitalWrite(d2, HIGH);
    } 
    else {// Stop
        digitalWrite(d1, LOW);
        digitalWrite(d2, LOW);
    }

    analogWrite(pwmPin, speedVal);
}