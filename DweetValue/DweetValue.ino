#include "dweetESP8266.h"

//Lib LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <LCD.h>


#define THIG_NAME "potenciometro"  // Put here your thing name
#define WIFISSID "Wideo"
#define PASSWORD "churnparamkt<5%"
#define DATO1 "Temp"

dweet client;
int variable=0,positivo=0,decimal=0;
float tension = 0,coma=0,total=0;
String val;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);


void setup(){
    
    client.wifiConnection(WIFISSID, PASSWORD);
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
    /*tension = (variable*3.3)/1024;


    positivo = int(tension);
    decimal = int((positivo - tension) * 100);
    coma = (decimal/100);
    total = positivo + coma;
    */
    total = map(variable, 0, 1023, 0, 100);  // convertir a porcentaje

    val = String(total);
    //sprintf(val, "%f", total);
    //Put INFO LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Valor Pote"); 
    lcd.setCursor(0, 1);      
    lcd.print(val);

    //mando datos a dweet
    client.add(DATO1, val); // specifies the args of type "String"
    client.sendAll(THIG_NAME);
    delay(5000);
}
