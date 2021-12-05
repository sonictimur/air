/**
 * Arduino Air Analytics
 * By 'sonictimur'
 */

/** Из-за нехватки бюджетных средств и временеи, программа будет посылать рандомные данные
  * Ядро прототипа готова, осталось прикрепить соответствующие сенсоры */

 #include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "TIMUR"
#define WIFI_PASSWORD "Fucksonichen5"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAKtTY7Hd9ZOUWCxIx8F0lxpZr7_ceOORE"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://airduino-analytics-default-rtdb.asia-southeast1.firebasedatabase.app" 


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

String hardware_name = "Arduino_Sensor_A1";

void setup()
{
  // We'll set up the LED pin to be an output.
  pinMode(A0, OUTPUT); // Этот код потом мотифицируем под настоящие
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print("Connected!");
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  digitalWrite(LED_BUILTIN, HIGH);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setString(&fbdo, hardware_name + "/Hardware_Status", "Online")){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    digitalWrite(LED_BUILTIN, HIGH);
    Firebase.RTDB.setFloat(&fbdo, hardware_name + "/Humidity", 0.01 + random(0,80));
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);    
    Firebase.RTDB.setFloat(&fbdo, hardware_name + "/Light_Level", 0.01 + random(0,80));
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);     
    Firebase.RTDB.setFloat(&fbdo, hardware_name + "/Temperature", 0.01 + random(0,80));
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);     
    Firebase.RTDB.setFloat(&fbdo, hardware_name + "/UV_Radiation", 0.01 + random(0,80));
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);     

    count++;
  }
  }
