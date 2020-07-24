#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

String clientName = "";

ESP8266WebServer server(80); //Server on port 80
WiFiServer wifiServer(80);
WiFiClient client = wifiServer.available();

void setUpAccessPoint() {
    Serial.println('\n');
    WiFi.mode(WIFI_AP);  
    WiFi.softAP(clientName);
    IPAddress myIP = WiFi.softAPIP(); //Get IP address
    Serial.print("HotSpot IP:");
    Serial.println(myIP);
   
    server.begin();                  //Start server
    Serial.println("HTTP server started");
  
    server.on("/", handleRoot);      //Which routine to handle at root location
    server.on("/scan", HTTP_POST, scanNetworks);
    //server.on("/settings", HTTP_PUT, handleSettings);

}

String htmlPage =
            String("") +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "<ul>" +
            "<li>Scan Networks Available: /scan </li>" +
            "\r\n" +
            "<li>Set WiFi Connection: /settings?SSID=???&password=???&security=???&nickname=??? </li>" +
            "\r\n" +
            "<li>Feeding: /feed?cups=xxx </li>" +
            "</ul>" +
            "\r\n" +
            "</html>" +
            "\r\n";

void handleRoot() {
  server.send(200, "text/html", htmlPage); //Send web page
}

void scanNetworks() {
    delay(10000);

    int networks = WiFi.scanNetworks();
    String str = "";
    String fullString = "";
    
    str = "<ul>";
    for(int i = 0; i < networks; i++) {
        //Serial.println(WiFi.SSID(i));
        str += "<li>";
        str += i + 1;
        str += ": ";
        str += WiFi.SSID(i);
        str += "</li>";
    }
    str += "</ul>";

    fullString = "\n\r\n<!DOCTYPE HTML>\r\n<html><h1>WiFi List</h1> ";
    fullString += "<p>";
    fullString += str;
    fullString += "</thml>\r\n\r\n";

    server.send(200, "text/html", fullString);
    Serial.println();
    delay(10000);
}

void handleNotAccepted() {
  server.send(406, "text/plain", "Value not accepted");
}

void handleBadRequest() {
  server.send(400, "text/plain", "Bad Request");
}

void setup() {
    Serial.begin(115200);
    Serial.println();

    if(clientName == "") {
      clientName = "ESP8266 AP";
    }

    WiFi.mode(WIFI_STA);
    delay(1000);
    setUpAccessPoint();
}

void loop() {
    server.handleClient();
}
