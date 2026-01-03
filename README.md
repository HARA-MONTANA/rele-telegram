# rele-telegram

Bot de Telegram en C++ (Arduino/ESP32) para encender/apagar un relé conectado al pin GPIO 27.

## Requisitos
- Placa con WiFi y pin GPIO 27 (ej. ESP32).
- Librerías Arduino: `WiFi`, `WiFiClientSecure` y [`UniversalTelegramBot`](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot).
- Token del bot generado con `@BotFather`.

## Configuración rápida
1. Copia `bot_rele.ino` a tu sketch de Arduino o abre la carpeta del repositorio en el IDE Arduino / PlatformIO.
2. Completa tus credenciales en la parte superior del archivo:
   ```cpp
   const char* WIFI_SSID = "TU_SSID";
   const char* WIFI_PASSWORD = "TU_PASSWORD";
   const char* BOT_TOKEN = "TU_BOT_TOKEN";
   const char* ALLOWED_CHAT_ID = "123456789"; // opcional
   ```
3. Conecta el módulo de relé al GPIO 27 y al GND de la placa.
4. Compila y sube el firmware a tu placa.

## Uso
- Envía `/start` para ver los comandos disponibles.
- Envía `/on` o `/off` para energizar o desactivar el relé.
- Envía `/status` para consultar el estado actual.

> El cliente TLS se configura con `setInsecure()` para simplificar las pruebas; considera cargar el certificado de Telegram en entornos de producción.
