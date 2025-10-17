#include <WiFi.h>
#include <index.h>

WiFiServer server(80);
const char WIFI_SSID[] = "space_program";
const char WIFI_PASSWORD[] = "538976";

void handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  server.sendResponse(client, HTML_CONTENT);
}

void setup() {
  // put your setup code here, to run once:
  // TODO: setup wifi
  // - connect to network
  // - setup disconect callback
  //     - stop if not connected

}

void loop() {
  // put your main code here, to run repeatedly:

}
