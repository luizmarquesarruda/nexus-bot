#include "nexus/safety/SafetyController.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

using nexus::safety::Controller;
using nexus::safety::Inputs;
using nexus::safety::State;

namespace {

int failures = 0;

void expect(bool condition, const std::string& name) {
  if (!condition) {
    ++failures;
    std::cerr << "FAIL: " << name << '\n';
  } else {
    std::cout << "PASS: " << name << '\n';
  }
}

Inputs healthyInputs(bool arm = false, bool reset = false) {
  Inputs inputs;
  inputs.emergencyCircuitHealthy = true;
  inputs.driverFeedbackHealthy = true;
  inputs.criticalSensorsHealthy = true;
  inputs.armRequested = arm;
  inputs.resetRequested = reset;
  return inputs;
}

void testMissingHardwareFailsClosed() {
  Controller controller;
  controller.update(Inputs{});
  expect(controller.state() == State::EmergencyStopLatched,
         "unconfigured interlock blocks startup");
  expect(!controller.motionPermitted(), "unconfigured hardware never permits motion");
}

void testSafeBootAndExplicitArm() {
  Controller controller;
  controller.update(healthyInputs());
  expect(controller.state() == State::Safe, "healthy startup enters SAFE");
  expect(!controller.motionPermitted(), "SAFE does not permit motion");

  controller.update(healthyInputs(true));
  expect(controller.state() == State::Armed, "explicit request enters ARMED");
  expect(controller.motionPermitted(), "ARMED permits logical motion requests");
}

void testEmergencyStopIsLatched() {
  Controller controller;
  controller.update(healthyInputs(true));

  Inputs emergency = healthyInputs(true);
  emergency.emergencyCircuitHealthy = false;
  controller.update(emergency);
  expect(controller.state() == State::EmergencyStopLatched,
         "emergency input has priority over arm request");
  expect(!controller.motionPermitted(), "emergency state blocks motion");

  controller.update(healthyInputs());
  expect(controller.state() == State::EmergencyStopLatched,
         "restoring circuit alone does not clear emergency latch");

  controller.update(healthyInputs(true, true));
  expect(controller.state() == State::EmergencyStopLatched,
         "reset while arm is requested is rejected");

  controller.update(healthyInputs(false, true));
  expect(controller.state() == State::Safe,
         "explicit reset with arm released returns to SAFE");
  expect(!controller.motionPermitted(), "reset never starts movement");
}

void testCriticalFailureIsLatched() {
  Controller controller;
  controller.update(healthyInputs(true));

  Inputs failedSensor = healthyInputs(true);
  failedSensor.criticalSensorsHealthy = false;
  controller.update(failedSensor);
  expect(controller.state() == State::FaultLatched,
         "critical sensor failure latches a fault");
  expect(!controller.motionPermitted(), "fault state blocks motion");

  controller.update(healthyInputs(false, true));
  expect(controller.state() == State::Safe,
         "healthy checks plus explicit reset clear fault to SAFE");
}

}  // namespace

int main() {
  testMissingHardwareFailsClosed();
  testSafeBootAndExplicitArm();
  testEmergencyStopIsLatched();
  testCriticalFailureIsLatched();

  if (failures != 0) {
    std::cerr << failures << " test(s) failed\n";
    return EXIT_FAILURE;
  }
  std::cout << "All safety-controller tests passed.\n";
  return EXIT_SUCCESS;
}
