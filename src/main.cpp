#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "dht11esp2866-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "BaG9izNyVd4yIrPEw7aXvFOAjJS4BDZqxz91SNUu"
#define WIFI_SSID "TP-LINK_C860A0"
#define WIFI_PASSWORD "MANCHAS123"

#define DHTPIN D2     // Se declara el pin de datos del sensor de temperatura
#define DHTTYPE DHT11 // Se declara que tipo de sensor dht es
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin(); // de inicializa el sensor

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());               // imprime la ip que fue orotgada al esp8266
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // conexion con firebase
  
}

void loop()
{
  float h = dht.readHumidity();    // Variable para leer la humedad
  float t = dht.readTemperature(); // Variable para leer la temperatura

  if (isnan(h) || isnan(t)) // verificar si esta funcionando el sensor
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  String fireHumid = String(h) + String("%"); // se pasa la medida de la humedad a string

  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C ");
  String fireTemp = String(t) + String("°C"); // se para la temperatura a string
  delay(5000);

  Firebase.pushString("/DHT11/Humidity", fireHumid);   // ruta a la que se envia la humedad
  Firebase.pushString("/DHT11/Temperature", fireTemp); // ruta a la que se envia la temperatura
  if (Firebase.failed())
  {
    Serial.print("pushing /logs failed:");
    Serial.println(Firebase.error());
    return;
  }
}
