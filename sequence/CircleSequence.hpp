#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include <eeros/sequencer/Wait.hpp>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"
#include "conditions/BlueButtonCondition.hpp"
#include "conditions/MoveMouseCondition.hpp"


using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
  
class CircleSequence : public Sequence {
 public:
  CircleSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties);
  int action();
  void resetMousePos();
  
 private:
  Wait wait;
  Move move;
  BlueButtonExceptionSequence blueButtonExceptionSequence;
  BlueButtonCondition blueButtonCondition;
  Monitor blueButtonMonitor;
  MouseExceptionSequence mouseExceptionSequence;
  MoveMouseCondition moveMouseCondition;
  Monitor moveMouseMonitor;

  DeltaControlSystem& controlSys;
  DeltaSafetyProperties& safetyProp;
  SafetySystem& safetySys;
};

}
}

