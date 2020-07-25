#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

String clientName = "";
String ssid = "";
String password = "";
byte state = 0;
float amountDispense = NULL;
const double minFoodAmount = 0.125;

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

  state = 0;

  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/scan", HTTP_POST, scanNetworks);
  server.on("/settings", HTTP_PUT, handleSettings);
}

void accessWifi() {
  int connectionTimeout = 0;
  Serial.print("Connecting to  " + ssid); //Print out connecting to the WIFI network
  WiFi.begin(ssid, password); //Credentials

  while (WiFi.status() != WL_CONNECTED && connectionTimeout < 7 ) {
    delay(500);
    Serial.print(".");
    digitalWrite(0, HIGH);
    delay(500);
    digitalWrite(0, LOW);
    delay(500);
    connectionTimeout++;
  }
  if(WiFi.status() != WL_CONNECTED || ssid == "" || ssid == NULL) {
    delay(10000); //TODO: Delay might need to be extended at later time.
    return;
  } else {
    WiFi.softAPdisconnect (true);
    Serial.println(" connected"); 
    state = 1; 
    Serial.printf(WiFi.localIP().toString().c_str());
    delay(5000);
    EEPROM.commit();
    EEPROM.end();
  }
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

int addressLocation = 0;
void writeValue(String writeString) {
  int stringLength = writeString.length();
  for(int i = 0; i < stringLength; i++) {
    EEPROM.write(addressLocation + i, writeString[i]);
  } 
  addressLocation += stringLength;
  EEPROM.write(addressLocation, 0);
  addressLocation += 1; 
}

void readValue() {
  delay(10);
  char tempArray[][64] = {"","",""};
  int number = 0;
  for(int i = 0; i < 3; i++) {
   for(int j = 0; j < 64; j++) {
      char checkValue = EEPROM.read(number);
      if(checkValue == 0) {
        number++;
        break;
      }
      tempArray[i][j] = checkValue;
      number ++;
    }
    Serial.println(tempArray[i]);
  }
  clientName = String(tempArray[0]);
  ssid = String(tempArray[1]);
  password = String(tempArray[2]);
}

void sendToMemory() {
  writeValue(clientName);
  writeValue(ssid);
  writeValue(password);
}

void handleSettings() {
  if(server.hasArg("ssid") && server.hasArg("password")) {
    ssid = server.arg("ssid");
    password = server.arg("password");
    if(server.hasArg("nickname")) {
      clientName = server.arg("nickname");
    }
    sendToMemory();
    server.send(202);
    accessWifi();
  } else {
      if(server.hasArg("ssid") || server.hasArg("password")) {
        handleBadRequest();   
      }
  }
  if(server.hasArg("nickname") && !server.hasArg("ssid") && !server.hasArg("password")) {
    clientName = server.arg("nickname");
    sendToMemory();
  }
}

void handleFeeding() {
  double userAmount = server.arg("cups").toDouble();
  if(userAmount == NULL){
    userAmount = 0;
  }

  if(userAmount > 0) {
      amountDispense = userAmount / minFoodAmount;
      if(!fmod(amountDispense, 1)) {
        server.send(204);
        Serial.println(amountDispense);
        
    } else {
      handleNotAccepted();
    }
  } else {
    handleNotAccepted();
  }
}

void handleSchedule() {
  
}

void handleNotAccepted() {
  server.send(406, "text/plain", "Value not accepted");
}

void handleBadRequest() {
  server.send(400, "text/plain", "Bad Request");
}

void handleReset() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(256);
  Serial.println();

  readValue();

  if(clientName == "" || clientName.length() > 32) {
    clientName = "ESP8266 AP";
  }

  WiFi.mode(WIFI_STA);
  delay(1000);
  setUpAccessPoint();
  accessWifi();

  server.on("/feed", HTTP_POST, handleFeeding);
  server.on("/schedule", HTTP_POST, handleSchedule);
  server.on("/schedule", HTTP_DELETE, handleReset);
}

void loop() {
  server.handleClient();
  if(state == 0 && (ssid != "" || ssid != NULL) && (password != "" || password != NULL)) {
    accessWifi();
    Serial.println(" LINE 200  ");
  } else if(WiFi.status() != WL_CONNECTED) {
    Serial.println( " LINE 204  ");
    setUpAccessPoint();
  }
}
