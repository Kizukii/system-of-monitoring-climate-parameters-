#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#define WIFI_SSID "netis_5CC8BE"
#define WIFI_PASSWORD "Zz123654"
#define FIREBASE_HOST "meteotest-d2bc4-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "bsGlXb00pwt7OzpVECCLpf2nKM0XRXM7oeTLBzHs"

String ProjectBucket="/Data/";
String current_state = "0";

FirebaseData firebaseData;

String a_on = "1";

void setup() {
  Serial.begin(115200);

  pinMode(0, OUTPUT);

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
  Firebase.get(firebaseData, ProjectBucket+"AAAAA"); //считывание команды
  String out_state = firebaseData.stringData();
  //Serial.println(out_state);
    if(!out_state.equals(current_state))//получена новая команда 
    {
      Serial.println ("OUT_CMD: " + out_state);
      current_state = out_state;
      if(out_state.equals(a_on)) //подтверждение действия
      {
        //digitalWrite(0,HIGH);
        Firebase.setString(firebaseData, ProjectBucket+"RABOTAEET", "KEK"); 
      }
      else
      {
       //digitalWrite(0,LOW);
       Firebase.setString(firebaseData, ProjectBucket+"AAAAA", "ABPOPA"); 
      }
    
  
  
  delay(1000);// не обязательно
  }
}
