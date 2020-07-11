#include <ESP8266WiFi.h>

const char* ssid = "ESP8266 Access Point"; //TODO: Hide later
const char* network = "Cicso05019-guest";
const char* networkPassword = "MarTesla";
//const char* password = "Jax"; //TODO: Hide later
byte state = 0;  //0=Hot Spot Mode  ::  1=Hot Spot Mode

WiFiServer server(80);
WiFiClient client = server.available();

void setUpAccessPoint()
{
  Serial.println('\n');
  WiFi.softAP(ssid);
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
  state = 0;
}

void startWebserver()
{
  
  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
      }
    }
    delay(1); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("[Client disonnected]");
  }
}


void accessWifi()
{
  int connectionTimeout = 0;
  Serial.printf("Connecting to %s ", network); //Print out connecting to the WIFI network
  WiFi.begin(network, networkPassword); //Credentials
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
  if(WiFi.status() != WL_CONNECTED) 
  {
    state = 0;
    delay(10000); //TODO: Delay might need to be extended at later time.
    return;
  }
  else
  {
    WiFi.softAPdisconnect (true);
    state = 1;
    Serial.println(" connected"); //Displayed if connected
    Serial.printf(WiFi.localIP().toString().c_str());
  }
}

byte getState()
{
  if(WiFi.status() != WL_CONNECTED) 
  {
    return 0;
  }
  else
  {
    return 1; 
  }
}

String prepareHtmlPage()
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "It is connected!  Congrats you did it! =] " +
            "</html>" +
            "\r\n";
  return htmlPage;
}

void setup()
{
  Serial.begin(115200); //What is this?
  Serial.println();
  pinMode(0, OUTPUT);
  
  setUpAccessPoint();
}

void loop()
{
  
  if(state == 0)
  {
    accessWifi();
  } else if(state == 1 && WiFi.status() != WL_CONNECTED)
  {
    setUpAccessPoint();
  }
}
