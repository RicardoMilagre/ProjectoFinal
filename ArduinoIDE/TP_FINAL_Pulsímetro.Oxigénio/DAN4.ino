//BIBLIOTECA
#include <Wire.h>
#include <WiFi.h>
#include "MAX30100_PulseOximeter.h"
#include <UrlEncode.h>
#include <HTTPClient.h>
#define tempoUpdate     1000
#define arrayPulsacao   10

//DEFINIÇÕES
PulseOximeter pox;
HTTPClient http;


// DEFINIÇÃO DOS TEMPOS LIMITE E ARRAYS
int ultimaMedicao = 0;
int arraybpm[arrayPulsacao];
int semPulsacao;

// DEFINIÇÕES WIFI
const char* ssid = "C-137";
const char* password = "18e457b313";
const char* host = "192.168.1.72";
WiFiClient client;

//DEFINIÇÕES P/CALLME BOT
String phoneNumber = "+351963151007";
String apiKey = "7129350";

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

  // INICIAR POX
  pox.begin();

  Serial.println("A iniciar MAX30100");
  delay(1000);
  if (!pox.begin())
  {
    Serial.println("");
    Serial.println("Falha ao iniciar MAX30100");
    while (1) 
    {
      delay(10);
    }
  }

  // CONFIGURAÇÃO LED 8MA
  pox.setIRLedCurrent(MAX30100_LED_CURR_46_8MA);

  // INICIAR ARRAY PULSAÇÃO, VALORES DEFAULT
  for (int i = 0; i < arrayPulsacao; i++) 
  {
      arraybpm[i] = -1;
  }
  
}

// CÓDIGO EDITADO PARA USO DO CALLMEBOT.
// CÓDIGO ORIGINAL DISPONIVEL EM CALLMEBOT.COM
void sendMessage(String message)
{
  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
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
  
    // UPDATE VALORES
    pox.update();

    // VERIFICA SE PASSO O TEMPO NECESSÁRIO ENTRE LEITURAS
    if (millis() - ultimaMedicao > tempoUpdate)
    {
        // LE OS VALORES
        int bpm = pox.getHeartRate();
        int spo2 = pox.getSpO2();

        // GUARDA O NOVO VALOR BPM NO FIM DO ARRAY
        if (bpm > 0) 
        {
            for (int i = 0; i < arrayPulsacao - 1; i++) 
            {
                arraybpm[i] = arraybpm[i + 1];
            }
            arraybpm[arrayPulsacao - 1] = bpm;
        }

        // CALCULA A MEDIA DAS ULTIMAS LEITURAS SE FOR NÃO NULO
        int sum = 0;
        int count = 0;
        for (int i = 0; i < arrayPulsacao; i++)
        {
            if (arraybpm[i] != -1) 
            {
                sum += arraybpm[i];
                count++;
            }
        }
        float mediabpm = (count > 0) ? (float)sum / count : 0;

        // IMPRESSÃO EM SERIAL PARA VISUALIZAÇÃO
        Serial.print("Heart rate:");
        Serial.print(bpm);
        Serial.print("bpm / SpO2:");
        Serial.print(spo2);
        Serial.print("% / Average Heart Rate:");
        Serial.print(mediabpm);
        Serial.println(" bpm");

        // RESET TIMER SE NÃO FOR 0
        if (bpm > 0) 
        {
          semPulsacao = 0;
        } 
        else 
        {
          // INCREMENTA O VALOR SEM PULSAÇÃO
          semPulsacao++;
        }
        
        // TRESHOLD PARA ALERTA DE SEM BATIMENTOS
        if (semPulsacao >= 7) 
        {
          sendMessage("ALERTA! Sem medições de bpm e SpO2.");
          semPulsacao = 0;
        }

        // INDICA CONEXÃO
        Serial.print("A conectar a ");
        Serial.println(host);
    
        // ESTABLECER CONEXÃO COM WIFICLIENT
        const int httpPort = 8080;
        if (!client.connect(host, httpPort)) 
        {
            Serial.println("conexão falhada");
            return;
        }
    
        // CONSTRUÇÃO DA STRING DE ENVIO
        String url = "/connectmax?";
        url += "bpm=" + String(bpm) + "&";
        url += "spo2=" + String(spo2) + "&";
        url += "mediabpm=" + String(mediabpm);
    
        // GET REQUEST
        Serial.println("A enviar pedido HTTP...");
        client.print(String("GET ") + url + "HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
    
        // THRESHOLD TIMEOUT && ESTABLECER RERUN
        unsigned long timeout = millis();
        while (client.available() == 0) 
        {
            if (millis() - timeout > 1000) 
            {
                Serial.println(">>> Client Timeout !");
                client.stop();
                return;
            }
        }
    
        // DEVOLVER O QUE O SERVER INDICA
        while(client.available()) 
        {
            String line = client.readStringUntil('\r');
            Serial.print(line);
            
        }
        
        // FECHAR CONEXÃO
        Serial.println();
        Serial.println("Conexão fechada");
       
        // UPDATE TEMPO ULTIMA MEDIÇÃO
        ultimaMedicao = millis();
    }
}
