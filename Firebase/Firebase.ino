// Подключение миллиона библиотек для датчиков и esp
#include <ESP8266WiFi.h> // Библиотека для ESP
#include "FirebaseESP8266.h" // Библиотека для работы с БД
#include <math.h> // Библиотека мат. расчетов
#include <SFE_BMP180.h> // Библиотека для BMP180
#include <Wire.h> // Библиотека для I2C шины
#include <DHT.h> // Библиотека для DHT
// Библиотеки для NTP сервера
#include <NTPClient.h>
#include <WiFiUdp.h>

// Инициализация и подключение датчиков
//#define DHTPIN 3 // Подключение DHT22 к пину 5 GPIO16
#define DHTPIN1 13 // Подключение DHT11 к пину 3 GPIO0
SFE_BMP180 pressure; // Переменная для датчика BMP180

#define ALTITUDE 151.0 // Высота над уровнем моря в метрах 

//DHT dht(DHTPIN, DHT22); //Инициация датчиков DHT
DHT dht1(DHTPIN1, DHT11);

const long utcOffsetInSeconds = 10800;
WiFiUDP ntpUDP; // Инициализация NTP сервера
NTPClient timeClient(ntpUDP);

// Переменные для хранения даты/времени
String formattedDate;
String dayStamp;
String timeStamp;

// Объявление глобальных переменных
float h1; // Переменная хранения влажности для DHT11
float t1; // Переменная хранения температуры для DHT11
//float h; // Переменная хранения влажности для DHT22
//float t; // Переменная хранения температуры для DHT22
char status; // Переменная отслеживания состояния измерения 1 - измерение проведено, 0 - измерение не проведено
double T,P; // Переменные температуры и давления для BMP180

#define WIFI_SSID "1500" // Имя wi-fi сети
#define WIFI_PASSWORD "disbalans" // Пароль от wi-fi
#define FIREBASE_HOST "meteotest-d2bc4-default-rtdb.firebaseio.com"  // Имя хоста БД
#define FIREBASE_AUTH "bsGlXb00pwt7OzpVECCLpf2nKM0XRXM7oeTLBzHs" // Секретный ключ БД

FirebaseData firebaseData; // Объявление класса БД

int tact_int = 0;
String tact = "";

void setup() {
    // Подключение и проверка датчиков
    Serial.begin(115200);
    //dht.begin();
    dht1.begin();
  
    timeClient.begin(); // Подключение NTP сервера
    timeClient.setTimeOffset(10800); // Задаем часовой пояс

    if (pressure.begin()) { 
        // Проверка на подключение BMP180
        Serial.print("BMP180 ok");
    } else {
        Serial.print("BMP180 fail");
        delay(2000);
        while(1); // Бесконечный цикл. Если есть сообщение об ошибке, нужно проверить подключение BMP180 и перезапустить плату (выключить и включить питание)
    }

    // Подключние к wi-fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
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
    // Получение даты/времени
    while(!timeClient.update()) {
        timeClient.forceUpdate();
    }
    formattedDate = timeClient.getFormattedTime();
    Serial.println(formattedDate);

    // Extract date
    //int splitT = formattedDate.indexOf("T");
//    dayStamp = formattedDate.substring(0, splitT);
//    Serial.print("DATE: ");
//    Serial.println(dayStamp);
//    // Extract time
    //timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
//    Serial.print("HOUR: ");
//    Serial.println(timeStamp);

    // Читаем данные с датчиков и записываем в строки
    String analogValue0;
    String analogValue1;
    h1 = dht1.readHumidity(); //Измеряем влажность
    t1 = dht1.readTemperature(); //Измеряем температуру
    if (isnan(h1) || isnan(t1)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
        Serial.println("Ошибка считывания");
        return;
    }

    status = pressure.startTemperature(); // Старт измерения температуры
    delay(status);
    status = pressure.getTemperature(T);    
    analogValue0 = String(T);
    // Определяем атм. давление:
    // Параметр указывает разрешение, от 0 до 3 (чем больше разрешение, тем выше точность, тем дольше ждать). 
    status = pressure.startPressure(1);
    delay(status);
    status = pressure.getPressure(P,T);            
    analogValue1 = String(P*0.750064);
                   
//    h = dht.readHumidity(); //Измеряем влажность
//    t = dht.readTemperature(); //Измеряем температуру
//    if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
//        Serial.println("Ошибка считывания DHT22");
//        return;
//    } 
    // Запись данных в строки
    String analogValue2 = String(t1);
    String analogValue3 = String(h1);
//    String analogValue4 = String(t);
//    String analogValue5 = String(h); 

    // Формирование данных на отправку
    String data = analogValue2+"q"+analogValue3+"q"+analogValue0+"q"+analogValue1+"q"+formattedDate; 

    
  
    Firebase.setString(firebaseData, " Node "+formattedDate, data); 

    

    delay(10000); // 10 секунд задержки
}
