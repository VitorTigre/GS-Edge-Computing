#include <WiFi.h>
#include <HTTPClient.h>

// Pinos onde os potenciômetros estão conectados
const int potFocus = 34;  
const int potStress = 35; 

// Pinos do LED RGB
const int ledR = 25;
const int ledG = 26;
const int ledB = 27;

// Dados da rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// API do ThingSpeak
String apiKey = "N5PJLRLHQ4WSFHTR";
String server = "https://api.thingspeak.com/update?api_key=";

// Variáveis para controle do tempo focado
unsigned long focusStart = 0;
unsigned long focusLimit = 60000 * 45; // 45 minutos
bool focusing = false;

void setup() {
  Serial.begin(115200);
  
  // Define entradas dos potenciômetros
  pinMode(potFocus, INPUT);
  pinMode(potStress, INPUT);

  // Define saídas do LED
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  apagarLed(); // Começa com o LED apagado

  // Conecta ao Wi-Fi
  Serial.print("Conectando ao Wi-Fi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400); // Espera até conectar
  }
}

void loop() {
  // Lê os valores dos potenciômetros (0–4095)
  int focusValue = analogRead(potFocus);
  int stressValue = analogRead(potStress);
  
  // Converte para porcentagem (0–100)
  int focusLevel = map(focusValue, 0, 4095, 0, 100);
  int stressLevel = map(stressValue, 0, 4095, 0, 100);

  // Atualiza a cor do LED baseado no foco
  setColorByFocus(focusLevel);

  // Se o foco está acima de 70, começa a contar tempo
  if (focusLevel > 70) {
    if (!focusing) {
      focusing = true;
      focusStart = millis(); // salva o momento que começou
    }
  } else {
    focusing = false; // se o foco cair, para de contar
  }

  // Se passou de 45 minutos focando, reseta
  if (focusing && (millis() - focusStart >= focusLimit)) {
    focusing = false;
  }

  // Envia os dados para o ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Monta o link com os valores de foco e estresse
    String url = server + apiKey +
                 "&field1=" + String(focusLevel) +
                 "&field2=" + String(stressLevel);

    http.begin(url);   // abre o link
    int httpCode = http.GET(); // envia os dados
    http.end(); // finaliza a conexão
  }

  delay(15000); // envia a cada 15 segundos
}

void setColorByFocus(int focus) {
  apagarLed(); // limpa antes de acender
  
  // Define cor conforme nível de foco
  if (focus <= 20) setColor(0, 0, 255);        // Azul
  else if (focus <= 40) setColor(0, 255, 255); // Ciano
  else if (focus <= 60) setColor(255, 255, 0); // Amarelo
  else if (focus <= 80) setColor(0, 255, 0);   // Verde
  else setColor(255, 0, 0);                    // Vermelho
}

void setColor(int r, int g, int b) {
  // Ajusta brilho das cores do LED RGB
  analogWrite(ledR, r);
  analogWrite(ledG, g);
  analogWrite(ledB, b);
}

void apagarLed() {
  // Desliga todas as cores
  analogWrite(ledR, 0);
  analogWrite(ledG, 0);
  analogWrite(ledB, 0);
}
