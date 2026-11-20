#include <Arduino.h>

#include "nexus/safety/SafetyController.hpp"

namespace {

constexpr uint32_t kStatusIntervalMs = 1000;
constexpr uint32_t kSafetyEvaluationIntervalMs = 20;

nexus::safety::Controller safetyController;
uint32_t lastStatusAtMs = 0;
uint32_t lastSafetyEvaluationAtMs = 0;

nexus::safety::Inputs readSafetyInputs() {
  // Hardware inputs are intentionally not mapped yet. Until the exact board,
  // emergency circuit, driver and critical sensors are selected and tested,
  // every health flag remains false and the controller fails closed.
  return {};
}

}  // namespace

void setup() {
  Serial.begin(115200);
  safetyController.update(readSafetyInputs());

  Serial.println();
  Serial.println("NEXUS BOT - ESP32 principal");
  Serial.println("Firmware foundation only; traction outputs are not implemented.");
  Serial.println("Hardware interlock, board pins and sensors: NOT VALIDATED.");
  Serial.println(safetyController.stateName());
}

void loop() {
  const uint32_t now = millis();

  if (now - lastSafetyEvaluationAtMs >= kSafetyEvaluationIntervalMs) {
    lastSafetyEvaluationAtMs = now;
    safetyController.update(readSafetyInputs());
  }

  if (now - lastStatusAtMs >= kStatusIntervalMs) {
    lastStatusAtMs = now;
    Serial.print("safety_state=");
    Serial.print(safetyController.stateName());
    Serial.print(" motion_permitted=");
    Serial.println(safetyController.motionPermitted() ? "true" : "false");
  }
}
