#include <iostream>
#include <functional>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>

#include <eeros/logger/Logger.hpp>
#include <eeros/logger/LogWriter.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/core/Version.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/task/Periodic.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Sequencer.hpp>

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;
using namespace eeros::sequencer;
namespace {
	using Logger = eeros::logger::Logger;
}

class ButtonTestSequence : public Sequence {
public:
	ButtonTestSequence(std::string name, Sequencer& seq): 
	  Sequence(name, seq)
	{
	  HAL &hal = eeros::hal::HAL::instance();
	  buttonBlue =  hal.getLogicInput("buttonBlue", false);
	  ledBlue = hal.getLogicOutput("ledBlue", false);
	  buttonRed =  hal.getLogicInput("buttonRed", false);
	  ledRed = hal.getLogicOutput("ledRed", false);
	  buttonGreen =  hal.getLogicInput("buttonGreen", false);
	  ledGreen = hal.getLogicOutput("onBoardRed", false);
	  
	}
	
	
	int action(){
	  int count = 0;
	 
	  while(Sequencer::running){
	    if(buttonBlue->get()) log.info() << "blue button pressed";
	    if(buttonGreen->get()) log.trace() << "green button pressed";
	    if(buttonRed->get()) log.error() << "red button pressed";
	    
	    if(count < 500) {
	      ledGreen->set(true);
	      ledBlue->set(false);
	      ledRed->set(false);
	    }else if(count >= 500 && count < 1000){
	      ledGreen->set(false);
	      ledBlue->set(false);
	      ledRed->set(true);
	    }else if(count >= 1000 && count < 1500){
	      ledGreen->set(false);
	      ledBlue->set(true);
	      ledRed->set(false);
	    }else if(count >= 1500 && count < 2000){
	      ledGreen->set(true);
	      ledBlue->set(true);
	      ledRed->set(true);
	    }else if(count >= 2000 && count < 2500){
	      ledGreen->set(false);
	      ledBlue->set(false);
	      ledRed->set(false);
	    }else count = 0;
	    
	    count++;
	  }
	  
	}
	
	eeros::hal::Input<bool>* buttonBlue;
	eeros::hal::Output<bool>* ledBlue;
	eeros::hal::Input<bool>* buttonRed;
	eeros::hal::Output<bool>* ledRed;
	eeros::hal::Input<bool>* buttonGreen;
	eeros::hal::Output<bool>* ledGreen;
};

class SafetyPropertiesTest : public SafetyProperties {
public:
	SafetyPropertiesTest() : slOff("off") {	
		addLevel(slOff);
		setEntryLevel(slOff);
	}
	
	SafetyLevel slOff;
};

void signalHandler(int signum){
	SafetySystem::exitHandler();

	Sequencer::instance().abort();
}

int main(int argc, char *argv[]) {
	HAL& hal = HAL::instance();
	hal.readConfigFromFile(&argc, argv);
	
	const double dt = 0.001;
	eeros::logger::StreamLogWriter w(std::cout);
	Logger::setDefaultWriter(&w);
	w.show();

	Logger log('M');

	log.trace() << "button test started";
	log.trace() << "eeros " << eeros::Version::string;

	// Create and initialize safety system
	SafetyPropertiesTest ssProperties;
	SafetySystem safetySys(ssProperties, dt);
	
	auto& sequencer = Sequencer::instance();
	ButtonTestSequence bts("ButtonTestSequence", sequencer);
	sequencer.addSequence(bts);

	bts.start();

	auto executor = eeros::Executor::instance();;
	executor.setMainTask(safetySys);


	executor.run();
	sequencer.wait();
	
	log.info() << "Button test finished";

	return 0;
}