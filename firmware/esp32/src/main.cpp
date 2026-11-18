#include <Arduino.h>
#include <ArduinoJson.h>
#include <math.h>

// Pinagem UART2 provisória para ESP32 DevKit clássico.
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

// Driver/motores ainda não foram escolhidos. A saída física fica desabilitada.
#ifndef NEXUS_MOTOR_OUTPUT_ENABLED
#define NEXUS_MOTOR_OUTPUT_ENABLED 0
#endif

static constexpr uint32_t MAX_COMMAND_TTL_MS = 350;
static constexpr size_t MAX_LINE_LENGTH = 256;
static constexpr int BUZZER_PIN = 25; // Provisório; validar placa e circuito.
static constexpr int LED_PIN = 2;     // Presente em algumas DevKits, não em todas.

HardwareSerial &linkSerial = Serial2;
String inputLine;
uint32_t driveDeadlineMs = 0;
bool movingRequested = false;

void stopMotors() {
  movingRequested = false;
#if NEXUS_MOTOR_OUTPUT_ENABLED
  // TODO: desabilitar fisicamente o driver após seleção e validação do hardware.
#endif
}

bool validSpeed(float value) {
  return isfinite(value) && value >= -1.0f && value <= 1.0f;
}

void setMotorSpeeds(float left, float right, uint32_t ttlMs) {
#if NEXUS_MOTOR_OUTPUT_ENABLED
  // TODO: aplicar PWM/direção ao driver definido.
#else
  // A V1 inicial compila sem acionar motores: driver e pinagem não aprovados.
  (void)left;
  (void)right;
#endif
  movingRequested = (left != 0.0f || right != 0.0f);
  driveDeadlineMs = millis() + ttlMs;
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

void sendFault(const char *reason) {
  JsonDocument fault;
  fault["v"] = 1;
  fault["type"] = "fault";
  fault["reason"] = reason;
  sendMessage(fault);
}

void handleLine(const String &line) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, line);
  if (error || !doc["v"].is<int>() || doc["v"].as<int>() != 1 ||
      !doc["seq"].is<uint32_t>() || !doc["type"].is<const char *>()) {
    sendFault("invalid_frame");
    return;
  }

  const uint32_t seq = doc["seq"].as<uint32_t>();
  const char *type = doc["type"];
  const char *name = doc["name"] | "";

  if (strcmp(type, "cmd") != 0) {
    sendAck(seq, "unsupported_type");
    return;
  }

  // STOP é tratado antes dos demais comandos e sempre resulta em saída parada.
  if (strcmp(name, "stop") == 0) {
    stopMotors();
    sendAck(seq, "stopped");
    return;
  }

  if (strcmp(name, "drive") == 0) {
    if (!doc["left"].is<float>() || !doc["right"].is<float>() ||
        !doc["ttl_ms"].is<uint32_t>()) {
      stopMotors();
      sendAck(seq, "invalid_drive");
      return;
    }

    const float left = doc["left"].as<float>();
    const float right = doc["right"].as<float>();
    const uint32_t ttl = doc["ttl_ms"].as<uint32_t>();
    if (!validSpeed(left) || !validSpeed(right) || ttl == 0 || ttl > MAX_COMMAND_TTL_MS) {
      stopMotors();
      sendAck(seq, "drive_out_of_range");
      return;
    }

    setMotorSpeeds(left, right, ttl);
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
        sendFault("frame_too_large");
      } else {
        inputLine += c;
      }
    }
  }

  // TTL é aplicado localmente pelo ESP32, sem depender da Orange Pi.
  if (movingRequested && static_cast<int32_t>(millis() - driveDeadlineMs) >= 0) {
    stopMotors();
    sendFault("command_timeout");
  }
}
