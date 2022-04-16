
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int led = 12; // Connected to D7 pin of NodeMCU
int flame_sensor = 13; //Connected to D6 pin of NodeMCU
int buzzer = 15; //Connected to D8 pin of NodeMCU
int relay = 14; //Connected to D5 pin of NodeMCU
int smoke_sensor = A0; //Connected to A0 pin of NodeMCU
int temp_smoke_sensor;
int flame_sensor_read;


char auth[] = "ksU2hlHwYSRB3Ak48dF-gsvA9JHZIsNg"; // Enter Auth Token

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "kanha"; //Enter the wifi name
char pass[] = "kanha2108";// Enter the wifi password
BlynkTimer timer;
#define ONE_WIRE_BUS 5 // DS18B20 on arduino pin2 corresponds to D1 on physical board " but D4 pin on the nodemcu Module"
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float temp;
float Fahrenheit=0;

void setup()
{
   Serial.begin(9600);
     DS18B20.begin();
  timer.setInterval(1000L, getSendData);
  lcd.begin();
   lcd.backlight();
   //ldc.clear();
    lcd.print(" IoT Fire Alarm");
    delay(3000);
  Blynk.begin(auth, ssid, pass);
  Wire.begin(2,0);

  lcd.print(" IoT Fire Alarm");
  pinMode(led, OUTPUT);
  pinMode(flame_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(smoke_sensor, INPUT);
  
  digitalWrite(led, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(relay, HIGH);
  // Debug console
 
}

void loop() 
{
  lcd.begin();
   lcd.backlight();
   //ldc.clear();
  flame_sensor_read = digitalRead(flame_sensor); //reading the sensor data on A0 pin
  Blynk.virtualWrite(V0, flame_sensor_read); //sending data to Blynk
  //Serial.print("Flame Status:");
  lcd.setCursor(0, 0);
  lcd.print("Flame Status: ");
  lcd.print(flame_sensor_read);
 // Serial.println(flame_sensor_read);

  int led_status = digitalRead(led);
  if (led_status == HIGH)
  {
   Blynk.virtualWrite(V1, 255);
  }
  else
  {
   Blynk.virtualWrite(V1, 0);
  }
  
  temp_smoke_sensor = analogRead(smoke_sensor);
  Blynk.virtualWrite(V2, temp_smoke_sensor);
 Serial.print("Current Gas Level:");
 Serial.println(temp_smoke_sensor);
   lcd.setCursor(0, 1);
 lcd.print("Gas Level: ");
 lcd.print(temp_smoke_sensor);
 
  if (temp_smoke_sensor > 215)
  {
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(relay, HIGH);
    Blynk.notify("Alert Fire Detected");  
     lcd.setCursor(0, 0); 
    lcd.print(" Fire Detected "); 
    
    }
  else
    {
     digitalWrite(led, LOW);
     digitalWrite(buzzer, LOW);
     digitalWrite(relay, HIGH);
    }

if (flame_sensor_read == 0)
    {
    Blynk.virtualWrite(V1, 255);
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(relay, LOW);
    Blynk.notify("Alert Fire Detected"); 
    lcd.setCursor(0, 1); 
    lcd.print(" Fire Detected ");  
     
    }
  else
      {
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
      digitalWrite(relay, HIGH);
      }
      //delay(500);
     
      timer.run(); 
      Blynk.run();

}

void getSendData()
{
  DS18B20.requestTemperatures(); 
  temp = DS18B20.getTempCByIndex(0); // Celcius
   Fahrenheit = DS18B20.toFahrenheit(temp); // Fahrenheit
  Serial.println(temp);
  Serial.println(Fahrenheit);
  Blynk.virtualWrite(V3, temp); //virtual pin V3
  Blynk.virtualWrite(V4, Fahrenheit); //virtual pin V4
   if (temp >= 40)
    {
    
    Blynk.notify("Alert High Temperature"); 
     digitalWrite(buzzer, HIGH);
    
}

else{
   digitalWrite(buzzer, LOW);
  }
}
