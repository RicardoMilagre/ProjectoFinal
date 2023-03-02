// BIBLIOTECAS
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <UrlEncode.h>
#include <HTTPClient.h>

// DEFINIÇÕES
Adafruit_MPU6050 mpu;

// DEFINIÇÃO DOS LIMITE 
float accel_limite = 10.0; // LIMITE PARA ACCELL
float gyro_limite = 2.0; // LIMITE PARA GYRO

// DEFINIÇÕES WIFI
const char* ssid = "C-137";
const char* password = "18e457b313";

//DEFINIÇÕES P/CALLME BOT
String phoneNumber = "+351963151007";
String apiKey = "7129350";

// INICIAR
void setup() 
{
  // CONECTAR SERIAL
  Serial.begin(115200);
  Wire.begin();
  
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

  // INICIAR MPU
  mpu.begin();

  Serial.println("Teste MPU6050");
  delay(1000);
  if (!mpu.begin())
  {
    Serial.println();
    Serial.println("Falha ao conectar com o MPU6050");
    while (1) 
    {
      delay(10);
    }
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
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
  // DEFINIÇÃO DE ACORDO COM MPU6050
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  // CALCULO DAS MAGNITUDES
  float accel_magnitude = (sqrt(accel.acceleration.x*accel.acceleration.x +
                              accel.acceleration.y*accel.acceleration.y +
                              accel.acceleration.z*accel.acceleration.z))-9.8;
  float gyro_magnitude = sqrt(gyro.gyro.x*gyro.gyro.x +
                               gyro.gyro.y*gyro.gyro.y +
                               gyro.gyro.z*gyro.gyro.z);

  // CALCULO HORIZONTAL/VERTICAL
  float pitch = atan2(accel.acceleration.x, sqrt(accel.acceleration.y*accel.acceleration.y +
                                                    accel.acceleration.z*accel.acceleration.z)) * 180.0 / PI;
  float roll = atan2(accel.acceleration.y, sqrt(accel.acceleration.x*accel.acceleration.x +
                                                   accel.acceleration.z*accel.acceleration.z)) * 180.0 / PI;

  if (accel_magnitude > accel_limite && gyro_magnitude < gyro_limite)
  {
    Serial.println("Queda Detectada!");
    Serial.print("Horizontal: ");
    Serial.print(pitch);
    Serial.print(", Vertical: ");
    Serial.println(roll);
    sendMessage("Queda detectada!");
    delay(1500);
    // take appropriate action here (e.g. send a notification, sound an alarm)
  }

  delay(100);
}
















//#include <Wire.h>
//#include <Adafruit_MPU6050.h>
//#include <WiFi.h>
//#include <UrlEncode.h>
//
//Adafruit_MPU6050 mpu;
//
//void setup() {
//  Serial.begin(115200);
//  Wire.begin();
//  mpu.begin();
//  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
//  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
//  
//}
//
//void loop() {
//  sensors_event_t accel, gyro, temp;
//  mpu.getEvent(&accel, &gyro, &temp);
//
//  float accel_magnitude = (sqrt(accel.acceleration.x*accel.acceleration.x +
//                              accel.acceleration.y*accel.acceleration.y +
//                              accel.acceleration.z*accel.acceleration.z))-9.8;
//  float gyro_magnitude = sqrt(gyro.gyro.x*gyro.gyro.x +
//                               gyro.gyro.y*gyro.gyro.y +
//                               gyro.gyro.z*gyro.gyro.z);
//
//  float pitch = atan2(accel.acceleration.x, sqrt(accel.acceleration.y*accel.acceleration.y +
//                                                    accel.acceleration.z*accel.acceleration.z)) * 180.0 / PI;
//  float roll = atan2(accel.acceleration.y, sqrt(accel.acceleration.x*accel.acceleration.x +
//                                                   accel.acceleration.z*accel.acceleration.z)) * 180.0 / PI;
//
//  float accel_limite = 10.0; // LIMITE PARA ACCELL
//  float gyro_limite = 2.0; // LIMITE PARA GYRO
//
//  if (accel_magnitude > accel_limite && gyro_magnitude < gyro_limite)
//  {
//    Serial.println("Queda Detectada!");
//    Serial.print("Horizontal: ");
//    Serial.print(pitch);
//    Serial.print(", Vertical: ");
//    Serial.println(roll);
//    delay(1500);
//    // take appropriate action here (e.g. send a notification, sound an alarm)
//  }
//
//  delay(100);
//}
