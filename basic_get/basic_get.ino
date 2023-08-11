/*
Based on example from ArduinoHttpClient remake for Ethernet shield. It's basic concept to get data from DB using get request. Communication goes throught node-red as for arduino  it's impossible to connect properly to docker image. 
  */
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(0,0,0,0); //to fulfill
IPAddress dns(0,0,0,0); //to fulfill

char serverAddress[] = "0.0.0.0";  // server address - to fulfill
int port = 1880; //node red port

EthernetClient eth;
HttpClient client = HttpClient(eth, serverAddress, port);


void setup() {
  Serial.begin(9600);
  
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to configure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, dns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void loop() {
  Serial.println("making GET request");
  client.beginRequest();
  client.get("/api/iot/token"); //to fulfill with path to api
  client.sendHeader("token_val", "2"); //custom header is sent to be fetch and changed to url like server.ip/api/iot/token/2
  client.endRequest();


  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Wait five seconds");
  delay(5000);
}
