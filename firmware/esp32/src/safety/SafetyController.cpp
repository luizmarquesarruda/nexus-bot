#include "nexus/safety/SafetyController.hpp"

namespace nexus::safety {
namespace {

bool allHealthChecksPass(const Inputs& inputs) noexcept {
  return inputs.emergencyCircuitHealthy &&
         inputs.driverFeedbackHealthy &&
         inputs.criticalSensorsHealthy;
}

}  // namespace

void Controller::update(const Inputs& inputs) noexcept {
  lastInputsHealthy_ = allHealthChecksPass(inputs);

  // The hardware interlock must cut traction independently of this firmware.
  if (!inputs.emergencyCircuitHealthy) {
    state_ = State::EmergencyStopLatched;
    return;
  }

  // A reset returns to SAFE only. The arm request must be released.
  if (state_ == State::EmergencyStopLatched || state_ == State::FaultLatched) {
    if (inputs.resetRequested && !inputs.armRequested && lastInputsHealthy_) {
      state_ = State::Safe;
    }
    return;
  }

  if (!inputs.driverFeedbackHealthy || !inputs.criticalSensorsHealthy) {
    state_ = State::FaultLatched;
    return;
  }

  if (state_ == State::Boot) {
    state_ = State::Safe;
  }
  state_ = inputs.armRequested ? State::Armed : State::Safe;
}

State Controller::state() const noexcept {
  return state_;
}

bool Controller::motionPermitted() const noexcept {
  return state_ == State::Armed && lastInputsHealthy_;
}

const char* Controller::stateName() const noexcept {
  switch (state_) {
    case State::Boot: return "BOOT";
    case State::Safe: return "SAFE";
    case State::Armed: return "ARMED";
    case State::EmergencyStopLatched: return "EMERGENCY_STOP_LATCHED";
    case State::FaultLatched: return "FAULT_LATCHED";
  }
  return "UNKNOWN";
}

}  // namespace nexus::safety
