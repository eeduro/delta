#pragma once

#include <vector>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "MouseSequence.hpp"
#include "../Calibration.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace eeros::hal;

namespace eeduro {
namespace delta {
  
class CalibrateSequence : public Sequence {
 public:
  CalibrateSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, Calibration& cal);
  virtual int action();	
  
 private:
  Calibration& calibration;
  DeltaControlSystem& controlSys;
  Wait wait;
  eeros::hal::Input<bool>* buttonBlue;
  eeros::hal::Output<bool>* ledBlue;
  eeros::hal::Output<bool>* ledGreen;
};

}
}
