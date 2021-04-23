#pragma once

#include <vector>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "step/Move.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
  
class ParkSequence : public Sequence{
 public:
  ParkSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp);
  int action();

 private:
  DeltaControlSystem& controlSys;
  SafetySystem& safetySys;
  DeltaSafetyProperties& safetyProp;
  Move move;
};

}
}
