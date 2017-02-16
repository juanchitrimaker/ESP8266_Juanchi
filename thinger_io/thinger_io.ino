#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>
#include <LiquidCrystal_I2C.h>

#define USERNAME "juanchitrimaker"
#define DEVICE_ID "ESP8266_POTE"
#define DEVICE_CREDENTIAL "tTmaE7H$hTBr"
#define LATITUDE "-58.430916"
#define LONGITUDE "-34.593215"

#define LATITUDE2 "58.430916"
#define LONGITUDE2 "-34.593215"

#define LATITUDE3 "-58.430916"
#define LONGITUDE3 "34.593215"



#define SSID_WiFi "Wideo"
#define SSID_PASSWORD "churnparamkt<5%"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

int variable=0,leds=0, i=0;
float total=0;
String val,latitude,longitude;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);

void Control_Leds(void);

void setup() {

  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(12,OUTPUT);
  
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(12,LOW);
  digitalWrite(14,LOW);
  latitude = LATITUDE;
  longitude = LONGITUDE;
  thing.add_wifi(SSID_WiFi, SSID_PASSWORD);

  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thing["led"] <<  digitalPin(14);
  // allow reading potenciometro
  thing["percent"] >> [](pson& out){ out = map(analogRead(A0), 0, 1023, 0, 100);};

  //GPS Location
  thing["location"] >> [](pson& out){out["lat"] = latitude; out["lon"] = longitude;};

  //Set LCD
  Wire.begin(2,0);
  lcd.begin(16,2);
  lcd.setBacklight(LCD_BACKLIGHT);
  lcd.home();
  lcd.print("IoT Example");
  delay(3000);

  //Put INFO LCD
  lcd.setCursor(0, 0);
  lcd.print("WIFI Connected"); 
  lcd.setCursor(0, 1);      
  lcd.print("IP:"); // Start Print Test to Line 2
  lcd.print(WiFi.localIP());

}

void loop() {
  thing.handle();
  
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

   delay(500);
   if(i >= 25 && i<50){
      latitude = LATITUDE2;
      longitude = LONGITUDE2;
   }
   if(i >= 50){
      latitude = LATITUDE3;
      longitude = LONGITUDE3;
   }
   Control_Leds();

  
}

void Control_Leds(){

  if(digitalRead(14) == HIGH){
    switch(i){
      case 10:  digitalWrite(4,HIGH);
                digitalWrite(5,LOW);
                digitalWrite(12,LOW);
                break;
      case 20:  digitalWrite(4,LOW);
                digitalWrite(5,HIGH);
                digitalWrite(12,LOW);
                break;
      case 30:  digitalWrite(4,LOW);
                digitalWrite(5,LOW);
                digitalWrite(12,HIGH);
                break;
      case 40:  digitalWrite(4,HIGH);
                digitalWrite(5,HIGH);
                digitalWrite(12,LOW);
                break;
      case 50:  digitalWrite(4,HIGH);
                digitalWrite(5,LOW);
                digitalWrite(12,HIGH);
                break;
      case 60:  i==0;
                break;
      default:  break;
      i++;
    }
       
  }
  if(digitalRead(14) == LOW){
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(12,LOW);   
  }

}



