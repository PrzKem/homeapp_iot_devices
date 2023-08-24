#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <Ethernet.h>

//config data
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //arduino MAC
IPAddress ip(); //arduino ip
char serverAddress[] = "";  // server address
int port = ; //node-red port

//timers
unsigned long lastPostRequestTime, lastGetRequestTime, actualTime;
long postRequestDelay = 4000;
long getRequestDelay = 3000;

//semaphore to block request until response is obtained
bool semaphore_requestBlock = false;

//other data
int actualWorkMode = 2; //1-auto, 2-manu

EthernetClient eth;
HttpClient client = HttpClient(eth, serverAddress, port);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  setupEthernetShield();
  actualTime = millis();
  lastPostRequestTime = actualTime;
  lastGetRequestTime = actualTime;
  get_request("/api/iot/token", 1);
}

void loop() 
{
  actualTime = millis();
  if(actualTime>=lastPostRequestTime+postRequestDelay)
  {
    post_sensor_reading("/api/readings", 1, random(0, 300)/10.0);
    post_sensor_reading("/api/readings", 2, random(0, 300)/10.0);
    post_keep_alive(2,"Arduino led controller", "sypialnia");
    lastPostRequestTime = actualTime;
  }
  if(actualTime>=lastGetRequestTime+getRequestDelay)
  {
    get_request("/api/iot/token", 1);
    lastGetRequestTime = actualTime;
  }
}
