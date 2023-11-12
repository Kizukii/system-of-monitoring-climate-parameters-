#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#include "DHT.h"
#define DHT11PIN 0 //пин данных с дачика
DHT dht11(DHT11PIN, DHT11);

#define WIFI_SSID "netis_5CC8BE"
#define WIFI_PASSWORD "Zz123654"
#define FIREBASE_HOST "meteotest-d2bc4-default-rtdb.firebaseio.com/" 
#define FIREBASE_AUTH "bsGlXb00pwt7OzpVECCLpf2nKM0XRXM7oeTLBzHs"

FirebaseData firebaseData;

String container = "/Data/";

int test = 0;
int tact_int = 0;
String tact = "";

void setup() {
  Serial.begin(115200);
  dht11.begin();

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {  

//  if(test == 0) {
//    String test_in = "test";
//    String test_out = "";
//
//    while(test_in != test_out)
//    {
//      Serial.print("Testing connection with Firebase.");
//      Serial.print(".");
//      Firebase.setString(firebaseData, "test", "test");
//      delay(2000);
//      test_out = Firebase.getString(firebaseData, "test");
//      delay(2000);
//    }
//    Serial.println("Connection successful!");
//    Firebase.deleteNode(firebaseData, "test");
//    test=test+1;
//  }
  float h = dht11.readHumidity(); //Измеряем влажность
  float t = dht11.readTemperature(); //Измеряем температуру
  if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.println("Ошибка считывания");
    return;
  }

  String analogDHT_h = String(h);
  String analogDHT_t = String(t);

  tact = String(tact_int);
  
  Firebase.setString(firebaseData, container+"Hum"+tact, analogDHT_h); 
  Firebase.setString(firebaseData, container+"Temp"+tact, analogDHT_t); 

  tact_int = tact_int+1;

  delay(5000); // 5 секунд задержки
}
