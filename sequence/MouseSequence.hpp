#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "step/Move.hpp"
#include "ExceptionSequence.hpp"
#include "conditions/BlueButtonCondition.hpp"
#include "conditions/EmergencyCondition.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
class MouseSequence : public Sequence {
 public:
  MouseSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp);
  int action();

 private:
  DeltaControlSystem& controlSys;
  DeltaSafetyProperties& safetyProp;
  SafetySystem& safetySys;
  AxisVector mousePosPrev;
  Wait wait;
  MouseTimeOutExceptionSequence mouseTimeoutSequence;
  BlueButtonExceptionSequence blueButtonExceptionSequence;
  BlueButtonCondition blueButtonCondition;
  Monitor blueButtonMonitor;
};

}
}
