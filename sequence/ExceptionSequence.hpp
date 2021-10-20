#pragma once

#include <vector>

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>

#include "../control/DeltaControlSystem.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "step/Move.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
class MouseExceptionSequence : public Sequence {
 public:
  MouseExceptionSequence(std::string name, Sequence* caller, SafetySystem& ss, DeltaSafetyProperties& sp)
      : Sequence(name, caller, true), safetySys(ss), safetyProp(sp) { }
  
  int action(){
    safetySys.triggerEvent(safetyProp.doMouseControl);
    return(0);
  }

 private:
  SafetySystem& safetySys;
  DeltaSafetyProperties& safetyProp;
};
  
class MouseTimeOutExceptionSequence : public Sequence {
 public:
  MouseTimeOutExceptionSequence(std::string name, Sequence* caller, SafetySystem& ss, DeltaSafetyProperties& sp) 
      : Sequence(name, caller, true), safetySys(ss), safetyProp(sp) { }
  
  int action(){
    safetySys.triggerEvent(safetyProp.doAutoMoving);
    return(0);
  }
  
 private:
  SafetySystem& safetySys;
  DeltaSafetyProperties& safetyProp;
};

class BlueButtonExceptionSequence : public Sequence {
 public:
  BlueButtonExceptionSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp)
      : Sequence(name, caller, true),
        safetySys(ss),
        safetyProp(sp),
        controlSys(cs),
        move("Move", this, cs) { }
  
  int action(){
    controlSys.pathPlanner.setStart(controlSys.directKin.getOut().getSignal().getValue());
    controlSys.setPathPlannerInput();
    move({0, 0, tcpReady_z, 0});
    safetySys.triggerEvent(safetyProp.stopMoving);
    return(0);
  }

 private:
  SafetySystem& safetySys;
  DeltaSafetyProperties& safetyProp;
  DeltaControlSystem& controlSys;
  Move move;
};

class EmergencyExceptionSequence : public Sequence {
 public:
  EmergencyExceptionSequence(std::string name, Sequence* caller, DeltaControlSystem& cs, SafetySystem& ss, DeltaSafetyProperties& sp) 
      : Sequence(name, caller, true),
        controlSys(cs),
        safetySys(ss),
        safetyProp(sp) { }
    
  int action(){
    controlSys.voltageSetPoint.setValue({0, 0, 0, 0});
    controlSys.voltageSwitch.switchToInput(1);
    HAL::instance().getLogicOutput("ledBlue", false)->set(false);
    while ((Sequencer::running) && !HAL::instance().getLogicInput("buttonGreen", false)->get());
    safetySys.triggerEvent(safetyProp.doControlStart);
    return(0);
  }

 private:
  SafetySystem& safetySys;
  DeltaSafetyProperties& safetyProp;
  DeltaControlSystem& controlSys;
};

}
}
