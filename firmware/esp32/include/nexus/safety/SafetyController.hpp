#pragma once

namespace nexus::safety {

enum class State {
  Boot,
  Safe,
  Armed,
  EmergencyStopLatched,
  FaultLatched,
};

// Missing or unconfigured hardware fails closed. "Healthy" describes a
// monitored circuit; it does not replace an independent physical stop circuit.
struct Inputs {
  bool emergencyCircuitHealthy{false};
  bool driverFeedbackHealthy{false};
  bool criticalSensorsHealthy{false};
  bool armRequested{false};
  bool resetRequested{false};
};

class Controller {
 public:
  void update(const Inputs& inputs) noexcept;
  State state() const noexcept;
  bool motionPermitted() const noexcept;
  const char* stateName() const noexcept;

 private:
  State state_{State::Boot};
  bool lastInputsHealthy_{false};
};

}  // namespace nexus::safety
