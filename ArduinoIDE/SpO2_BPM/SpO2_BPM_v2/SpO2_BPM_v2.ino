#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     2000

PulseOximeter pox;

int HeartRate;
int SpO2;
uint32_t tsLastReport = 0;
void onBeatDetected()
{
  Serial.println("Beat!");
}
void setup()
{
  Serial.begin(115200);
  Serial.print("Initializing...");
  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("MAX30100 was not found. Please check the wiring/power.");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}
void loop()
{
  // Make sure to call update as fast as possible
  pox.update();
  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    HeartRate = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    Serial.print("Heart rate:");
    Serial.print(HeartRate);
    Serial.print("bpm / SpO2:");
    Serial.print(SpO2);
    Serial.println("%");
    tsLastReport = millis();
  }
}


// #include <Wire.h>
// #include "MAX30100_PulseOximeter.h"

// #define REPORTING_PERIOD_MS     1000

// // Create a PulseOximeter object
// PulseOximeter pox;

// // Time at which the last beat occurred
// uint32_t tsLastReport = 0;

// // Callback routine is executed when a pulse is detected
// void onBeatDetected() 
// {
//   Serial.println("Beat!");
// }

// void setup() 
// {
//   Serial.begin(115200);
//   Serial.print("Initializing pulse oximeter..");

//   // Initialize sensor
  
//   if (!pox.begin()) 
//   {
//     Serial.println("FAILED");
//     for(;;);
//   } 
//   else 
//   {
//     Serial.println("SUCCESS");
//   } 
//   pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
//   // Register a callback routine
//   pox.setOnBeatDetectedCallback(onBeatDetected);
// }

// void loop() {
//   // Read from the sensor
//   pox.update();

//   // Grab the updated heart rate and SpO2 levels
//   if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
//   {
//     Serial.print("Heart rate:");
//     Serial.print(pox.getHeartRate());
//     Serial.print("bpm / SpO2:");
//     Serial.print(pox.getSpO2());
//     Serial.println("%");

//     tsLastReport = millis();
//   }
// }


// #include <Wire.h>
// #include "MAX30100_PulseOximeter.h"

// #define REPORTING_PERIOD_MS     2000

// // Create a PulseOximeter object
// PulseOximeter pox;

// // Time at which the last beat occurred
// uint32_t tsLastReport = 0;

// // Callback routine is executed when a pulse is detected
// void onBeatDetected() 
// {
//     Serial.println("Beat!");
// }

// void setup() 
// {
//   Serial.begin(115200);
//   Serial.print("Initializing pulse oximeter..");

//   // Initialize sensor
//   if (!pox.begin()) 
//   {
//       Serial.println("FAILED");
//       for(;;);
//   } else {
//       Serial.println("SUCCESS");
//   } 
//   pox.setIRLedCurrent(MAX30100_LED_CURR_43_6MA);
//   // Register a callback routine
//   pox.setOnBeatDetectedCallback(onBeatDetected);
// }

// void loop() {
//     // Read from the sensor
//     pox.update();

//     // Grab the updated heart rate and SpO2 levels
//     if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
//     {
//         Serial.print("Heart rate:");
//         Serial.print(pox.getHeartRate());
//         Serial.print("bpm / SpO2:");
//         Serial.print(pox.getSpO2());
//         Serial.println("%");

//         tsLastReport = millis();
//     }
// }


// #include <Wire.h>
// #include "MAX30100_PulseOximeter.h"

// #define REPORTING_PERIOD_MS     1000

// // Create a PulseOximeter object
// PulseOximeter pox;

// // Time at which the last beat occurred
// uint32_t tsLastReport = 0;

// // Callback routine is executed when a pulse is detected
// void onBeatDetected() {
//     Serial.println("Beat!");
// }

// void setup() {
//     Serial.begin(115200);

//     Serial.print("Initializing pulse oximeter..");

//     // Initialize sensor
//     if (!pox.begin()) {
//         Serial.println("FAILED");
//         for(;;);
//     } else {
//         Serial.println("SUCCESS");
//     } 
//   pox.setIRLedCurrent(MAX30100_LED_CURR_46_8MA);

//     // Register a callback routine
//     pox.setOnBeatDetectedCallback(onBeatDetected);
// }

// void loop() {
//     // Read from the sensor
//     pox.update();

//     // Grab the updated heart rate and SpO2 levels
//     if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
//         Serial.print("Heart rate:");
//         Serial.print(pox.getHeartRate());
//         Serial.print("bpm / SpO2:");
//         Serial.print(pox.getSpO2());
//         Serial.println("%");

//         tsLastReport = millis();
//     }
// }
