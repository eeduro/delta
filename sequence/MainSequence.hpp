#pragma once

#include <array>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "TestingSequence.hpp"
#include "HomingSequence.hpp"
#include "ExceptionSequence.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
  
class MainSequence : public Sequence {
 public:
  MainSequence(std::string name, Sequencer& seq, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp);
  int action();
  
 private:
  DeltaControlSystem& cs;
  SafetySystem& ss;
  DeltaSafetyProperties& sp;
  HomingSequence homingSeq;
  TestingSequence testingSeq;
  Wait wait;
  
  EmergencyExceptionSequence emergencyExceptionSeq;
  EmergencyCondition emergencyCondition;
  Monitor emergencyMonitor;
  
  eeros::hal::Input<bool>* buttonGreen;
  eeros::hal::Input<bool>* buttonBlue;
  eeros::hal::Output<bool>* ledGreen;
  eeros::hal::Output<bool>* ledBlue;
  uint8_t blueButtonCounter = 0;
};

}
}

