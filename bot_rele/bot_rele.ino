#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Configuración de red
const char* WIFI_SSID = "TU_SSID";
const char* WIFI_PASSWORD = "TU_PASSWORD";

// Token del Bot de Telegram (de @BotFather)
const char* BOT_TOKEN = "TU_BOT_TOKEN";
// Opcional: limita el control a un chat específico para mayor seguridad
const char* ALLOWED_CHAT_ID = "";  // Deja vacío para permitir cualquier chat

constexpr uint8_t RELAY_PIN = 27;
constexpr unsigned long BOT_POLL_INTERVAL = 500;  // ms - respuesta más rápida

WiFiClientSecure secureClient;
UniversalTelegramBot bot(BOT_TOKEN, secureClient);
unsigned long lastPollTime = 0;
int lastUpdateId = 0;

void connectWifi() {
  WiFi.mode(WIFI_STA);
  Serial.println("Conectando a WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.print("WiFi conectado, IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relé apagado (activo en LOW)

  Serial.begin(115200);
  connectWifi();

  // Deshabilitamos la verificación de certificado para simplificar
  secureClient.setInsecure();
}

void sendStatus(const String& chatId) {
  const bool relayOn = digitalRead(RELAY_PIN) == LOW;  // Activo en LOW
  String message = relayOn ? "Relé: ENCENDIDO" : "Relé: APAGADO";
  bot.sendMessage(chatId, message, "Markdown");
}

bool isChatAllowed(const String& chatId) {
  if (strlen(ALLOWED_CHAT_ID) == 0) {
    return true;
  }
  return chatId == ALLOWED_CHAT_ID;
}

void handleMessage(const telegramMessage& msg) {
  Serial.print("Comando recibido (chat ");
  Serial.print(msg.chat_id);
  Serial.print("): ");
  Serial.println(msg.text);

  if (!isChatAllowed(msg.chat_id)) {
    bot.sendMessage(msg.chat_id, "Acceso no autorizado", "Markdown");
    return;
  }

  if (msg.text == "/on" || msg.text == "on") {
    digitalWrite(RELAY_PIN, LOW);  // Activa el relé (LOW)
    Serial.println("Relé ENCENDIDO (LOW)");
    bot.sendMessage(msg.chat_id, "Relé encendido", "Markdown");
  } else if (msg.text == "/off" || msg.text == "off") {
    digitalWrite(RELAY_PIN, HIGH);  // Desactiva el relé (HIGH)
    Serial.println("Relé APAGADO (HIGH)");
    bot.sendMessage(msg.chat_id, "Relé apagado", "Markdown");
  } else if (msg.text == "/status" || msg.text == "status") {
    sendStatus(msg.chat_id);
  } else if (msg.text == "/start") {
    bot.sendMessage(msg.chat_id,
                   "Comandos:\n/on – Enciende el relé\n/off – Apaga el relé\n/status – Estado actual", "Markdown");
  } else {
    bot.sendMessage(msg.chat_id, "Comando no reconocido. Usa /on, /off o /status", "Markdown");
  }
}

void pollBot() {
  int numNewMessages = bot.getUpdates(lastUpdateId + 1);

  while (numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
      lastUpdateId = bot.messages[i].update_id;
      handleMessage(bot.messages[i]);
    }
    numNewMessages = bot.getUpdates(lastUpdateId + 1);
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  if (millis() - lastPollTime > BOT_POLL_INTERVAL) {
    pollBot();
    lastPollTime = millis();
  }
}
