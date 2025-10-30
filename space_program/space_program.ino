#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char WIFI_SSID[] = "space_program";
const char WIFI_PASSWORD[] = "538976";

// NOTE: this is EXTREAMLY important, it should only be touched with good reason
// ensure that the robot is OFF while it cannot recieve commands
void Disconect() {
    // TODO: make the thing go OFF!
    Serial.println("WIFI disconected");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void setup() {
    // put your setup code here, to run once:
    // TODO: setup wifi
    // - connect to network
    // - setup disconect callback
    //     - stop if not connected
    Serial.begin(115200);
    WiFi.mode(WIFI_AP);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // TODO: we cant detect if a client disconects verry easily
    // make shure that the client regularly polls the server.
    //
    // WiFi.onEvent(Disconect, ARDUINO_EVENT_WIFI_AP_DISCONNECTED)


    server.on("/poll", poll); // TODO:
    server.on("/control/a_button", a_button);
    server.on("/control/a_button_off", a_button);
    server.on("/control/b_button", b_button);
    server.on("/control/b_button_off", b_button);
    server.on("/control/x_button", x_button);
    server.on("/control/x_button_off", x_button);
    server.on("/control/y_button", y_button);
    server.on("/control/y_button_off", y_button);
    server.on("/control/r_bumper", r_bumper);
    server.on("/control/r_bumper_off", r_bumper_off);
    server.on("/control/l_bumper", l_bumper);
    server.on("/control/l_bumper_off", l_bumper_off);
    //

}

void loop() {
    // put your main code here, to run repeatedly:
}
