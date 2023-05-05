#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../Calibration.hpp"
#include "step/Move.hpp"
#include "step/Touch.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
  
class DetectSequence : public Sequence {
 public:
  DetectSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, Calibration& cal);
  int operator() (int pos);
  int action();
  
 private:
  DeltaControlSystem& controlSys;
  Calibration& calibration;
  Move move;
  Touch touch;
  Wait wait;
  int position;
};

}
}

