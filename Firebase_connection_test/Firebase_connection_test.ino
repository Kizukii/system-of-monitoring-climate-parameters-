#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#define WIFI_SSID "netis_5CC8BE"
#define WIFI_PASSWORD "Zz123654"
#define FIREBASE_HOST "https://meteotest-d2bc4-default-rtdb.firebaseio.com/" 
#define FIREBASE_AUTH "bsGlXb00pwt7OzpVECCLpf2nKM0XRXM7oeTLBzHs"

/*
 1) Подключаемся
 2) Передаем тестовую строку test/test
 3) Считываем тестовую строку
 4) Если не удалось считать: реконнект к бд, второй заход
 5) Если удалось: вход в void loop с удалением тестовой строки
 */
FirebaseData firebaseData;

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
  
  String test_in = "test";
  String test_out = "";
  // 4)
  while(test_in.equals(test_out))
  {
    // 1)
    Serial.println("Testing connection with Firebase...");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    // 2)
    Firebase.setString(firebaseData, "test", "test");
    delay(1000);
    // 3)
    test_out = Firebase.getString(firebaseData, "test");
  }
  // 5)
  Serial.println("Connection successful!");
  Firebase.deleteNode(firebaseData, test_out);
}

void loop() {
  
}
