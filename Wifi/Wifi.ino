#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESPAsyncWebServer.h>

String ssid = "ESP8266 Access Point"; //Change by nickname
String networkSSID = "network";
String networkPassword = "password";
String networkSecurity = "security";
//char* networkNickname = "nickname";
byte state = 0;  //0=Hot Spot Mode  ::  1=Hot Spot Mode

ESP8266WebServer server(80); //Server on port 80
WiFiServer wifiServer(80);
WiFiClient client = wifiServer.available();

void setUpAccessPoint() {
  Serial.println('\n');
  WiFi.mode(WIFI_AP);  
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpot IP:");
  Serial.println(myIP);
 
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/settings", HTTP_GET, handleSettings);
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");

  state = 0;
}

//http://<ip address of feeder>/settings?[SSID=???]&[password=???]&[security=???]&[nickname=???]

void handleSettings() {
   networkSSID = server.arg("SSID");
   networkPassword = server.arg("password");
   networkSecurity = server.arg("security");
   ssid = server.arg("nickname");
//  String userSSID = server.arg("SSID");
//  String userPassword = server.arg("password");
//  String userSecurity = server.arg("security");
//  String userNickname = server.arg("nickname");

 // networkSSID = userSSID.toCharArray(255,255);
 // networkPassword = userPassword.toCharArray(255,255);
 // networkSecurity = userSecurity.toCharArray(255,255);
 // ssid = userNickname.toCharArray(255,255);
  
  }

void accessWifi()
{
  int connectionTimeout = 0;
  Serial.print("Connecting to  " + ssid); //Print out connecting to the WIFI network
  WiFi.begin(networkSSID, networkPassword); //Credentials
  //While the WIFI is not connected do the following:
  while (WiFi.status() != WL_CONNECTED && connectionTimeout < 7 ) 
  {
    delay(500);
    Serial.print(".");
    digitalWrite(0, HIGH);
    delay(500);
    digitalWrite(0, LOW);
    delay(500);
    connectionTimeout++;
  }
  if(WiFi.status() != WL_CONNECTED || networkSSID == "" || networkSSID == NULL) 
  {
    state = 0;
    delay(10000); //TODO: Delay might need to be extended at later time.
    return;
  }
  else
  {
    WiFi.softAPdisconnect (true);
    state = 1;
    Serial.println(" connected"); 
    Serial.printf(WiFi.localIP().toString().c_str());
  }
}

String htmlPage =
            String("") +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "Enter into the URL the WiFi settings in the following manner: http://<ip address of feeder>/settings?[SSID=???]&[password=???]&[security=???]&[nickname=???] " +
            "</html>" +
            "\r\n";

void handleRoot() {
  //String page = prepareHTMLPage;
  server.send(200, "text/html", htmlPage); //Send web page
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(0, OUTPUT);
  
  setUpAccessPoint();
  
}

void loop() {
  server.handleClient();
  if(state == 0 && networkSSID != "" && networkSSID != NULL)
  {
    accessWifi();
  } else if(state == 1 && WiFi.status() != WL_CONNECTED)
  {
    setUpAccessPoint();
  }
}
