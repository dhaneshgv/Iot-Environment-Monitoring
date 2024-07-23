#include <DHT11.h>
#include <WiFi.h>
#include "ThingSpeak.h"
#include <Wire.h>
#include <BH1750.h>

const char* ssid = "KCIRI";   // your network SSID (name) 
const char* password = "welcome@KCT"; 

WiFiClient  client;
BH1750 lightMeter;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "XTID08JXAFVYNICX";
// Create an instance of the DHT11 class and set the digital I/O pin.
DHT11 dht11(4);

float x;
float y;
float voltage_min = 0.0;     // Minimum voltage (measured when intensity is at its minimum)
float voltage_max = 3.3;     // Maximum voltage (measured when intensity is at its maximum)
float intensity_min = 0.0;   // Minimum intensity value (corresponding to voltage_min)
float intensity_max = 100.0; 
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }// Maximum intensity value (corresponding to voltage_max)

void setup() {
  // Initialize serial communication at 115200 baud.

    Serial.begin(115200);  //Initialize serial
    
    WiFi.mode(WIFI_STA);   
    ThingSpeak.begin(client);
    Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);

    lightMeter.begin();

    Serial.println(F("BH1750 Test begin"));
}

void loop() 
 
{

    
        // Connect or reconnect to WiFi
        if(WiFi.status() != WL_CONNECTED){
          Serial.print("Attempting to connect");
          while(WiFi.status() != WL_CONNECTED){
            WiFi.begin(ssid, password); 
            delay(5000);     
          } 
          Serial.println("\nConnected.");
    }

    float a = analogRead(34);
    float Xaxis = map(a,1,2279,0,180);
    delay(500);

    float y = analogRead(35);  
    float Yaxis = map(y,1,2279,0,180);


// Read the humidity from the sensor.
    float humidity = dht11.readHumidity();
// Read the temperature from the sensor.
    float temperature = dht11.readTemperature();

  
    float intensity_mes = analogRead(32);
    float voltage = (intensity_mes / 4095.0) * 3.3;
    float intensity = map(voltage, 0, 3.3, 0, 100);

  // Custom mapping function to map float values
  



    float lux = lightMeter.readLightLevel();
   


    ThingSpeak.setField(2, humidity);
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(3, Xaxis);
    ThingSpeak.setField(4, Yaxis);
    ThingSpeak.setField(6, intensity);
    ThingSpeak.setField(7, lux);
    Serial.print("humidity=");
    Serial.println(humidity);
    Serial.print("temperature=");
    Serial.println(temperature);
    Serial.print("Xaxis=");
    Serial.println(Xaxis);
    Serial.print("Yaxis=");
    Serial.println(Yaxis);
    Serial.print("intensity_mes=");
    Serial.println(intensity);
    Serial.print("lux=");
    Serial.println(lux);
    
   

    int x = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("1 Problem updating channel. HTTP error code " + String(x));
    }

}
