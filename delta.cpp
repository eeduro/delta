#include <iostream>
#include <ostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <eeros/hal/HAL.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/sequencer/Sequencer.hpp>

#include "control/DeltaControlSystem.hpp"
#include "safety/DeltaSafetyProperties.hpp"
#include "sequence/MainSequence.hpp"
#include "Calibration.hpp"

#include <eeros/task/Lambda.hpp>

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::control;
using namespace eeros::safety;
using namespace eeros::logger;
using namespace eeros::sequencer;
using namespace eeduro::delta;

void signalHandler(int signum){
  SafetySystem::exitHandler();
  Sequencer::instance().abort();
}

int main(int argc, char **argv) {

  Logger::setDefaultStreamLogger(std::cout);
  Logger log = Logger::getLogger();
  
  log.info() << "Start Delta application";
  
  log.info() << "Initializing hardware";
  HAL& hal = HAL::instance();
  hal.readConfigFromFile(&argc, argv);
  
  signal(SIGINT, signalHandler);
  
  Calibration calibration{};
  calibration.loadDefaults();
  if (!calibration.load()) {
    log.warn() << "could not load calibration";
    return 0;
  }

  // Create the control system
  DeltaControlSystem controlSys;
  
  // Create and initialize a safety system
  DeltaSafetyProperties safetyProp(controlSys);
  SafetySystem safetySys(safetyProp, dt);
  controlSys.timedomain.registerSafetyEvent(safetySys, safetyProp.doEmergency);
  
  auto& seq = Sequencer::instance();
  MainSequence mainSequence("Main", seq, controlSys, safetySys, safetyProp, calibration);
  mainSequence();
  
  auto &executor = Executor::instance();
  executor.setMainTask(safetySys);


  eeros::task::Lambda l1 ([&] () { });
  eeros::task::Periodic perLog("periodic log", 1, l1);
  perLog.monitors.push_back([&](PeriodicCounter &pc, Logger &log){
//    log.info() << controlSys.pathPlanner.getPosOut().getSignal();
    log.info() << controlSys.pathPlanner.getPosOut().getSignal().getValue() << ", " << controlSys.directKin.getOut().getSignal().getValue();
  });

  // and further down the code
  executor.add(perLog);


  executor.run();

  seq.wait();
  
  log.info() << "Delta application finished";
  return 0;
}
