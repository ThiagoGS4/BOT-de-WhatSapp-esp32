#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "inserir nome da rede wifi";
const char* password = "inserir senha do wifi";

String phoneNumber = "+5519982219011"; //aqui insere-se o número de telefone
String apiKey = "7138013"; //inserir o numero da api gerado pelo site callmebot

void mandarMensagem(String messagem) {

  // dados a serem enviados via HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(messagem);
  HTTPClient http;
  http.begin(url);

  // Especificar conteudo de header
  String headerName = "Content-Type";
  String headerValue = "application/x-www-form-urlencoded";
  http.addHeader(headerName, headerValue);

  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.println("mensagem enviada com sucesso!");
  } else {
    Serial.println("Ocorreu um erro ao enviar a mensagem!");
    Serial.print("codigo de resposta HTTP:");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("sensor DHTxx teste!"));

  dht.begin();

   pinMode(4, INPUT); //pino 4 definido para entrada de dados

  WiFi.begin(ssid, password);
  Serial.println("Conectando...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado ao endereco de IP: ");
  Serial.println(WiFi.localIP());

  // Exemplo de mensagem para WhatsApp
  // sendMessage("teste");
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // a leitura de humidade e temperatura dura aprox. 3ms
  // pode cheagar até 2s de delay para ler
  float h = dht.readHumidity();
  // Ler temeperatura em °C
  float t = dht.readTemperature();

  // checka se a leitura funcionou.
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    mandarMensagem("Failed to read from DHT sensor!");
    return;
  }

  // Computa calor em celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

    // Mandar mensagem com temperatura e humidade
  mandarMensagem("Temperature: " + String(t) + "°C, Humidity: " + String(h) + "%");

}
