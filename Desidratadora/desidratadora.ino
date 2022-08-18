#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/*Put your SSID & Password*/
const char* ssid = "ANDERSON";  // Enter SSID here
const char* password = "12121212";  //Enter Password here

ESP8266WebServer server(80);

uint8_t RELEpin = D7;
bool RELEstatus = LOW;

// DHT Sensor
uint8_t DHTPin = 12; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature = 0.0;
float Humidity = 0.0;
 
void setup() {
  
  Serial.printf("testando ");
  Serial.begin(115200);
  pinMode(RELEpin, OUTPUT);
  delay(100);
  pinMode(DHTPin, INPUT);
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);

  dht.begin();              
  Serial.println("Conectado a rede ");
  Serial.println(ssid);

  //conectar-se à sua rede wi-fi local
  WiFi.begin(ssid, password);

  //verificar wi-fi está conectado à rede wi-fi
  while (WiFi.status() != WL_CONNECTED) {
  delay(100);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_releon);
  server.on("/led1off", handle_releoff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void loop() {
  server.handleClient();
  if(RELEstatus)
  {digitalWrite(RELEpin, HIGH);}
  else
  {digitalWrite(RELEpin, LOW);}
  
}

void handle_OnConnect() {
  RELEstatus = LOW;
  Temperature = dht.readTemperature(); // Obtém os valores da temperatura
  Humidity = dht.readHumidity(); // Obtém os valores da umidade
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(Temperature,Humidity,RELEstatus));
}

void handle_releon() {
  RELEstatus = HIGH;
  Temperature = dht.readTemperature(); // Obtém os valores da temperatura
  Humidity = dht.readHumidity(); // Obtém os valores da umidade
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Temperature,Humidity,true)); 
}

void handle_releoff() {
  RELEstatus = LOW;
  Temperature = dht.readTemperature(); // Obtém os valores da temperatura
  Humidity = dht.readHumidity(); // Obtém os valores da umidade
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Temperature,Humidity,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat,uint8_t relestat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<link rel='stylesheet' href='style.css'>";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +=".tipo {text-align: left; margin: auto;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  
  ptr +="<h1>ESP8266 NodeMCU Dehydrator</h1>\n";

  ptr +="<div class='tipo'>\n";
  ptr +="<input type='radio' value='1' name='tipo'>\n";
  ptr +="<span>FRUTAS NORMAL</span><br>\n";
  ptr +="<input type='radio' value='2' name='tipo'>\n";
  ptr +="<span>FRUTAS RAPIDO</span><br>\n";
  ptr +="<input type='radio' value='3' name='tipo'>\n";
  ptr +="<span>LEGUMES NORMAL</span><br>\n";
  ptr +="<input type='radio' value='4' name='tipo'>\n";
  ptr +="<span>LEGUMES RAPIDO</span><br><br><br>\n";
  ptr +="</div>\n";
  
  if(relestat)
  {ptr +="<p>RELE Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>RELE Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  ptr +="<p>Temperatura: ";
  ptr +=(float)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(float)Humiditystat;
  ptr +="%</p>";
  ptr +="</body>\n";

  ptr +="<script>";
  ptr +="setTimeout(function(){";
  ptr +="document.location.reload(true);";
  ptr +="},5000);";
  ptr +="</script>";
  
  ptr +="</html>\n";
  return ptr;
}
