#include <Wire.h>
#include <WiFi.h>
#include "MAX30100_PulseOximeter.h"
#include <UrlEncode.h>
#include <HTTPClient.h>

#define REPORTING_PERIOD_MS     1000
#define HEART_RATE_ARRAY_SIZE   10

// DEFINIÇÕES WIFI
const char* ssid = "C-137";
const char* password = "18e457b313";

//DEFINIÇÕES P/CALLME BOT
String phoneNumber = "+351963151007";
String apiKey = "7129350";

// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

// Array to store the heart rate values of the last HEART_RATE_ARRAY_SIZE seconds
int heartRateArray[HEART_RATE_ARRAY_SIZE];
int zeroBpmCount;

// Callback routine is executed when a pulse is detected
void onBeatDetected() 
{
    Serial.println("YOU ALIVE!");
}

void setup() 
{
    Serial.begin(115200);
    Serial.print("A inicializar MAX30100..");
    
    // CONECTAR WIFI
    WiFi.begin(ssid, password);
    Serial.println("A conectar via WiFi");
    while(WiFi.status() != WL_CONNECTED) 
    {
      delay(250);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Conectado com Sucesso!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    
    // TENTATIVA DE INICIAR
    if (!pox.begin()) 
    {
        Serial.println("FAILED");
        for(;;);
    } 
    else 
    {
        Serial.println("Ligado com sucesso");
        delay(2000);
    }

    // Configure sensor to use 7.6mA for LED drive
    pox.setIRLedCurrent(MAX30100_LED_CURR_46_8MA);
    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);

    // Initialize the heart rate array with default values
    for (int i = 0; i < HEART_RATE_ARRAY_SIZE; i++) 
    {
        heartRateArray[i] = -1;
    }
}

// CÓDIGO EDITADO PARA USO DO CALLMEBOT.
// CÓDIGO ORIGINAL DISPONIVEL EM CALLMEBOT.COM
void sendMessage(String message)
{
  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Mensagem enviada com sucesso");
  }
  else{
    Serial.println("Erro ao enviar mensagem");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void loop() 
{
    // Update the sensor values
    pox.update();

    // Check if enough time has elapsed since the last report
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        // Read the heart rate value from the sensor
        int heartRate = pox.getHeartRate();

        // Store the new heart rate value at the end of the array
        if (heartRate > 0) {
            for (int i = 0; i < HEART_RATE_ARRAY_SIZE - 1; i++) 
            {
                heartRateArray[i] = heartRateArray[i + 1];
            }
            heartRateArray[HEART_RATE_ARRAY_SIZE - 1] = heartRate;
        }

        // Calculate the average heart rate over the last HEART_RATE_ARRAY_SIZE seconds
        int sum = 0;
        int count = 0;
        for (int i = 0; i < HEART_RATE_ARRAY_SIZE; i++) {
            if (heartRateArray[i] != -1) {
                sum += heartRateArray[i];
                count++;
            }
        }
        float averageHeartRate = (count > 0) ? (float)sum / count : 0;

        // Print the heart rate and SpO2 values to the serial monitor along with the unit of measurement
        Serial.print("Heart rate:");
        Serial.print(heartRate);
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.print("% / Average Heart Rate:");
        Serial.print(averageHeartRate);
        Serial.println(" bpm");

        // Reset the timer if bpm is not 0
        if (heartRate > 0) 
        {
          zeroBpmCount = 0;
        } 
        else 
        {
            // Increment the timer if bpm is 0
          zeroBpmCount++;
        }
            // Call the function if 10 seconds have elapsed with 0 bpm readings
            if (zeroBpmCount >= 7) 
            {
                sendMessage("Você Faleceu.. Uma tristeza = (");
                zeroBpmCount = 0;
            }
        // Update the last report time
        tsLastReport = millis();
    }
}
