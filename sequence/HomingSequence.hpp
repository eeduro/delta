#pragma once

#include <vector>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "conditions/EmergencyCondition.hpp"
#include "step/Move.hpp"
#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
  
class HomingSequence : public Sequence {
 public:
  HomingSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp);
  int action();

 private:
  DeltaControlSystem& controlSys;
  SafetySystem& safetySys;
  DeltaSafetyProperties& safetyProp;
  Move move;
  Wait wait;
};

}
}
