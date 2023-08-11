#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <Ethernet.h>

//config data
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //arduino MAC
IPAddress ip(192, 168, 50, 200); //arduino ip
char serverAddress[] = "192.168.50.100";  // server address
int port = 1880; //node-red port

//timers
unsigned long lastPutRequestTime, lastGetRequestTime, actualTime;
long putRequestDelay = 9000;
long getRequestDelay = 4000;

//semaphore to block request until response is obtained
bool semaphore_requestBlock = false;

EthernetClient eth;
HttpClient client = HttpClient(eth, serverAddress, port);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  setupEthernetShield();
  actualTime = millis();
  lastPutRequestTime = actualTime;
  lastGetRequestTime = actualTime;
  get_request("/api/iot/token", 1);
}

void loop() 
{
  actualTime = millis();
  if(actualTime>=lastPutRequestTime+putRequestDelay)
  {
    post_request("/api/readings", 1, random(0, 300)/10.0);
    lastPutRequestTime = actualTime;
  }
  if(actualTime>=lastGetRequestTime+getRequestDelay)
  {
    get_request("/api/iot/token", 1);
    lastGetRequestTime = actualTime;
  }
}
