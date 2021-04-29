#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "SortSequence.hpp"
#include "ShuffleSequence.hpp"
#include "ExceptionSequence.hpp"
#include "conditions/BlueButtonCondition.hpp"
#include "conditions/EmergencyCondition.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
  
class AutoMoveSequence : public Sequence {
 public:
  AutoMoveSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp, Calibration& cal);
  int action();
  
 private:
  SortSequence sortSeq;
  ShuffleSequence shuffSeq;
  Wait wait;
  MouseExceptionSequence mouseExceptionSeq;
  MoveMouseCondition moveMouseCondition;
  Monitor moveMouseMonitor;
  BlueButtonExceptionSequence blueButtonExceptionSeq;
  BlueButtonCondition blueButtonCondition;
  Monitor blueButtonMonitor;
  
  DeltaControlSystem& controlSys;
  SafetySystem& safetySys;
  DeltaSafetyProperties& safetyProp;
};

}
}

