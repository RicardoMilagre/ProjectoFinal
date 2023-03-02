// BIBLIOTECAS
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <UrlEncode.h>
#include <HTTPClient.h>

// DEFINIÇÕES
Adafruit_MPU6050 mpu;
HTTPClient http;

// DEFINIÇÃO DOS LIMITE 
float accel_limite = 10.0; // LIMITE PARA ACCELL
float gyro_limite = 2.0; // LIMITE PARA GYRO

// DEFINIÇÕES WIFI
const char* ssid = "C-137";
const char* password = "18e457b313";
const char* host = "192.168.1.72";
WiFiClient client;

//DEFINIÇÕES P/CALLME BOT
String phoneNumber = "+351963151007";
String apiKey = "7129350";
String response = "";
String url = "http://localhost:8080/connect?";

// INICIAR
void setup() 
{
  // CONECTAR SERIAL
  Serial.print ("A iniciar serial");
  Serial.begin(115200);
  Wire.begin();
  
  // CONECTAR WIFI
  Serial.println("");
  WiFi.begin(ssid, password);
  Serial.println("A iniciar WiFi");
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(250);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.print("Conectado com Sucesso!");
    Serial.println("IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
  }
  else
  {
    Serial.println("");
    Serial.print("Falha a inciar WiFi");
  }

  // INICIAR MPU
  mpu.begin();

  Serial.println("A iniciar MPU6050");
  delay(1000);
  if (!mpu.begin())
  {
    Serial.println("");
    Serial.println("Falha ao iniciar MPU6050");
    while (1) 
    {
      delay(10);
    }
  }

  // DEFINIÇÕES DE RANGE
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
    Serial.println("Retomando leituras");
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
  sensors_event_t accel, gyro, temp;  // TEMP NÃO USADO
  mpu.getEvent(&accel, &gyro, &temp); //TEMP NÃO USADO

  // CALCULO DAS MAGNITUDES
  // MAGNITUDE ACELERAÇÃO - RETIRAMOS 9.8, UMA VEZ QUE É O VALOR DA ACELERAÇÃO GRAVITICA
  float accel_magnitude = (sqrt(accel.acceleration.x*accel.acceleration.x +
                              accel.acceleration.y*accel.acceleration.y +
                              accel.acceleration.z*accel.acceleration.z))-9.8;

  // MAGNITUDE ANGULO                     
  float gyro_magnitude = sqrt(gyro.gyro.x*gyro.gyro.x +
                               gyro.gyro.y*gyro.gyro.y +
                               gyro.gyro.z*gyro.gyro.z);

  // CALCULO HORIZONTAL/VERTICAL COM CONVERSÃO PARA GRAUS
  float pitch = atan2(accel.acceleration.x, sqrt(accel.acceleration.y*accel.acceleration.y +
                                                    accel.acceleration.z*accel.acceleration.z)) * 180.0 / PI;
  float roll = atan2(accel.acceleration.y, sqrt(accel.acceleration.x*accel.acceleration.x +
                                                   accel.acceleration.z*accel.acceleration.z)) * 180.0 / PI;

  // CASO PASSE OS LIMITES ESTABLECIDOS PELO USER, EMITE UM ALERTA DE QUEDA
  if (accel_magnitude > accel_limite && gyro_magnitude < gyro_limite)
  {
    // POR UMA QUESTÃO DE COMPREENSÃO IMPRIME EM SERIAL, TODOS OS VALORES SÃO GUARDADOS EM DB
    Serial.println("Queda Detectada!");
    Serial.print("Horizontal: ");
    Serial.print(pitch);
    Serial.print(", Vertical: ");
    Serial.println(roll);
    sendMessage("ATENÇÃO: Foi detectada uma queda por parte do User, por favor verifique o estado do mesmo.");
    // DELAY DEVIDO A POSSIVEIS INCONGRUENCIAS DE LEITURAS SEGUINTES
    delay(1500);
  }
  Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 8080;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

      String url = "/connect?";
    url += "acceleration_x=" + String(accel.acceleration.x) + "&";
    url += "acceleration_y=" + String(accel.acceleration.y) + "&";
    url += "acceleration_z=" + String(accel.acceleration.z) + "&";
    url += "gyro_x=" + String(gyro.gyro.x) + "&";
    url += "gyro_y=" + String(gyro.gyro.y) + "&";
    url += "gyro_z=" + String(gyro.gyro.z);

    // Send the HTTP request
    Serial.println("Sending HTTP request...");
    client.print(String("GET ") + url + "%20HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

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
  






  // URL
//  String url = "/connect?";
//  url += "acceleration_x=" + String(accel.acceleration.x) + "&";
//  url += "acceleration_y=" + String(accel.acceleration.y) + "&";
//  url += "acceleration_z=" + String(accel.acceleration.z) + "&";
//  url += "gyro_x=" + String(gyro.gyro.x) + "&";
//  url += "gyro_y=" + String(gyro.gyro.y) + "&";
//  url += "gyro_z=" + String(gyro.gyro.z);
//
//  // Send the HTTP request
//  Serial.println("Sending HTTP request...");
//  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" +
//               "Connection: close\r\n\r\n");

  
//  Serial.print("Connecting to ");
//  Serial.println(host);
//  Serial.println("");
//  // Use WiFiClient class to create TCP connections
//  WiFiClient client;
//  //const int httpPort = 3306;
//  if (!client.connect(host)//, httpPort)) 
//  {
//      Serial.println("connection to DB failed");
//      return;
//  }

  // This will send the request to the server
                 
  
//  String url = "http://localhost:8080/connect?";
//  http.begin(url);
//  int httpcode = http.GET();
//  Serial.println("httpcode:");
//  Serial.println(httpcode);
//  if (httpcode == HTTP_CODE_OK)
//  {
//    Serial.print ("php ok");
//    String response = http.getString();
//  }
//  delay (1500);

//Serial.print("POST " + url + 
//                          ("&acceleration_x=") + String(accel.acceleration.x) +
//                          ("&acceleration_y=") + String(accel.acceleration.y) +
//                          ("&acceleration_z=") + String(accel.acceleration.z) +
//                          ("&gyro_x=") + String(gyro.gyro.x) +
//                          ("&gyro_y=") + String(gyro.gyro.y) +
//                          ("&gyro_z=") + String(gyro.gyro.z) +
//                         "%20HTTP/1.1\r\n" +
//                 "Host: " + host + "\r\n" +
//                 "Connection: close\r\n\r\n");
//client.print("POST " + url + 
//                          ("&acceleration_x=") + String(accel.acceleration.x) +
//                          ("&acceleration_y=") + String(accel.acceleration.y) +
//                          ("&acceleration_z=") + String(accel.acceleration.z) +
//                          ("&gyro_x=") + String(gyro.gyro.x) +
//                          ("&gyro_y=") + String(gyro.gyro.y) +
//                          ("&gyro_z=") + String(gyro.gyro.z) +
//                         "%20HTTP/1.1\r\n" +
//                 "Host: " + host + "\r\n" +
//                 "Connection: close\r\n\r\n");
//    unsigned long timeout = millis();
//    while (client.available() == 0) 
//    {
//        // THRESHOLD PARA TIMEOUT ESTABLECIDO EM 2 SEGUNDOS
//        if (millis() - timeout > 2000) {
//            Serial.println(">>> Client Timeout !");
//            client.stop();
//            return;
//        }
//    }
//
//    // Read all the lines of the reply from server and print them to Serial
//    while(client.available()) {
//        String line = client.readStringUntil('\r');
//        Serial.print(line);
//        
//    }
//
//    Serial.println();
//    Serial.println("Ligação Fechada");
//  delay(100);















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
