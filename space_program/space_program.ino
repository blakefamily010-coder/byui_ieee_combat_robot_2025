#include <WiFi.h>
#include <WebServer.h>
// #include <AsyncTCP.h>
#include <uri/UriBraces.h>
#include <esp_task_wdt.h>
// #include <Servo.h>

WebServer server(80);

const char WIFI_SSID[] = "esp32test";
const char WIFI_PASSWORD[] = "test1234";

// TODO: rename to more clear names
const uint8_t L_speed = 27; // L motor PWM
const uint8_t L_dir1 = 25; // L motor direction control 1
const uint8_t L_dir2 = 26; // L motor direction control 2
const uint8_t R_speed = 21; // R motor PWM
const uint8_t R_dir1 = 20; // R motor direction control 1
const uint8_t R_dir2 = 19; // R motor direction control 2
const uint8_t W_motor_PWM = 5;
const uint8_t W_dir1 = 18;
const uint8_t W_dir2 = 19;
const uint8_t test_pin = 12;
// TODO: add control pin


const esp_task_wdt_config_t watchdog_config(10, false);
// Servo ESC;     // create servo object to control the ESC

void disconect() {
    // TODO: make the thing go OFF!
    Serial.println("WIFI disconected");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // pinMode(test_pin, OUTPUT);
}
void connect_poll() {
    esp_task_wdt_reset();
}
void handle_wifi_events(WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            delay(200)
            Serial.print("ip addr: ");
            Serial.println(WiFi.localIP());
    }
}

void setup() {
    // put your setup code here, to run once:
    // TODO: setup wifi
    // - connect to network
    // - setup disconect callback
    //     - stop if not connected
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.onEvent(handle_wifi_events);

    // NOTE: this is EXTREAMLY important, it should only be touched with good reason
    // ensure that the robot is OFF while it cannot recieve commands
    //
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
        Serial.println("test");

        int lstickx = 0;
        int lsticky = 0;
        // sscanf(&server.pathArg(0), "%x", &lstickx);
        // sscanf(&server.pathArg(1), "%x", &lsticky);
        lstickx = server.pathArg(0).toInt();
        lsticky = server.pathArg(1).toInt();

        movementControl(lstickx, lsticky);

        server.send(204);
    });
    server.on(UriBraces("/control/l_trigger/{}"), []() {
        Serial.println("test1");
        int l_trigger = server.pathArg(0).toInt();
        weapon_control(l_trigger);

    });
    server.on(UriBraces("/control/a_button_off"), []() {
        Serial.println("A button falling edge");
        movementControl(W_motor_PWM, W_dir1, W_dir2, 255);
        server.send(204);
    });
    server.on(UriBraces("/control/a_button"), []() {
        Serial.println("A button rising edge");
        movementControl(W_motor_PWM, W_dir1, W_dir2, 0);
        server.send(204);
    });

    pinMode(L_speed, OUTPUT);
    pinMode(L_dir1, OUTPUT);
    pinMode(L_dir2, OUTPUT);

    pinMode(R_speed, OUTPUT);
    pinMode(R_dir1, OUTPUT);
    pinMode(R_dir2, OUTPUT);

    pinMode(W_motor_PWM, OUTPUT);
    pinMode(W_dir1, OUTPUT);
    pinMode(W_dir2, OUTPUT);
    
    server.begin();

}

void loop() {
    // put your main code here, to run repeatedly:
    server.handleClient();
    delay(20);
    esp_task_wdt_reset();
}

void weapon_control(int l_trigger) {

    int motor_cmd = map(l_trigger, 0, 1023, -255, 255);
    setMotor(W_motor_PWM, W_dir1, W_dir2, motor_cmd);

}

void movementControl(int joy1_X_Value, int joy1_Y_Value) {
    // Convert to -255..255 ranges
    // may be -2048 / 2047
    //
    int throttle = map(joy1_Y_Value, -2048, 2047, -255, 255);   // forward/back
    int steering = map(joy1_X_Value, -2048, 2047, -255, 255);   // left/right

    // Differential motor mixing
    int leftCmd  = throttle + steering;
    int rightCmd = throttle - steering;

    // Constrain output to motor-safe range
    leftCmd  = constrain(leftCmd,  -255, 255);
    rightCmd = constrain(rightCmd, -255, 255);

    // Send commands to motors
    setMotor(L_speed, L_dir1, L_dir2, leftCmd);
    setMotor(R_speed, R_dir1, R_dir2, rightCmd);
}

void setMotor(int pwmPin, int d1, int d2, int cmd) {
    uint8_t speedVal = abs(cmd);

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
