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

// DHT Sensor
uint8_t DHTPin = 12; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature = 0.0;
float Humidity = 0.0;
 
void setup() {
  
  Serial.printf("testando ");
  Serial.begin(115200);
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
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void loop() {
  
  server.handleClient();
  
}

void handle_OnConnect() {

 Temperature = dht.readTemperature(); // Obtém os valores da temperatura
  Humidity = dht.readHumidity(); // Obtém os valores da umidade
  server.send(200, "text/html", SendHTML(Temperature,Humidity));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<link rel='stylesheet' href='style.css'>";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP8266 NodeMCU Weather Report</h1>\n";
  //
  ptr +="<p>Temperatura: ";
  ptr +=(float)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(float)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";

  ptr +="<label class='switch'>";
  ptr +="<input type='checkbox' checked>";
  ptr +="<span class='slider round'></span>";
  ptr +="</label>";
  
  ptr +="</body>\n";

//  ptr +="<script>";
//  ptr +="setTimeout(function(){";
//  ptr +="document.location.reload(true);";
//  ptr +="},5000);";
//  ptr +="</script>";
  
  ptr +="</html>\n";
  return ptr;
}
