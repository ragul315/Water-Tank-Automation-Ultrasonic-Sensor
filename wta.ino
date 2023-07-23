#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const char* ssid = "YOUR WIFI SSID";     
const char* password = "YOUR WIFI PASSWORD"; 

ESP8266WebServer server(80); 

const int trigPin = D2;
const int echoPin = D3;
const int motorrelay = D4;

long duration;
int distance;
float persent;
int mindist;
int level;
int  minper;
int maxper;
int mindistadd=1,minperadd=5,maxperadd=9;

void setup() {
  Serial.begin(115200);  
  pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(motorrelay, OUTPUT);
    EEPROM.get(mindistadd,mindist);
    if (mindist == NULL)
    {

       EEPROM.put(mindistadd,mindist);
    }
  WiFi.mode(WIFI_AP); 
  WiFi.softAP(ssid, password);  
  IPAddress apIP = WiFi.softAPIP(); 
  Serial.print("AP IP address: ");
  Serial.println(apIP);  
  server.on("/", pub); 
  server.on("/form", writenew);
  server.begin(); 
}

void loop() {
  server.handleClient(); 
  EEPROM.get(minperadd,level);
    Serial.println(level);
    if (level >= calper())
    {
        digitalWrite(motorrelay, HIGH);
    }
      EEPROM.get(maxperadd,level);
    Serial.println(level);
    if (level <= calper())
    {
        digitalWrite(motorrelay, LOW);
    }
}
void pub() {
    String html ="<!DOCTYPE html><head><title>Water tank</title></head><body><h3>Water Level :";
    html+=calper();
    html+="%</h3><form action='/form'><input type='number' name='minper' id='minper' placeholder='Minimum Level'><br><br><input type='number' name='maxper' id='maxper' placeholder='Maximum Level'><br><br><input type='submit' value='Change'> </form></body></html>";
    server.send(200, "text/html", html); 
}
void writenew(){
  minper=server.arg("minper").toInt(); 
  EEPROM.put(minperadd,minper);
  maxper=server.arg("maxper").toInt();
  EEPROM.put(maxperadd,maxper);
   
}

int cald
is() // distance calculator
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = duration * 0.034 / 2;

    return distance;
}
int calper() // persentage calculator
{
    persent = map(caldis(), mindist, 5, 0, 100);
    return persent;
}
