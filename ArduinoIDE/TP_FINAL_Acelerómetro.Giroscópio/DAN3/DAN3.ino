// BIBLIOTECAS
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <UrlEncode.h>
#include <HTTPClient.h>

// DEFINIÇÕES
Adafruit_MPU6050 mpu;
HTTPClient http;

// DEFINIÇÃO DOS LIMITE E INICIO DO ESTADO
float accel_limite = 10.0;  // LIMITE PARA ACCELL
float gyro_limite = 2.0;    // LIMITE PARA GYRO
float andar_limite = 2.0;   // LIMITE PARA ANDAR
String estado = "";

// DEFINIÇÕES WIFI
const char* ssid = "C-137";
const char* password = "18e457b313";
const char* host = "192.168.1.72";
WiFiClient client;

//DEFINIÇÕES P/CALLME BOT
String phoneNumber = "+351963151007";
String apiKey = "7129350";

// INICIAR
void setup() 
{
  // CONECTAR SERIAL
  delay(1500);
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
  //HTTPClient http;
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

  // MAGNITUDE THRESHOLD ANDAR
  float accel_andar = sqrt(accel.acceleration.x*accel.acceleration.x +
                              accel.acceleration.y*accel.acceleration.y);

                              
  // CALCULO HORIZONTAL/VERTICAL COM CONVERSÃO PARA GRAUS
  float pitch = atan2(accel.acceleration.x, sqrt(accel.acceleration.y*accel.acceleration.y +
                                                    accel.acceleration.z*accel.acceleration.z)) * 180.0 / PI;
  float roll = atan2(accel.acceleration.y, sqrt(accel.acceleration.x*accel.acceleration.x +
                                                   accel.acceleration.z*accel.acceleration.z)) * 180.0 / PI;  

  // CAOS PASSE O THRESHOLD DEFINIDO
  if (accel_magnitude > accel_limite && gyro_magnitude < gyro_limite)
  {
    // CASO PASSE OS LIMITES ESTABLECIDOS PELO USER, EMITE UM ALERTA DE QUEDA
    Serial.print("");
    Serial.println("Queda Detectada!");
    Serial.print(" | Horizontal: ");
    Serial.print(pitch);
    Serial.print(" | Vertical: ");
    Serial.print(roll);
    Serial.println("");
    sendMessage("ALERTA: Foi detectada uma queda");
    // DELAY DEVIDO A POSSIVEIS INCONGRUENCIAS DE LEITURAS SEGUINTES
  }
  
  // SIMPLIFICAÇÃO DE ESTADO PARADO
  if (accel_andar > -1 || accel_andar < 1)
  {
    estado = "parado";
  }

  //SIMPLIFICAÇÃO ESTADO ANDAR
  if (accel_andar > 1 && accel_andar < 3)
  {
    estado = "andar";
  }

  // SIMPLIFICAÇÃO ESTADO CORRER
  if (accel_andar > 3)
  {
    estado = "correr";
  }

  // IMPRESSÃO EM SERIAL PARA VISUALIZAÇÃO
  Serial.print("Accel x: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" | Accel y: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" | Accel z: ");
  Serial.print(accel.acceleration.z);
  Serial.print(" | Gyro x: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" | Gyro y: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" | Gyro z: ");
  Serial.print(gyro.gyro.z);
  Serial.print(" | Estado: ");
  Serial.print(estado);


  // INDICA CONEXÃO
  Serial.print("A conectar a ");
  Serial.println(host);

  // ESTABLECER CONEXÃO COM WIFICLIENT
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
      Serial.println("conexão falhada");
      return;
  }

  // CONSTRUÇÃO DA STRING DE ENVIO
  String url = "/connectmpu?";
  url += "acceleration_x=" + String(accel.acceleration.x) + "&";
  url += "acceleration_y=" + String(accel.acceleration.y) + "&";
  url += "acceleration_z=" + String(accel.acceleration.z) + "&";
  url += "gyro_x=" + String(gyro.gyro.x) + "&";
  url += "gyro_y=" + String(gyro.gyro.y) + "&";
  url += "gyro_z=" + String(gyro.gyro.z) + "&";
  url += "estado=" + String(estado);

  // GET REQUEST
  Serial.println("A enviar pedido HTTP...");
  client.print(String("GET ") + url + "HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: close\r\n\r\n");

  // THRESHOLD TIMEOUT && ESTABLECER RERUN
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 1000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
      }
  }

  // DEVOLVER O QUE O SERVER INDICA
  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
      
  }
  
  // FECHAR CONEXÃO
  Serial.println();
  Serial.println("Conexão fechada");
  delay (1000);
}
