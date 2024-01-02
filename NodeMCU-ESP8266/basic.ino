#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Adafruit_Sensor.h>
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor
const int DHTPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

ESP8266WebServer server(80);


String strGetValue(){

  
  String sym;
  float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      //Serial.println("Failed to read from DHT sensor!");
      //strcpy(celsiusTemp,"Failed");
      //strcpy(fahrenheitTemp, "Failed");
      //strcpy(humidityTemp, "Failed");    
      sym = "Failed to read from DHT sensor!";
    }
    else{
      float hic = dht.computeHeatIndex(t, h, false);       
      dtostrf(hic, 6, 2, celsiusTemp);             
      float hif = dht.computeHeatIndex(f, h);
      dtostrf(hif, 6, 2, fahrenheitTemp);         
      dtostrf(h, 6, 2, humidityTemp);

      //client.println("<!DOCTYPE HTML>");
      //client.println("<html>");
      //client.println("<head></head><body><h1>ESP8266 - Temperature and Humidity</h1><h3>Temperature in Celsius: ");
      //client.println(celsiusTemp);
      //client.println("*C</h3><h3>Temperature in Fahrenheit: ");
      //client.println(fahrenheitTemp);
      //client.println("*F</h3><h3>Humidity: ");
      //client.println(humidityTemp);
     //client.println("%</h3><h3>");
     //client.println("</body></html>"); 
     sym = "<br> Celsius: "+String(hic)+"<br>" + "fahrenheitTemp: "+String(hif)+"<br>" + "humidityTemp: "+String(h)+"<br>";
    }
    return sym;
}

String webPage =
{
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "   <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"
  "  <title>Điều khiển thiết bị</title>"
  "  <meta name='viewport' content='width=device-width, initial-scale=1'>"
  "  <style>"
  "    .b{width: 100px;height: 40px;font-size: 21px;color: #FFF;background-color:#4caf50;border-radius: 10px;}"
  "    .t{width: 100px;height: 40px;font-size: 21px;color: #FFF;background-color:#f44336;border-radius: 10px;}"
  "  </style>"
  "</head>"
  "<body>"
  "<div style='width: 330px;height: auto;margin: 0 auto;margin-top: 70px'>"
  "<h1 align='center'>Điều khiển thiết bị qua WIFI</h1>"
  "  <table align='center'>"
  "    <tr>"
  "    <td><a href='/bat1'><button class='b'>Bật 1</button></a><td>"
  "    <td><a href='/tat1'><button class='t'>Tắt 1</button></a><td>"
  "    <tr>"
  "    <tr>"
  "    <td><a href='/bat2'><button class='b'>Bật 2</button></a><td>"
  "    <td><a href='/tat2'><button class='t'>Tắt 2</button></a><td>"
  "    <tr>"
  "    <tr>"
  "    <td><a href='/bat3'><button class='b'>Bật 3</button></a><td>"
  "    <td><a href='/tat3'><button class='t'>Tắt 3</button></a><td>"
  "    <tr>"
  "    <tr>"
  "    <td><a href='/bat4'><button class='b'>Bật 4</button></a><td>"
  "    <td><a href='/tat4'><button class='t'>Tắt 4</button></a><td>"
  "      <tr>"
  "  </table>"
  + strGetValue() +
  "</div>"
  "</body>"
  "</html>"
};

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP8266 NodeMCU Weather Report</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="&#176;C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}



void TrangChu()
{
  float Temperature = dht.readTemperature(); // Gets the values of the temperature
  float Humidity = dht.readHumidity(); // Gets the values of the humidity 
  //server.send(200, "text/html", webPage);
  server.send(200, "text/html", SendHTML(Temperature,Humidity));
}
void setup()
{
  //Serial.begin(9600);
  Serial.begin(115200);
pinMode(DHTPin, INPUT);
dht.begin();
  while (WiFi.softAP("ESP8266 WiFI .4.1", "12345678") == false) 
   {
     Serial.print(".");
     delay(300);
   }
  IPAddress myIP = WiFi.softAPIP();
  server.on("/", TrangChu);
  server.begin();
}
void loop()
{
  delay(500);
  server.handleClient();
}
