#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#define WIFI_SSID "netis_5CC8BE"
#define WIFI_PASSWORD "Zz123654"
#define FIREBASE_HOST "meteotest-d2bc4-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "bsGlXb00pwt7OzpVECCLpf2nKM0XRXM7oeTLBzHs"

String ProjectBucket="/ESP/";
FirebaseData firebaseData;
String a = "";
void setup() {
  Serial.begin(115200);

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

void loop()
{
    Firebase.setString(firebaseData, ProjectBucket+"Popitka", "ne pitka");
    delay(2000); 
    Firebase.get(firebaseData, ProjectBucket+"Popitka");
    a = firebaseData.stringData();
    delay(2000);
    Firebase.deleteNode(firebaseData, ProjectBucket+"Popitka");
    delay(2000);
    Serial.println(a);
}
