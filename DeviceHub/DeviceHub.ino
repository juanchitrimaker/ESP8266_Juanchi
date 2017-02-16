#include <ESP8266WiFi.h>
#include "floatToString.h"

//Lib LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <LCD.h>


#define THIG_NAME "potenciometro"  // Put here your thing name
#define WIFISSID "Wideo"
#define PASSWORD "churnparamkt<5%"
#define DATO1 "Temp"
#define LATITUDE2 "-59.433917"
#define LATITUDE3 "-58.436917"
#define LATITUDE4 "-58.439917"

#define LONGITUDE2 "-40.596217"
#define LONGITUDE3 "-34.599217"
#define LONGITUDE4 "-34.600217"
int variable=0,positivo=0,decimal=0;
float tension = 0,coma=0,total=0;
String val;

String pubString, pubString2;
String longitude = "44.436069";
String latitude = "26.133531";
const char* host = "api.devicehub.net";//api.devicehub.net


int i=0;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);

void POST (void);

void setup(){
    
    WiFi.begin(WIFISSID, PASSWORD);
    
    pinMode(5,OUTPUT);
    digitalWrite(5,LOW);
    Serial.begin(9600);
    delay(10);
    
    
     
    //Set LCD
    Wire.begin(2,0);
    lcd.begin(16,2);
    lcd.setBacklight(LCD_BACKLIGHT);
    lcd.home();
    lcd.print("IoT Example");
    delay(3000);

    

      //Put INFO Serial
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

      //Put INFO LCD
    lcd.setCursor(0, 0);
    lcd.print("WIFI Connected"); 
    lcd.setCursor(0, 1);      
    lcd.print("IP:"); // Start Print Test to Line 2
    lcd.print(WiFi.localIP());
    
    digitalWrite(5,HIGH);
    delay(5000);
    digitalWrite(5,LOW);
}

void loop(){

    
    variable = analogRead(A0);
    total = map(variable, 0, 1023, 0, 100);  // convertir a porcentaje

    val = String(total);
    //sprintf(val, "%f", total);
    //Put INFO LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Valor Pote"); 
    lcd.setCursor(0, 1);      
    lcd.print(val);

    //mando datos a DeviceHub
    POST();

    switch(i)
    {case 5:  longitude = String(LONGITUDE2);
              latitude = String(LATITUDE2);
              break;
     case 10: longitude = String(LONGITUDE3);
              latitude = String(LATITUDE3);
              break;    
     case 15: longitude = String(LONGITUDE4);
              latitude = String(LATITUDE4);
              i = 0;
              break;
     default: break;
     }
    i++;
    
}


void POST(){
   

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  Serial.print("Requesting POST: ");
  // Send request to the server:

  pubString2 = "{\"value\": " + String(total) + "}";
  String pubStringLength2 = String(pubString2.length(), DEC);
  
  client.println("POST /v2/project/12282/device/6e53622e-1704-4f50-bd54-6fcc9ba40d81/sensor/Pote_Sensor/data HTTP/1.1");
  client.println("Host: api.devicehub.net");
  client.print("X-ApiKey: 3d184fa2-01a1-417b-a9d7-d1585e186f06\r\n"); // Get this from DeviceHub.net
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(pubStringLength2);
  client.println();
  client.print(pubString2);
  client.println();
  delay(500); // Can be changed

  client.stop();

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  pubString = "{\"value\": {\"type\": \"Feature\", \"properties\": {}, \"geometry\": { \"type\": \"Point\", \"coordinates\": [" + latitude + ", " + longitude + "] } } }";
  String pubStringLength = String(pubString.length(), DEC);
  
  client.println("POST /v2/project/12282/device/6e53622e-1704-4f50-bd54-6fcc9ba40d81/sensor/Ubicacion2/data HTTP/1.1");
  client.println("Host: api.devicehub.net");
  client.print("X-ApiKey: 3d184fa2-01a1-417b-a9d7-d1585e186f06\r\n"); // Get this from DeviceHub.net
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(pubStringLength);
  client.println();
  client.print(pubString);
  client.println();
  delay(500);
  client.stop();


  // Read all the lines of the reply from server and print them to Serial
 /* while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
*/}
