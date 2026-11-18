#include <Arduino.h>
#include <ArduinoJson.h>

// Pinagem UART2 é provisória para ESP32 DevKit clássico.
// Confirmar conflitos com a placa/periféricos antes da montagem.
#ifndef NEXUS_UART_BAUD
#define NEXUS_UART_BAUD 115200
#endif
#ifndef NEXUS_UART_RX_PIN
#define NEXUS_UART_RX_PIN 16
#endif
#ifndef NEXUS_UART_TX_PIN
#define NEXUS_UART_TX_PIN 17
#endif

// O driver/motores ainda não foram escolhidos. Por segurança, a saída física
// permanece desabilitada até existir pinagem e driver aprovados.
#ifndef NEXUS_MOTOR_OUTPUT_ENABLED
#define NEXUS_MOTOR_OUTPUT_ENABLED 0
#endif

static constexpr uint32_t COMMAND_TIMEOUT_MS = 350;
static constexpr size_t MAX_LINE_LENGTH = 256;
static constexpr int BUZZER_PIN = 25; // Provisório; validar placa e circuito.
static constexpr int LED_PIN = 2;     // LED onboard comum em algumas DevKits.

HardwareSerial &linkSerial = Serial2;
String inputLine;
uint32_t lastDriveCommandMs = 0;
bool movingRequested = false;

void stopMotors() {
  movingRequested = false;
#if NEXUS_MOTOR_OUTPUT_ENABLED
  // TODO: implementar somente após selecionar driver e validar pinagem.
#endif
}

void setMotorSpeeds(float left, float right) {
  // Limites locais independem da Orange Pi.
  left = constrain(left, -1.0f, 1.0f);
  right = constrain(right, -1.0f, 1.0f);
#if NEXUS_MOTOR_OUTPUT_ENABLED
  // TODO: aplicar PWM/direção ao driver definido.
#else
  (void)left;
  (void)right;
#endif
  movingRequested = (left != 0.0f || right != 0.0f);
  lastDriveCommandMs = millis();
}

void beepPattern(const char *pattern) {
  if (strcmp(pattern, "ack") == 0) {
    tone(BUZZER_PIN, 1400, 70);
  } else if (strcmp(pattern, "thinking") == 0) {
    tone(BUZZER_PIN, 900, 110);
  } else if (strcmp(pattern, "error") == 0) {
    tone(BUZZER_PIN, 420, 220);
  } else if (strcmp(pattern, "obstacle") == 0) {
    tone(BUZZER_PIN, 650, 90);
  } else if (strcmp(pattern, "shutdown") == 0) {
    tone(BUZZER_PIN, 1100, 80);
  }
}

void sendMessage(JsonDocument &doc) {
  serializeJson(doc, linkSerial);
  linkSerial.println();
}

void sendAck(uint32_t seq, const char *status) {
  JsonDocument response;
  response["v"] = 1;
  response["seq"] = seq;
  response["type"] = "ack";
  response["status"] = status;
  sendMessage(response);
}

void handleLine(const String &line) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, line);
  if (error || !doc["v"].is<int>() || doc["v"].as<int>() != 1 ||
      !doc["seq"].is<uint32_t>() || !doc["type"].is<const char *>()) {
    JsonDocument fault;
    fault["v"] = 1;
    fault["type"] = "fault";
    fault["reason"] = "invalid_frame";
    sendMessage(fault);
    return;
  }

  const uint32_t seq = doc["seq"].as<uint32_t>();
  const char *type = doc["type"];
  const char *name = doc["name"] | "";

  if (strcmp(type, "cmd") != 0) {
    sendAck(seq, "unsupported_type");
    return;
  }

  // STOP sempre é aceito antes de qualquer outra validação de movimento.
  if (strcmp(name, "stop") == 0) {
    stopMotors();
    sendAck(seq, "stopped");
    return;
  }

  if (strcmp(name, "drive") == 0) {
    if (!doc["left"].is<float>() || !doc["right"].is<float>() ||
        !doc["ttl_ms"].is<uint32_t>()) {
      sendAck(seq, "invalid_drive");
      return;
    }
    const uint32_t ttl = doc["ttl_ms"].as<uint32_t>();
    if (ttl == 0 || ttl > COMMAND_TIMEOUT_MS) {
      stopMotors();
      sendAck(seq, "ttl_out_of_range");
      return;
    }
    setMotorSpeeds(doc["left"].as<float>(), doc["right"].as<float>());
    sendAck(seq, "accepted");
    return;
  }

  if (strcmp(name, "beep_pattern") == 0) {
    const char *pattern = doc["pattern"] | "";
    beepPattern(pattern);
    sendAck(seq, "accepted");
    return;
  }

  if (strcmp(name, "set_led") == 0) {
    const bool enabled = doc["enabled"] | false;
    digitalWrite(LED_PIN, enabled ? HIGH : LOW);
    sendAck(seq, "accepted");
    return;
  }

  sendAck(seq, "unknown_command");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  stopMotors();
  Serial.begin(115200);
  linkSerial.begin(NEXUS_UART_BAUD, SERIAL_8N1, NEXUS_UART_RX_PIN, NEXUS_UART_TX_PIN);
  inputLine.reserve(MAX_LINE_LENGTH);
  lastDriveCommandMs = millis();
}

void loop() {
  while (linkSerial.available() > 0) {
    const char c = static_cast<char>(linkSerial.read());
    if (c == '\n') {
      if (inputLine.length() > 0) handleLine(inputLine);
      inputLine = "";
    } else if (c != '\r') {
      if (inputLine.length() >= MAX_LINE_LENGTH) {
        inputLine = "";
        stopMotors();
        JsonDocument fault;
        fault["v"] = 1;
        fault["type"] = "fault";
        fault["reason"] = "frame_too_large";
        sendMessage(fault);
      } else {
        inputLine += c;
      }
    }
  }

  if (movingRequested && (millis() - lastDriveCommandMs > COMMAND_TIMEOUT_MS)) {
    stopMotors();
    JsonDocument fault;
    fault["v"] = 1;
    fault["type"] = "fault";
    fault["reason"] = "command_timeout";
    sendMessage(fault);
  }
}
