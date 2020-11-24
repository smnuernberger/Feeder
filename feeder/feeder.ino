#include <SD.h>
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
String setTime = "";

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
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Can not connect.  Please check WiFi name and/or password.");
    delay(1000); //TODO: Delay might need to be extended at later time. 
    ssid = "";
    password = "";
    WiFi.disconnect(true); 
    setUpAccessPoint();
    return;
  } else {
    WiFi.softAPdisconnect (true);
    Serial.println(" connected"); 
    state = 1; 
    Serial.printf(WiFi.localIP().toString().c_str());
    delay(5000);
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

void handleNotAccepted() {
  server.send(406, "text/plain", "Value not accepted");
}

void handleBadRequest() {
  server.send(400, "text/plain", "Bad Request");
}

void printNetworks() {
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
    //Serial.println(tempArray[i]);
  }
  clientName = String(tempArray[0]);
  ssid = String(tempArray[1]);
  password = String(tempArray[2]);
}

void sendToMemory() {
  writeValue(clientName);
  writeValue(ssid);
  writeValue(password);
  EEPROM.commit();
}

void handleSettings() {
  if(server.hasArg("ssid")) {
    ssid = server.arg("ssid");
    password = server.arg("password");
    if(server.hasArg("nickname")) {
      clientName = server.arg("nickname");
    }
    sendToMemory();
    server.send(202);
    accessWifi();
    } else {
      handleBadRequest();   
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

String padTime(String userTime) { 
  userTime = "0" + userTime;
  Serial.println("PadTime Function");
  return userTime;
}

byte isValidTime(String userTime) { //False = 0; True = 1
  if(userTime.length() > 5 || userTime.length() < 4) {
    Serial.println(">5 or < 4");
    return 0;
  } else if(userTime.length() == 4) {
    Serial.println("Sent to padTime()");
    userTime = padTime(userTime);
  }
  int hour, minute; 
     hour = userTime.substring(0,2).toInt();
     minute = userTime.substring(3).toInt();
     if(userTime[2] = ':' && hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 ) {
      Serial.println(hour);
      Serial.println(minute);
      Serial.println(setTime);
      Serial.println(userTime);
      setTime = userTime;
      return 1;
     } else {
      Serial.println("Bad Line 229");
      handleBadRequest(); 
      return 0;
     }
}

// schedule?cups=##&time=???`
void handleSchedule() {
  setTime = "";
  amountDispense = NULL;
  if(server.hasArg("cups") && server.hasArg("time")) {
     String tempTime = server.arg("time");
     byte validTime = isValidTime(tempTime);
     if(validTime == 1){
      handleFeeding();
      Serial.println("Will Write to SD Card");
     }else {
      handleNotAccepted();
     }    
  } else {
    handleBadRequest();
  }
}

void handleReset() {
  for (int i = 0 ; i < 256 ; i++) {
    EEPROM.write(i, 0);  
  }
  EEPROM.commit();
  server.send(205);
  clientName = "ESP8266 AP";
  ssid = "";
  password = "";
  sendToMemory();
  WiFi.disconnect(true); 
  readValue();
  setUpAccessPoint();
  //ESP.restart();
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(256);
  Serial.println();

  readValue();

  if(clientName == "" || clientName.length() > 32) {
    clientName = "ESP8266 AP";
    ssid="";
    password="";
  }

  WiFi.mode(WIFI_STA);
  delay(1000);
  setUpAccessPoint();
  accessWifi();
  
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/scan", HTTP_POST, printNetworks);
  server.on("/settings", HTTP_PUT, handleSettings);
  server.on("/settings", HTTP_DELETE, handleReset);
  server.on("/feed", HTTP_POST, handleFeeding);
  server.on("/schedule", HTTP_POST, handleSchedule);
}

void loop() {
  server.handleClient();
  if(state == 0 && (ssid != "" || ssid != NULL)) {
    accessWifi();
    Serial.println(" LINE 200  ");
  } else if(WiFi.status() != WL_CONNECTED && (WiFi.getMode() & WIFI_AP) == 0) {
    Serial.println( " LINE 204  ");
    setUpAccessPoint();
  }
}
