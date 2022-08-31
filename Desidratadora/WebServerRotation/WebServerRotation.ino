//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
//
///* Put your SSID & Password */
//const char* ssid = "NodeMCU";  // Enter SSID here
//const char* password = "12345678";  //Enter Password here
//
///* Put IP Address details */
//IPAddress local_ip(192,168,1,1);
//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,255,0);
//
//ESP8266WebServer server(80);
//
//uint8_t LED1pin = D7;
//bool LED1status = LOW;
//
//uint8_t LED2pin = D6;
//bool LED2status = LOW;
//
//void setup() {
//  Serial.begin(115200);
//  pinMode(LED1pin, OUTPUT);
//  pinMode(LED2pin, OUTPUT);
//
//  WiFi.softAP(ssid, password);
//  WiFi.softAPConfig(local_ip, gateway, subnet);
//  delay(100);
//  
//  server.on("/", handle_OnConnect);
//  server.on("/led1on", handle_led1on);
//  server.on("/led1off", handle_led1off);
//  server.on("/led2on", handle_led2on);
//  server.on("/led2off", handle_led2off);
//  server.onNotFound(handle_NotFound);
//  
//  server.begin();
//  Serial.println("HTTP server started");
//}
//void loop() {
//  server.handleClient();
//  if(LED1status)
//  {digitalWrite(LED1pin, HIGH);}
//  else
//  {digitalWrite(LED1pin, LOW);}
//  
//  if(LED2status)
//  {digitalWrite(LED2pin, HIGH);}
//  else
//  {digitalWrite(LED2pin, LOW);}
//}
//
//void handle_OnConnect() {
//  LED1status = LOW;
//  LED2status = LOW;
//  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
//  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
//}
//
//void handle_led1on() {
//  LED1status = HIGH;
//  Serial.println("GPIO7 Status: ON");
//  server.send(200, "text/html", SendHTML(true,LED2status)); 
//}
//
//void handle_led1off() {
//  LED1status = LOW;
//  Serial.println("GPIO7 Status: OFF");
//  server.send(200, "text/html", SendHTML(false,LED2status)); 
//}
//
//void handle_led2on() {
//  LED2status = HIGH;
//  Serial.println("GPIO6 Status: ON");
//  server.send(200, "text/html", SendHTML(LED1status,true)); 
//}
//
//void handle_led2off() {
//  LED2status = LOW;
//  Serial.println("GPIO6 Status: OFF");
//  server.send(200, "text/html", SendHTML(LED1status,false)); 
//}
//
//void handle_NotFound(){
//  server.send(404, "text/plain", "Not found");
//}
//
//String SendHTML(uint8_t led1stat,uint8_t led2stat){
//  String ptr = "<!DOCTYPE html> <html>\n";
//  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
//  ptr +="<title>LED Control</title>\n";
//  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
//  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
//  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
//  ptr +=".button-on {background-color: #1abc9c;}\n";
//  ptr +=".button-on:active {background-color: #16a085;}\n";
//  ptr +=".button-off {background-color: #34495e;}\n";
//  ptr +=".button-off:active {background-color: #2c3e50;}\n";
//  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
//  ptr +="</style>\n";
//  ptr +="</head>\n";
//  ptr +="<body>\n";
//  ptr +="<h1>ESP8266 Web Server</h1>\n";
//  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
//  
//   if(led1stat)
//  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
//  else
//  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}
//
//  if(led2stat)
//  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
//  else
//  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}
//
//  ptr +="</body>\n";
//  ptr +="</html>\n";
//  return ptr;
//}




/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor
uint8_t DHTPin = 12;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

float Temperatura = 0.0;
float Humidade = 0.0;

// Replace with your network credentials
const char* ssid     = "LUIZ";
const char* password = "12341234";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";


// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;
const int tipo = 0;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(DHTPin, INPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /?tipo=1") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
              Temperatura = dht.readTemperature(); // Obtém os valores da temperatura
              Humidade = dht.readHumidity(); // Obtém os valores da umidade
            } else if (header.indexOf("GET /?tipo=2") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /?tipo=3") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /?tipo=4") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            } else{
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");

  client.println("<form action=\"\" class=\"tipo\" metohode=\"get\">");
  client.println("<input type=\"radio\" value=\"1\" name=\"tipo\">");
  client.println("<span>FRUTAS NORMAL</span><br>");
  client.println("<input type=\"radio\" value=\"2\" name=\"tipo\">");
  client.println("<span>FRUTAS RAPIDO</span><br>");
  client.println("<input type=\"radio\" value=\"3\" name=\"tipo\">");
  client.println("<span>LEGUMES NORMAL</span><br>");
  client.println("<input type=\"radio\" value=\"4\" name=\"tipo\">");
  client.println("<span>LEGUMES RAPIDO</span><br><br><br>");
  if (output5State=="off") {
              client.println("<p><a href=\"/on\"><button type=\"submit\" class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/off\"><button type=\"submit\" class=\"button button2\">OFF</button></a></p>");
            }
  client.println("</form>");
//            client.println("<p>"+(char(Temperatura))+"</p>");
//            client.println("<p>"+(char(Humidade+))"</p>");
            // Display current state, and ON/OFF buttons for GPIO 5  
//            client.println("<p>GPIO 5 - State " + output5State + "</p>");
//            // If the output5State is off, it displays the ON button       
//            if (output5State=="off") {
//              client.println("<p><a href=\"/on\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/off\"><button class=\"button button2\">OFF</button></a></p>");
//            } 
//               
//            // Display current state, and ON/OFF buttons for GPIO 4  
//            client.println("<p>GPIO 4 - State " + output4State + "</p>");
//            // If the output4State is off, it displays the ON button       
//            if (output4State=="off") {
//              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
//            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
