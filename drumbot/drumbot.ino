#include <WiFi.h>
#include <WebServer.h>
// #include <AsyncTCP.h>
#include <uri/UriBraces.h>
#include <esp_task_wdt.h>
#include <Servo.h>

WebServer server(80);

const char WIFI_SSID[] = "space_program";
const char WIFI_PASSWORD[] = "538976";

// TODO: rename to more clear names
const uint8_t L_speed = 27; // L motor PWM
const uint8_t L_dir1 = 25; // L motor direction control 1
const uint8_t L_dir2 = 26; // L motor direction control 2
const uint8_t R_speed = 14; // R motor PWM
const uint8_t R_dir1 = 12; // R motor direction control 1
const uint8_t R_dir2 = 13; // R motor direction control 2
const uint8_t W_motor_SIGNAL = 5;
// TODO: add control pin


const esp_task_wdt_config_t watchdog_config(1, false);
Servo ESC;     // create servo object to control the ESC

// NOTE: this is EXTREAMLY important, it should only be touched with good reason
// ensure that the robot is OFF while it cannot recieve commands
void disconect() {
    // TODO: make the thing go OFF!
    Serial.println("WIFI disconected");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    pinMode(test_pin, OUTPUT);
}
void connect_poll() {
    esp_task_wdt_reset();
}

void setup() {
    // put your setup code here, to run once:
    // TODO: setup wifi
    // - connect to network
    // - setup disconect callback
    //     - stop if not connected
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // start watchdog
    esp_task_wdt_init(&watchdog_config); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch
    server.on(UriBraces("/connect"), []() {
        connect_poll();
    });
    //
    // WiFi.onEvent(Disconect, ARDUINO_EVENT_WIFI_AP_DISCONNECTED)
    //
    server.on(UriBraces("/control/l_stick/{}/{}"), []() {

        int lstickx = 0;
        int lsticky = 0;
        sscanf(server.pathArg(0), "%x", &lstickx);
        sscanf(server.pathArg(1), "%x", &lsticky);

        movementControl(lstickx, lsticky);

        server.send(204);
    });
    server.on(UriBraces("/control/a_button_off"), []() {
        Serial.println("A button falling edge");
        a_button_off();
        server.send(204);
    });
    server.on(UriBraces("/control/a_button"), []() {
        Serial.println("A button falling edge");
        a_button();
        server.send(204);
    });
    // Attach the ESC on pin 9
    ESC.attach(W_motor_SIGNAL,1000,2000); // (pin, min pulse width, max pulse width in microseconds)

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
    // put your main code here, to run repeatedly:
    server.handleClient();
    delay(2);
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
