#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "DHT.h"
#define DHTTYPE DHT22

// DHT Sensor
uint8_t DHTPin = 12; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "ANDERSON";
const char* password = "12121212";

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

float Temperature = Temperature = dht.readTemperature(); // Obtém os valores da temperatura;
float Humidity = Humidity = dht.readHumidity(); // Obtém os valores da umidade;




// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  
  <form action="/get">
  <p>Tipo: </p>
    <input type="radio" value="1" name="input1">FRUTAS <br>
    <input type="radio" value="2" name="input1">LEGUMES <br>
   <p>Velocidade: </p>
    <input type="radio" value="3" name="input2">RAPIDO <br>
    <input type="radio" value="4" name="input2">NORMAL <br>
    <input type="submit" value="Submit">
  </form><br>
  
  <form action="/get">
    input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
  
  <form action="/get">
    input3: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
  
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  pinMode(DHTPin, INPUT);
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  dht.begin();
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputMessage2;
    String inputParam;
    String inputParam2;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
      inputMessage2 = "No message sent";
      inputParam2 = "none";
    }
    Serial.println(inputMessage);
    Serial.println(inputMessage2);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage + "<br>" +
                                     "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam2 + ") with value: " + inputMessage2 +
                                     "<br><a href=\"/\">Return to Home Page</a><br>"+
                                     Temperature + "<br>" +
                                     Humidity);
  });
  
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
//  server.handleClient();
  
}
