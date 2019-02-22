#include <LiquidCrystal.h>
#include "ESP8266.h"

#define SSID "Utkarsh's iPhone"     // "SSID-WiFiname" 
#define PASS "12345678"       // "password"

#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=Z4W7Y091E1GQSNWG"; //change it with your api key like "GET /update?key=Your Api Key"

#define QUALITY_PIN A0 //AIR QUALITY
#define METHANE_PIN A1 //MQ2 Methane
#define CO_PIN A2 //MQ9 Carbon Monoxide

ESP8266 ESP;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int qualityLevel;
int coLevel;
int methaneLevel;

void updateLevel(){
  String cmd = msg ;
  cmd += "&field1=";
  cmd += qualityLevel;
  cmd += "&field2=";
  cmd += coLevel;
  cmd += "&field3=";
  cmd += methaneLevel;
  
  cmd += "\r\n";
  if(ESP.get(IP, 80, cmd)){
//    ESP.serialDebug();
  }
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("Initializing!");

  ESP.preInit();
  while(ESP.connectWiFi(SSID, PASS)){
    Serial.print("Trying..");
    delay(500);
  }
  lcd.setCursor(0, 1);
  lcd.print("** ALL OK **");
  delay(2000);
}

void loop() {
  qualityLevel = analogRead(QUALITY_PIN);
  qualityLevel = map(qualityLevel, 0, 1023, 0, 100);
  qualityLevel=100-qualityLevel;


  coLevel = analogRead(CO_PIN);
  coLevel = (100-map(coLevel, 0, 1023, 0, 100));
  
//
//  float sensor_volt = coLevel / 1024 * 5.0;
//  sensor_volt = (5.0-sensor_volt)/sensor_volt; // omit * RL
//  coLevel = sensor_volt / 9.8; // The ratio of RS/R0 is 9.8 in a clear air from Graph (Found using WebPlotDigitizer)

  methaneLevel = analogRead(METHANE_PIN);
  methaneLevel = map(methaneLevel, 0, 1023, 0, 100)*10;

  Serial.println("Read sensor: ");
  Serial.print("Quality : ");
  Serial.print(qualityLevel);
  Serial.print("% CO : ");
  Serial.print(coLevel);
  Serial.print("% CH4 : ");
  Serial.print(methaneLevel);
  Serial.println("%");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Qty:");
  lcd.print(qualityLevel);
  lcd.print("%|CO:");
  lcd.print(coLevel);
  lcd.print("ppm");
  lcd.setCursor(0, 1);
  lcd.print("CH4:");
  lcd.print(methaneLevel);
  lcd.print("ppm");
  if(coLevel>150 || methaneLevel>1000){
      lcd.print("*Danger*");
  }
  updateLevel();
  
  delay(2000);
}
