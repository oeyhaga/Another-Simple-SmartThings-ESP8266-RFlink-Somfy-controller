//Based on the esp8266wifi example WifiWebserver
//Inspired by https://github.com/hobbzey/Simplest-SmartThings-ESP8266-Blinds

#include <ESP8266WiFi.h>

const char* ssid = "YOUR SSID IN HERE";
const char* password = "YOUR PASSWORD IN HERE";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(57600); //The baudrate used by rflink.
  delay(10);

  // Connect to WiFi network
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
    
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  //Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();

  // Match the request
  String val;
  if (req.indexOf("/up/") != -1) {
    val = "UP";
    Serial.println("10;RTS;0E0E0E;0;UP;");
    delay(1000);
  } else if (req.indexOf("/down/") != -1) {
    val = "DOWN";
    Serial.println("10;RTS;0E0E0E;0;DOWN;");
    delay(1000);
  } else if (req.indexOf("/stop/") != -1) {
    val = "STOPPED";
    Serial.println("10;RTS;0E0E0E;0;STOP;");
    delay(1000);
  } else {
    //Serial.println("invalid request");
    client.stop();
    return;
  }

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nScreen is now ";
  s += val;
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  //Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
