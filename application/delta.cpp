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
#include <eeros/sequencer/Monitor.hpp>

#include "control/DeltaControlSystem.hpp"
#include "safety/DeltaSafetyProperties.hpp"

#include "sequence/ConfigureBlockSequence.hpp"
#include "sequence/ShuffleSequence.hpp"
#include "sequence/SortSequence.hpp"
#include "sequence/MouseSequence.hpp"
#include "sequence/AutoMoveSequence.hpp"
#include "sequence/ExceptionSequence.hpp"
#include "sequence/HomingSequence.hpp"
#include "sequence/ParkSequence.hpp"

#include "conditions/MoveMouseCondition.hpp"

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
	signal(SIGINT, signalHandler);
	
	StreamLogWriter w(std::cout);
	Logger::setDefaultWriter(&w);
	Logger log;
	w.show();
	
	log.info() << "delta test started...";
	
	log.info() << "Initializing Hardware...";
	HAL& hal = HAL::instance();
	hal.readConfigFromFile(&argc, argv);
	
	// Create the control system
	DeltaControlSystem controlSys{};
	
	// Create and initialize a safety system
	DeltaSafetyProperties properties{controlSys};
	SafetySystem safetySys{properties, dt};
	controlSys.timedomain.registerSafetyEvent(safetySys, properties.doEmergency);
	
	
	Calibration calibration{};
	calibration.loadDefaults();
	if (!calibration.load()) {
		log.warn() << "could not load calibration";
	}
	
	auto& sequencer = Sequencer::instance();

	HomingSequence homingSequence{"Homing Sequence", sequencer, controlSys, safetySys, properties, calibration};
	AutoMoveSequence autoMoveSequence{"AutoMove Sequence", sequencer, controlSys, safetySys, properties, calibration};
	MouseSequence mouseSequence{"Mouse Sequence", sequencer, controlSys, safetySys, properties, calibration};
	ConfigureBlockSequence calibSequence{"Configure block Sequence", sequencer, controlSys, safetySys, calibration};
	ParkSequence parkSequence{"Park Sequence", sequencer, controlSys, safetySys, properties, calibration};
	

	sequencer.addSequence(homingSequence);
	sequencer.addSequence(autoMoveSequence);
	sequencer.addSequence(mouseSequence);
	sequencer.addSequence(calibSequence);
	sequencer.addSequence(parkSequence);
	
	auto &executor = Executor::instance();
	executor.setMainTask(safetySys);
	
	executor.run();
	
	log.info() << "Example finished...";
	return 0;
}