#include <WiFi.h>
#include <WebServer.h>
// #include <AsyncTCP.h>
#include <uri/UriBraces.h>
#include <esp_task_wdt.h>

WebServer server(80);

const char WIFI_SSID[] = "space_program";
const char WIFI_PASSWORD[] = "538976";
const int test_pin = 21;
const esp_task_wdt_config_t watchdog_config(1, false);

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
    // TODO: we cant detect if a client disconects verry easily
    // make shure that the client regularly polls the server.
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
        String lstickx = server.pathArg(0);
        String lsticky = server.pathArg(1);
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

}

void loop() {
    // put your main code here, to run repeatedly:
    server.handleClient();
    delay(2);
}
