#include <WiFi.h>
#include <WebServer.h>
// #include <AsyncTCP.h>
#include <uri/UriBraces.h>

WebServer server(80);

const char WIFI_SSID[] = "space_program";
const char WIFI_PASSWORD[] = "538976";
const int test_pin = 21;

// NOTE: this is EXTREAMLY important, it should only be touched with good reason
// ensure that the robot is OFF while it cannot recieve commands
void disconect() {
    // TODO: make the thing go OFF!
    Serial.println("WIFI disconected");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    pinMode(test_pin, OUTPUT);
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
    // TODO: we cant detect if a client disconects verry easily
    // make shure that the client regularly polls the server.
    //
    // WiFi.onEvent(Disconect, ARDUINO_EVENT_WIFI_AP_DISCONNECTED)
    //
    // server.on("/control/a_button", HTTP_GET, [](ServerRequest* request){
    //     Serial.println("A button rising edge");
    //     a_button();
    //     server.send(204);
    // });
    server.on(UriBraces("/control/l_stick/{}/{}"), []() {
        String lstickx = server.pathArg(0);
        String lsticky = server.pathArg(1);
        server.send(204);
    });
    server.on(UriBraces("/control/a_button_off"), []() {
        Serial.println("A button falling edge");
        server.send(204);
    });
    // server.on("/control/a_button_off", HTTP_GET, [](AsyncWebServerRequest* request){
    //     Serial.println("A button falling edge");
    //     a_button_off();
    //     server.send(204);
    //     // request->send(200, "text/plain", "Hello, world");
    //     // request->send(204);
    // });
    // server.on("/control/l_stick", HTTP_GET, [](AsyncWebServerRequest* request){
    //     if (request->hasParam("x") && (request->hasParam("y"))) {
    //         float x = request->getParam("x")->value().toFloat();
    //         float y = request->getParam("y")->value().toFloat();
    //         Serial.printf("Left stick: x=%.2f, y=%.2f\n", x, y);
    //         Serial.println("A button falling edge");
    //         request->send(204);
    //     }
    // });

}

void loop() {
    // put your main code here, to run repeatedly:
    server.handleClient();
    delay(2);
}
