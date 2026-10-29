/*
 * Projeto: Relogio e Calendario com Matriz 8x8 (TM1640)
 * Placa: WEMOS D1 Mini (ESP8266)
 * Shield: Matrix LED Shield 8x8 (TM1640) - Pinos D7 (DIN) e D5 (CLK)
 */

#include <ESP8266WiFi.h>
#include <time.h>
#include <WEMOS_Matrix_GFX.h>

// Credenciais Wi-Fi
const char* ssid     = "S729-1";
const char* password = "25102026";

// Configuracao NTP / Fuso Horario Brasilia (UTC -3)
const long gmtOffset_sec = -3 * 3600;
const int daylightOffset_sec = 0;
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "a.st1.ntp.br";

// Inicializacao do display TM1640 (Brilho padrao 3 de 7)
MLED matrix(3);

// Variaveis de controle de texto e animacao
char scrollMessage[80] = "Conectando WiFi...";
int scrollX = 8;
unsigned long lastScrollTime = 0;
const unsigned long scrollSpeedMs = 60; // Velocidade de rolagem (ms por pixel)

void updateTimeMessage() {
  time_t now = time(nullptr);
  struct tm timeinfo;
  
  if (gmtime_r(&now, &timeinfo) != nullptr && timeinfo.tm_year > (2020 - 1900)) {
    // Formato: " DD/MM/AAAA  HH:MM:SS "
    snprintf(scrollMessage, sizeof(scrollMessage), "  %02d/%02d/%04d  %02d:%02d:%02d  ",
             timeinfo.tm_mday,
             timeinfo.tm_mon + 1,
             timeinfo.tm_year + 1900,
             timeinfo.tm_hour,
             timeinfo.tm_min,
             timeinfo.tm_sec);
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n--- Iniciando D1 Mini Matriz TM1640 ---");

  // Configuracao da Matriz
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(0); // Ajustar rotacao se necessario de acordo com a orientacao fisica
  matrix.clear();
  matrix.writeDisplay();

  // Conectar ao Wi-Fi
  Serial.printf("Conectando a %s", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar no WiFi. Continuando com relogio interno...");
  }

  // Configura sincronizacao de tempo com servidores NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  // Aguarda sincronizacao inicial
  Serial.println("Sincronizando hora via NTP...");
  time_t now = time(nullptr);
  int ntpRetry = 0;
  while (now < 1600000000 && ntpRetry < 20) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
    ntpRetry++;
  }
  Serial.println();

  updateTimeMessage();
  Serial.print("Mensagem inicial: ");
  Serial.println(scrollMessage);
  
  scrollX = 8;
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastScrollTime >= scrollSpeedMs) {
    lastScrollTime = currentMillis;

    matrix.clear();
    matrix.setCursor(scrollX, 0);
    matrix.print(scrollMessage);
    matrix.writeDisplay();

    scrollX--;

    // Comprimento da string em pixels (cada caractere Adafruit padrao possui 6 pixels: 5x7 + 1 espaco)
    int messageWidth = strlen(scrollMessage) * 6;

    // Se a mensagem terminou de rolar completamente para a esquerda
    if (scrollX < -messageWidth) {
      scrollX = 8;
      updateTimeMessage(); // Atualiza a hora para o proximo ciclo de rolagem
      Serial.print("Nova mensagem atualizada: ");
      Serial.println(scrollMessage);
    }
  }

  yield(); // Mantém background tasks do ESP8266 ativas
}
