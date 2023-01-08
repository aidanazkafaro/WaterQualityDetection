#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ThingsBoard.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#define TOKEN               "286N1Z"
char thingsboardServer[] =  "172.20.10.9";
#define BOTtoken "5714109260:AAEkcu0hTZGOy2nes7GosztJCqdMX6BFCnY"
#define CHAT_ID "692876373"
#define WIFI_AP "iPhone"
#define WIFI_PASSWORD "thegreatest"

WiFiClient wifiClient;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
ThingsBoard tb(wifiClient);
int status = WL_IDLE_STATUS;
bool subscribed = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  InitWiFi();

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  delay(2000);
  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
    return;
  }

  if (!tb.connected()) {
    subscribed = false;

    Serial.print("Connecting to: ");
    Serial.print(thingsboardServer);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(thingsboardServer, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
  sendData();
  tb.loop();
}

void sendData(){
  // generating data
  
  // simulation of ideal water pH level for fish breeding
  // ranging from 6.5 to 8.5
  float pH = random(5,11);
  String stringPh = (String)pH;
  // simulation of ideal water temperature
  // ranging from 23 to 27 degree celcius
  float waterTemp = random(21, 30);
  String stringWaterTemp = (String) waterTemp;
  
  // simulation of Dissolved Oxygen level 
  // ranging from 5 to 20 ppm.
  float dissolvedOxygen = random(3, 23);
  String stringOxygen = (String) dissolvedOxygen;

  // simulation of Conductivity level 
  // ranging from 500 - 800 µS/cm
  float conductivity = random(480, 820);
  String stringConductivity = (String) conductivity;

  String alertMsg = "details:\npH = " + stringPh + "\n Water Temperature = " + stringWaterTemp + "°C\n Dissolved Oxygen = " + stringOxygen + "ppm\n Conductivity = " + stringConductivity + "µS/cm";

  if (pH < 6 || pH > 8 || waterTemp < 23 || waterTemp > 27 || dissolvedOxygen < 5 || dissolvedOxygen > 20 || conductivity < 500 || conductivity > 800) {
    bot.sendMessage(CHAT_ID, "Water quality dropped!", "");
    bot.sendMessage(CHAT_ID, alertMsg, "");

  }

  Serial.println("Sending data...");
        
  tb.sendTelemetryFloat("pH", pH);
  tb.sendTelemetryFloat("waterTemp", waterTemp);  
  tb.sendTelemetryFloat("Dissolved Oxygen", dissolvedOxygen);  
  tb.sendTelemetryFloat("Conductivity", conductivity);  
  
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(2000);
      Serial.println(".");
    }
    Serial.println("Connected to AP");
  }
}
