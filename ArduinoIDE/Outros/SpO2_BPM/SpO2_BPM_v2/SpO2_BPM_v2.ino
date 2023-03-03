#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

// Callback routine is executed when a pulse is detected
void onBeatDetected() {
    Serial.println("♥ Beat!");
}

void setup() {
    Serial.begin(115200);

    Serial.print("Initializing pulse oximeter..");

    // Initialize sensor
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
    // Read from the sensor
    pox.update();

    // Grab the updated heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");

        tsLastReport = millis();
    }
}









//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include "MAX30100_PulseOximeter.h"
//
//PulseOximeter pox;
//
//void setup() {
//  Serial.begin(115200);
//  Wire.begin();
//  pox.begin();
//}
//
//void loop() {
//  pox.update();
//  Serial.print("Heart rate: ");
//  Serial.print(pox.getHeartRate());
//  Serial.print(" bpm, SpO2: ");
//  Serial.print(pox.getSpO2());
//  Serial.println("%");
//  delay(1000);
//}




//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include "MAX30100_PulseOximeter.h"
//
//#define REPORTING_PERIOD_MS     1000
//
//// Create a PulseOximeter object
//PulseOximeter pox;
//
//// Time at which the last beat occurred
//uint32_t tsLastReport = 0;
//
//// Callback routine is executed when a pulse is detected
//void onBeatDetected() {
//    Serial.println("Beat!");
//}
//
//void setup() {
//    Serial.begin(9600);
//    Serial.print ("fodeu");
//    Serial.print("Initializing pulse oximeter..");
//
//    // Initialize sensor
//    if (!pox.begin()) {
//        Serial.println("FAILED");
//        for(;;);
//    } else {
//        Serial.println("SUCCESS");
//    } 
//  pox.setIRLedCurrent(MAX30100_LED_CURR_46_8MA);
//
//    // Register a callback routine
//    pox.setOnBeatDetectedCallback(onBeatDetected);
//}
//
//void loop() {
//    // Read from the sensor
//    pox.update();
//    Serial.print ("fodeu3");
//
//    // Grab the updated heart rate and SpO2 levels
//    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
//        Serial.print("Heart rate:");
//        Serial.print(pox.getHeartRate());
//        Serial.print("bpm / SpO2:");
//        Serial.print(pox.getSpO2());
//        Serial.println("%");
//
//        tsLastReport = millis();
//    }
//}





//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_MPU6050.h>
//#include <MAX30100_PulseOximeter.h>
//
//PulseOximeter pox;
//
//float HeartRate;
//float SpO2;
//
//void setup() {
//  Serial.begin(115200);
//  while (!Serial) {
//    ;
//  }
//  
//  Wire.begin();
//  pox.begin();
//  Serial.println("MPU6050 & MAX30100 test!");
//}
//
//void loop() {
//   
//  HeartRate = pox.getHeartRate();
//  SpO2 = pox.getSpO2();
//
//  float bpm = HeartRate;
//  float ox = SpO2;
//
//  Serial.println("BPM: "); Serial.print(bpm);
//  Serial.println(" SpO2: "); Serial.print(ox);
//
//  delay(500);
//}
















//
//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_MPU6050.h>
//#include <MAX30100_PulseOximeter.h>
//
//#define MPU6050_I2C_ADDRESS 18
//#define I2Ctwo 19
//
//Adafruit_MPU6050 mpu;
//PulseOximeter pox;
//
//int HeartRate;
//int SpO2;
//
//void setup() {
//  Serial.begin(115200);
//  while (!Serial) {
//    ;
//  }
//  
//  Wire.begin();
//  pox.begin();
//  mpu.begin(MPU6050_I2C_ADDRESS, &I2Ctwo);
//
//
//  Serial.println("MPU6050 & MAX30100 test!");
//}
//
//void loop() {
//  
//  sensors_event_t accel, gyro, temp;
//  mpu.getEvent(&accel, &gyro, &temp);
//
//  float temperature = temp.temperature;
//  float ax = accel.acceleration.x;
//  float ay = accel.acceleration.y;
//  float az = accel.acceleration.z;
//  float gx = gyro.gyro.x;
//  float gy = gyro.gyro.y;
//  float gz = gyro.gyro.z;
//  
//  HeartRate = pox.getHeartRate();
//  SpO2 = pox.getSpO2();
//
//  float bpm = HeartRate;
//  float ox = SpO2;
//
//  Serial.println("temperature: ");Serial.print(temperature);
// 
//  Serial.println("AccelerationX: ");Serial.print(ax);
//  Serial.println(" AccelerationY: ");Serial.print(ay);
//  Serial.println(" AccelerationZ: ");Serial.print(az);
//  
//  Serial.println("GyroX: ");Serial.print(gx);
//  Serial.println(" GyroY: ");Serial.print(gy);
//  Serial.println(" GyroZ: ");Serial.print(gz);
//  
//  Serial.println("BPM: ");Serial.print(bpm);
//  Serial.println(" SpO2: ");Serial.print(ox);
//
//  delay(500);
//}


//#include <Wire.h>
//#include "MAX30100_PulseOximeter.h"
//#define REPORTING_PERIOD_MS     1000
//
//PulseOximeter pox;
//
//int HeartRate;
//int SpO2;
//uint32_t tsLastReport = 0;
//void onBeatDetected()
//{
//  Serial.println("Beat!");
//}
//void setup()
//{
//  Serial.begin(115200);
//  Serial.print("Initializing...");
//  // Initialize the PulseOximeter instance
//  // Failures are generally due to an improper I2C wiring, missing power supply
//  // or wrong target chip
//  if (!pox.begin()) {
//    Serial.println("MAX30100 was not found. Please check the wiring/power.");
//    for (;;);
//  } else {
//    Serial.println("SUCCESS");
//  }
//  // The default current for the IR LED is 50mA and it could be changed
//  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
//  //   available options.
//  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
//  // Register a callback for the beat detection
//  pox.setOnBeatDetectedCallback(onBeatDetected);
//}
//void loop()
//{
//  // Make sure to call update as fast as possible
//  pox.update();
//  // Asynchronously dump heart rate and oxidation levels to the serial
//  // For both, a value of 0 means "invalid"
//  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
//    HeartRate = pox.getHeartRate();
//    SpO2 = pox.getSpO2();
//    if  (HeartRate > 0)
//    {
//      Serial.print("Heart rate: ");
//      Serial.print(HeartRate);
//      Serial.print(" SpO2: ");
//      Serial.print(SpO2);
//      Serial.println("%");
//      
//      tsLastReport = millis();
//    }
//  }
//}


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
