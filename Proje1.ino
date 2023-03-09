#include <DHT.h>  
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial

/*File – Preferences Additional Boards Manager URLs: http://arduino.esp8266.com/stable/package_esp8266com_index.json
Tools – Board Manager ESP8266 by ESP8266 Community
 */
 
String apiKey = "";  // thingspeak apikey   
char auth[] = ""; //blink auth
const char *ssid =  ""; //wifi name     
const char *pass =  ""; // wifi password
const char* server = "api.thingspeak.com";
 
#define DHTPIN 0          //dht11 pin baglanti noktasi D3
 
DHT dht(DHTPIN, DHT11);

WiFiClient client;

float h = 0;
float t = 0;
 
void setup() 
{
       Serial.begin(9600);
       delay(10);
       Blynk.begin(auth, ssid, pass);
       dht.begin();
 
       Serial.println("Baglaniyor ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi baglandi");
 
}
 
void loop() 
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("DHT Sensor hatasi!");
                      return;
                 }

                 Blynk.virtualWrite(V0, t);  //V0 Sicaklik
                 Blynk.virtualWrite(V1, h);  //V1 Nem
                 if ( (t > 31 ) )
                 {
                  Blynk.notify(String("Uyari ")+t + String("C"));
                 }
                 
 
                         if (client.connect(server,80))   //   
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Sicaklik: ");
                             Serial.print(t);
                             Serial.print(" Celcius, Nem: ");
                             Serial.print(h);
                             Serial.println("%. Thingspeak'e gonderiliyor.");
                        }
          client.stop();
 
          Serial.println("Bekleniyor...");
  
  // thingspeak 15 sn araliklarla guncelleme yapabilir
  delay(1000);
}
