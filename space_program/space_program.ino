#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char WIFI_SSID[] = "space_program";
const char WIFI_PASSWORD[] = "538976";

// NOTE: this is EXTREAMLY important, it should only be touched with good reason
// ensure that the robot is OFF while it cannot recieve commands
void Disconected() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void setup() {
    // put your setup code here, to run once:
    // TODO: setup wifi
    // - connect to network
    // - setup disconect callback
    //     - stop if not connected
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.onEvent()

    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.print("\n");

    server.on("/control/a_button", a_button);
    //

}

void loop() {
    // put your main code here, to run repeatedly:
}
