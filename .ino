#include <WiFi.h>
#include <HTTPClient.h>
const int potFocus = 34;  
const int potStress = 35; 

const int ledR = 25;
const int ledG = 26;
const int ledB = 27;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
String apiKey = "N5PJLRLHQ4WSFHTR";
String server = "https://api.thingspeak.com/update?api_key=";


unsigned long focusStart = 0;
unsigned long focusLimit = 60000 * 45; 
bool focusing = false;

void setup() {
  Serial.begin(115200);
  
  pinMode(potFocus, INPUT);
  pinMode(potStress, INPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  apagarLed();

  // Wi-Fi
  Serial.print("Conectando ao Wi-Fi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
  }
}

void loop() {
  int focusValue = analogRead(potFocus);
  int stressValue = analogRead(potStress);
  
  int focusLevel = map(focusValue, 0, 4095, 0, 100);
  int stressLevel = map(stressValue, 0, 4095, 0, 100);

  setColorByFocus(focusLevel);

  if (focusLevel > 70) {
    if (!focusing) {
      focusing = true;
      focusStart = millis();
    }
  } else {
    focusing = false;
  }

  if (focusing && (millis() - focusStart >= focusLimit)) {
    focusing = false;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = server + apiKey + "&field1=" + String(focusLevel) + "&field2=" + String(stressLevel);
    http.begin(url);
    int httpCode = http.GET();
    http.end();
  }
  delay(15000);
}

void setColorByFocus(int focus) {
  apagarLed();

  if (focus <= 20) setColor(0, 0, 255);       
  else if (focus <= 40) setColor(0, 255, 255); 
  else if (focus <= 60) setColor(255, 255, 0); 
  else if (focus <= 80) setColor(0, 255, 0);  
  else setColor(255, 0, 0);                    
}

void setColor(int r, int g, int b) {
  analogWrite(ledR, r);
  analogWrite(ledG, g);
  analogWrite(ledB, b);
}

void apagarLed() {
  analogWrite(ledR, 0);
  analogWrite(ledG, 0);
  analogWrite(ledB, 0);
}
