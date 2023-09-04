#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS 16 // Popular NeoPixel ring size

struct RGB {
  byte r;
  byte g;
  byte b;
};

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
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //inits
  Serial.begin(9600);
  setupEthernetShield();
  actualTime = millis();
  lastPutRequestTime = actualTime;
  lastGetRequestTime = actualTime;
  pixels.begin();
  //first functions call
  pixels.clear();
  get_request("/api/iot/token", 1);
}

void loop() 
{
  String response = "";
  RGB point;
  actualTime = millis();
  if(actualTime>=lastGetRequestTime+getRequestDelay)
  {
    response = get_request("/api/iot/token", 1);
    lastGetRequestTime = actualTime;

    switch (response.toInt())
    {
      case 12:
        response = get_request("/api/iot/token", 2); 
        point.r = response.toInt();
        response = get_request("/api/iot/token", 3); 
        point.g = response.toInt();
        response = get_request("/api/iot/token", 4); 
        point.b = response.toInt();
        set_monocolour(point);
        break;
      case 11:
        point.r = random(255);
        point.g = random(255);
        point.b = random(255);
        set_monocolour(point);
        break;
      default:
        break;
    }
  }
}
