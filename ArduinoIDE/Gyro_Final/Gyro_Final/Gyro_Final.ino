// versão 3 - melhoramentos

#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const char* ssid = "C-137";
const char* password = "18e457b313";
const char* host = "192.168.1.72";

Adafruit_MPU6050 mpu;

void setup(void) {
  // INICIALIZAR SERIAL
  Serial.begin(115200);
  while (!Serial)
    delay(500); // will pause Zero, Leonardo, etc until serial console opens

  delay(2000);
  // TESTE ENCONTRAR CHIP
  Serial.println();
  Serial.println("Adafruit MPU6050 test!");
  if (!mpu.begin()) {
    Serial.println();
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println();
  Serial.println("MPU6050 Found!");

  delay(2000);
  // WIFI CONEXÃO
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print (".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
  delay(2000);

  // DEFINIÇÕES SENSORES
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("");
  Serial.println("Sensors Calibrated");
  delay(2000);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float acceleration_x = a.acceleration.x;
  float acceleration_y = a.acceleration.y;
  float acceleration_z = a.acceleration.z;

  float gyro_x = g.gyro.x;
  float gyro_y = g.gyro.y;
  float gyro_z = g.gyro.z;
  
  /* Print out the values */
  Serial.print(acceleration_x);
  //Serial.print(",");
  Serial.print(acceleration_y);
  //Serial.print(",");
  Serial.print(acceleration_z);
  //Serial.print(",");
  Serial.print(gyro_x);
  //Serial.print(",");
  Serial.print(gyro_y);
  //Serial.print(",");
  Serial.print(gyro_z);

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 3306;
  if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
  }

  // This will send the request to the server
  client.print(String(("GET https://drive.google.com/file/d/1k00AmL-TohKmcbsGlf4I925742Kb-Gjr/view?usp=share_link") + 
                          ("&acceleration_x=") + acceleration_x +
                          ("&acceleration_y=") + acceleration_y +
                          ("&acceleration_z=") + acceleration_z +
                          ("&gyro_x=") + gyro_x +
                          ("&gyro_y=") + gyro_y +
                          ("&gyro_z=") + gyro_z +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n"));
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
    }

    Serial.println();
    Serial.println("closing connection");
}

































// versão 1 - impressão em serial.

//#include <Adafruit_MPU6050.h>
//#include <Adafruit_Sensor.h>
//#include <Wire.h>
//
//Adafruit_MPU6050 mpu;
//
//void setup(void) {
//  Serial.begin(115200);
//  while (!Serial)
//    delay(10); // will pause Zero, Leonardo, etc until serial console opens
//
//  Serial.println("Adafruit MPU6050 test!");
//
//  // Try to initialize!
//  if (!mpu.begin()) {
//    Serial.println("Failed to find MPU6050 chip");
//    while (1) {
//      delay(10);
//    }
//  }
//  Serial.println("MPU6050 Found!");
//
//  //setupt motion detection
//  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
//  mpu.setMotionDetectionThreshold(1);
//  mpu.setMotionDetectionDuration(20);
//  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
//  mpu.setInterruptPinPolarity(true);
//  mpu.setMotionInterrupt(true);
//
//  Serial.println("");
//  delay(100);
//}
//
//void loop() {
//
//  if(mpu.getMotionInterruptStatus()) {
//    /* Get new sensor events with the readings */
//    sensors_event_t a, g, temp;
//    mpu.getEvent(&a, &g, &temp);
//
//    /* Print out the values */
//    Serial.print("AccelX:");
//    Serial.print(a.acceleration.x);
//    Serial.print(",");
//    Serial.print("AccelY:");
//    Serial.print(a.acceleration.y);
//    Serial.print(",");
//    Serial.print("AccelZ:");
//    Serial.print(a.acceleration.z);
//    Serial.print(", ");
//    Serial.print("GyroX:");
//    Serial.print(g.gyro.x);
//    Serial.print(",");
//    Serial.print("GyroY:");
//    Serial.print(g.gyro.y);
//    Serial.print(",");
//    Serial.print("GyroZ:");
//    Serial.print(g.gyro.z);
//    Serial.println("");
//  }
//
//  delay(10);
//}



//  VERÃO 2 - WEBSERVER

//#include <Arduino.h>
//  #include <Adafruit_MPU6050.h>
//  #include <Adafruit_Sensor.h>
//  #include <Arduino_JSON.h>
//  #include <ESPAsyncWebServer.h>
//  #include <AsyncTCP.h>
//  #include "SPIFFS.h"
//  
//  #define m_pi 3.141592653589793238
//  
//  
//  // Replace with your network credentials
//  const char* ssid = "C-137";
//  const char* password = "18e457b313";
//  
//  // Create AsyncWebServer object on port 80
//  AsyncWebServer server(80);
//  
//  // Create an Event Source on /events
//  AsyncEventSource events("/events");
//  
//  // Json Variable to Hold Sensor Readings
//  JSONVar readings;
//  
//  // Timer variables
//  unsigned long lastTime = 0;  
//  unsigned long lastTimeTemperature = 0;
//  unsigned long lastTimeAcc = 0;
//  unsigned long gyroDelay = 10;
//  unsigned long temperatureDelay = 1000;
//  unsigned long accelerometerDelay = 200;
//  
//  boolean fall = false; //stores if a fall has occurred
//  boolean trigger1=false; //stores if first trigger (lower threshold) has occurred
//  boolean trigger2=false; //stores if second trigger (upper threshold) has occurred
//  boolean trigger3=false; //stores if third trigger (orientation change) has occurred
//  byte trigger1count=0; //stores the counts past since trigger 1 was set true
//  byte trigger2count=0; //stores the counts past since trigger 2 was set true
//  byte trigger3count=0; //stores the counts past since trigger 3 was set true
//  int angleChange=0;
//  
//  // Create a sensor object
//  Adafruit_MPU6050 mpu;
//  
//  sensors_event_t a, g, temp;
//  
//  float gyroX, gyroY, gyroZ;
//  float accX, accY, accZ;
//  float temperature;
//  
//  //Gyroscope sensor deviation
//  float gyroXerror = 0.07;
//  float gyroYerror = 0.03;
//  float gyroZerror = 0.01;
//  
//  // Init MPU6050
//  void initMPU()
//  {
//    if (!mpu.begin())
//    {
//      Serial.println("Failed to find MPU6050 chip");
//      while (1)
//      {
//        delay(10);
//      }
//    }
//    Serial.println("MPU6050 Found!");
//  }
//  
//  void initSPIFFS()
//  {
//    if (!SPIFFS.begin())
//    {
//      Serial.println("An error has occurred while mounting SPIFFS");
//    }
//    Serial.println("SPIFFS mounted successfully");
//  }
//  
//  // Initialize WiFi
//  void initWiFi() 
//  {
//    WiFi.mode(WIFI_STA);
//    WiFi.begin(ssid, password);
//    Serial.println("");
//    Serial.print("Connecting to WiFi...");
//    while (WiFi.status() != WL_CONNECTED) 
//    {
//      Serial.print(".");
//      delay(1000);
//    }
//    Serial.println("");
//    Serial.println(WiFi.localIP());
//  }
//  
//  String getGyroReadings()
//  {
//    mpu.getEvent(&a, &g, &temp);
//  
//    float gyroX_temp = g.gyro.x;
//    if(abs(gyroX_temp) > gyroXerror)
//    {
//      gyroX += gyroX_temp/100.00;
//    }
//    
//    float gyroY_temp = g.gyro.y;
//    if(abs(gyroY_temp) > gyroYerror)
//    {
//      gyroY += gyroY_temp/70.00;
//    }
//  
//    float gyroZ_temp = g.gyro.z;
//    if(abs(gyroZ_temp) > gyroZerror)
//    {
//      gyroZ += gyroZ_temp/90.00;
//    }
//  
//    readings["gyroX"] = String(gyroX);
//    readings["gyroY"] = String(gyroY);
//    readings["gyroZ"] = String(gyroZ);
//  
//    String jsonString = JSON.stringify(readings);
//    return jsonString;
//  }
//  
//  String getAccReadings() 
//  {
//    mpu.getEvent(&a, &g, &temp);
//    mpu.getEvent(&a, &g, &temp);
//  
//    float gyroX_temp = g.gyro.x;
//    if(abs(gyroX_temp) > gyroXerror)
//    {
//      gyroX += gyroX_temp/100.00;
//    }
//    
//    float gyroY_temp = g.gyro.y;
//    if(abs(gyroY_temp) > gyroYerror)
//    {
//      gyroY += gyroY_temp/70.00;
//    }
//  
//    float gyroZ_temp = g.gyro.z;
//    if(abs(gyroZ_temp) > gyroZerror)
//    {
//      gyroZ += gyroZ_temp/90.00;
//    }
//
//
//    
//    // Get current acceleration values
//    accX = a.acceleration.x;
//    accY = a.acceleration.y;
//    readings["accX"] = String(accX);
//    readings["accY"] = String(accY);
//    readings["accZ"] = String(accZ);
//    String accString = JSON.stringify (readings);
//    return accString;
//  }
//  
//  String getTemperature()
//  {
//    mpu.getEvent(&a, &g, &temp);
//    temperature = temp.temperature;
//    return String(temperature);
//  }
//  
//  void setup() 
//  {
//    Serial.begin(115200);
//    initWiFi();
//    initSPIFFS();
//    initMPU();
//  
//    // Handle Web Server
//    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
//    {
//      request->send(SPIFFS, "/index.html", "text/html");
//    });
//  
//    server.serveStatic("/", SPIFFS, "/");
//  
//    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
//    {
//      gyroX=0;
//      gyroY=0;
//      gyroZ=0;
//      request->send(200, "text/plain", "OK");
//    });
//  
//    server.on("/resetX", HTTP_GET, [](AsyncWebServerRequest *request)
//    {
//      gyroX=0;
//      request->send(200, "text/plain", "OK");
//    });
//  
//    server.on("/resetY", HTTP_GET, [](AsyncWebServerRequest *request)
//    {
//      gyroY=0;
//      request->send(200, "text/plain", "OK");
//    });
//  
//    server.on("/resetZ", HTTP_GET, [](AsyncWebServerRequest *request){
//      gyroZ=0;
//      request->send(200, "text/plain", "OK");
//    });
//  
//    // Handle Web Server Events
//    events.onConnect([](AsyncEventSourceClient *client)
//    {
//      if(client->lastId())
//      {
//        Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
//      }
//      // send event with message "hello!", id current millis
//      // and set reconnect delay to 1 second
//      client->send("hello!", NULL, millis(), 10000);
//    });
//    
//    server.addHandler(&events);
//    server.begin();
//  }
//  
//  void loop() 
//  {
//    if ((millis() - lastTime) > gyroDelay)
//    {
//      // Send Events to the Web Server with the Sensor Readings
//      events.send(getGyroReadings().c_str(),"gyro_readings",millis());
//      lastTime = millis();
//    }
//    if ((millis() - lastTimeAcc) > accelerometerDelay) 
//    {
//      // Send Events to the Web Server with the Sensor Readings
//      events.send(getAccReadings().c_str(),"accelerometer_readings",millis());
//      lastTimeAcc = millis();
//    }
//    if ((millis() - lastTimeTemperature) > temperatureDelay) 
//    {
//      // Send Events to the Web Server with the Sensor Readings
//      events.send(getTemperature().c_str(),"temperature_reading",millis());
//      lastTimeTemperature = millis();
//    }
//
//    mpu.getEvent(&a, &g, &temp);
//    mpu.getEvent(&a, &g, &temp);
//  
//    float gyroX_temp = g.gyro.x;
//    if(abs(gyroX_temp) > gyroXerror)
//    {
//      gyroX += gyroX_temp/100.00;
//    }
//    
//    float gyroY_temp = g.gyro.y;
//    if(abs(gyroY_temp) > gyroYerror)
//    {
//      gyroY += gyroY_temp/70.00;
//    }
//  
//    float gyroZ_temp = g.gyro.z;
//    if(abs(gyroZ_temp) > gyroZerror)
//    {
//      gyroZ += gyroZ_temp/90.00;
//    }
//
//
//    
//    // Get current acceleration values
//    accX = a.acceleration.x;
//    accY = a.acceleration.y;
//    accZ = a.acceleration.z;
//    float raw_amplitude = pow(pow(accX,2)+pow(accY,2)+pow(accZ,2),0.5);
//    int amplitude = raw_amplitude;  // Mulitiplied by 10 bcz values are between 0 to 1
//    Serial.println(amplitude);
//    if (amplitude<=5 && trigger2==false){ //if AM breaks lower threshold (0.4g) //10
//      trigger1=true;
//      Serial.println("TRIGGER 1 ACTIVATED");
//    }
//    if (trigger1==true)
//    {
//      trigger1count++;
//      if (amplitude>=12)
//      { //if AM breaks upper threshold (3g)
//        trigger2=true;
//        Serial.println("TRIGGER 2 ACTIVATED");
//        trigger1=false; trigger1count=0;
//      }
//    }
//    if (trigger2==true)
//    {
//      trigger2count++;
//      angleChange = pow(pow(gyroX,2)+pow(gyroY,2)+pow(gyroZ,2),0.5); 
//      Serial.println(angleChange);
//      if (angleChange>=5 && angleChange<=400)  //30+400
//      { //if orientation changes by between 80-100 degrees
//        trigger3=true; trigger2=false; trigger2count=0;
//        Serial.println(angleChange);
//        Serial.println("TRIGGER 3 ACTIVATED");
//      }
//    }
//    if (trigger3==true)
//    {
//      trigger3count++;
//      if (trigger3count>=1)
//      { 
//         angleChange = pow(pow(gyroX,2)+pow(gyroY,2)+pow(gyroZ,2),0.5);
//         Serial.println(angleChange); 
//         if ((angleChange>=0) && (angleChange<=10))
//         { //if orientation changes remains between 0-10 degrees
//             fall=true; trigger3=false; trigger3count=0;
//             Serial.println(angleChange);
//         }
//         else
//         { //user regained normal orientation
//            trigger3=false; trigger3count=0;
//            Serial.println("TRIGGER 3 DEACTIVATED");
//         }
//       }
//    }
//    if (fall==true)
//    { //in event of a fall detection
//      Serial.println("FALL DETECTED");
//      fall=false;
//    }
//    if (trigger2count>=6)
//    { //allow 0.5s for orientation change
//      trigger2=false; trigger2count=0;
//      Serial.println("TRIGGER 2 DECACTIVATED");
//    }
//    if (trigger1count>=6)
//    { //allow 0.5s for AM to break upper threshold
//      trigger1=false; trigger1count=0;
//      Serial.println("TRIGGER 1 DECACTIVATED");
//    }
//    delay(100);
//  }  
