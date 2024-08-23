#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <credentials.h>
#include <gpio.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
bool lectura = false;
bool lecturaPrev = false;
void WiFiConfiguration()
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi: ");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void FirebaseConfiguration()
{
  /* Assign the api key (required) */
  config.api_key = FIREBASE_AUTH;

  /* Assign the RTDB URL (required) */
  config.database_url = FIREBASE_HOST;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Firebase is Ready");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setup()
{
  Serial.begin(115200);
  WiFiConfiguration();
  FirebaseConfiguration();
  PinsConfiguration();
}

void loop()
{
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    for (int i = 0; i < 7; i++)
    {
      String path = "/test/days/" + String(days[i]);
      if (Firebase.RTDB.getInt(&fbdo, path.c_str(), &dayStates[i]))
      {
        digitalWrite(dayPins[i], dayStates[i]);
        if (dayStates[i] != prevDayStates[i])
        {
          Serial.print("Estado ");
          Serial.print(days[i]);
          Serial.print(": ");
          Serial.println(dayStates[i]);
          prevDayStates[i] = dayStates[i];
        }
      }
      else
      {
        Serial.println(fbdo.errorReason().c_str());
      }
    }
  }
}




// // Write an Float number on the database path test/float
// if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0, 100)))
// {
//   Serial.println("PASSED");
//   Serial.println("PATH: " + fbdo.dataPath());
//   Serial.println("TYPE: " + fbdo.dataType());
// }
// else
// {
//   Serial.println("FAILED");
//   Serial.println("REASON: " + fbdo.errorReason());
// }
//}

// if (Firebase.RTDB.getInt(&fbdo, "/test/days/monday", &ledState))
// {
//   // digitalWrite(LEDBUILDIN, ledState);
//   Serial.print("Estado LED: ");
//   Serial.println(ledState);
//   digitalWrite(ledpin, ledState);
// }
// else
// {
//   Serial.println(fbdo.errorReason().c_str());
// }