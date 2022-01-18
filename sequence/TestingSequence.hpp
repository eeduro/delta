#pragma once

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include <eeros/sequencer/Wait.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
  
class Check : public Sequence {
 public:
  Check(std::string name, Sequence* caller, DeltaControlSystem& cs);
  int operator() (AxisVector position);
  int action();
  
 private:
  Wait wait;
  Move move;
  AxisVector position;
  eeros::hal::Output<bool>* ledGreen;
};

class TestingSequence : public Sequence {
 public:
  TestingSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp);
  int operator() (bool broadSide);
  int action();
  void resetMousePos();
  
 private:
  Wait wait;
  Move move;
  Check check;
  DeltaControlSystem& cs;
  DeltaSafetyProperties& sp;
  SafetySystem& ss;
  eeros::hal::Output<bool>* ledRed;
  bool broadSide;
};

}
}

