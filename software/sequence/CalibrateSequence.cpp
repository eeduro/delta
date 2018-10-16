#include "CalibrateSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include <unistd.h>

using namespace eeduro::delta;
using namespace eeros::sequencer;
using namespace eeros::safety;


CalibrateSequence::CalibrateSequence(std::string name, eeros::sequencer::Sequencer& sequencer, DeltaControlSystem& controlSys, eeros::safety::SafetySystem& safetySys, Calibration& calibration) :
	Sequence(name, sequencer),
	controlSys(controlSys),
	safetySys(safetySys),
	safetyProp(safetyProp),
	calibration(calibration)
{
  HAL& hal = HAL::instance();
  buttonBlue = hal.getLogicInput("buttonBlue");
  ledBlue = hal.getLogicOutput("ledBlue");
	
}


bool CalibrateSequence::getDone()
{
  return done;
}


void CalibrateSequence::waitUntilReady() {
	while(safetySys.getCurrentLevel() != safetyProp.slCalibrating) {
		usleep(100000);
		std::this_thread::yield();
	}
}

void CalibrateSequence::waitForButton(std::vector<int> buttons) {
	for (auto i: buttons) {
		if (i < 0 || i > 3)
		eeros::Fault("index out of range");
		  
	}
	usleep(200000);


	while(true){
	  switch(buttons[0]){
	    case 0:		//blue button
		ledBlue->set(true);
		
		if(buttonBlue->get()){
		  ledBlue->set(false);
		  return;
		}
	      break;
	    
	  }
	}
}

void CalibrateSequence::logAndWaitForButton(std::vector<int> buttons) {
	for (auto i: buttons) {
		if (i < 0 || i > 3)
		eeros::Fault("index out of range");
		  
	}
	usleep(200000);


	while(true){
	  log.info() << controlSys.directKin.getOut().getSignal().getValue();
	  switch(buttons[0]){
	    case 0:		//blue button
		ledBlue->set(true);
		
		if(buttonBlue->get()){
		  ledBlue->set(false);
		  return;
		}
	      break;
	    
	  }
	}
}

void CalibrateSequence::waitForGreenButton()	 { waitForButton({2}); }
void CalibrateSequence::waitForRedButton()		 { waitForButton({1}); }
void CalibrateSequence::waitForBlueButton()		 { waitForButton({0}); }
void CalibrateSequence::waitForBlueOrRedButton() { waitForButton({ 0, 1 }); }

int CalibrateSequence::action() {
	log.info() << "Start calibration";
	
	const char *block[] = { "[no block]", "[block 1]", "[block 2]", "[block 3]" };
	
	controlSys.start();
	usleep(500000);
	
	/*test for controlsys*/
	logAndWaitForButton({0});
	

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			log.trace() << block[i] << " move TCP to position " << j << " and press the blue button";
			waitForBlueButton();
			
			auto p = controlSys.directKin.getOut().getSignal().getValue();
			log.info() << "p: " << p;
			calibration.position[j].zblock[i] = p[2];
			if (i == 3) {
				calibration.position[j].x = p[0];
				calibration.position[j].y = p[1];
			}
		}
	}
	

	for (int i = 0; i < 4; i++) {
		calibration.position[i].level12 = (calibration.position[i].zblock[1] + calibration.position[i].zblock[2]) / 2.0;
		calibration.position[i].level23 = (calibration.position[i].zblock[2] + calibration.position[i].zblock[3]) / 2.0;
		calibration.position[i].level30 = (calibration.position[i].zblock[3] + calibration.position[i].zblock[0]) / 2.0;
	}

	bool good = true;
	for (int i = 0; i < 4; i++) {
		double z0 = calibration.position[i].zblock[0];
		double z1 = calibration.position[i].zblock[1];
		double z2 = calibration.position[i].zblock[2];
		double z3 = calibration.position[i].zblock[3];
		
		if (z0 >= z3) {
			log.error() << "[position " << i << "] invalid calibration zblock0 >= zblock3";
			log.warn() << "[position " << i << "] z0: " << z0 << ", z3: " << z3;
			good = false;
		}
		if (z3 >= z2) {
			log.error() << "[position " << i << "] invalid calibration zblock3 >= zblock2";
			log.warn() << "[position " << i << "] z3: " << z3 << ", z2: " << z2;
			good = false;
		}
		if (z2 >= z1) {
			log.error() << "[position " << i << "] invalid calibration zblock2 >= zblock1";
			log.warn() << "[position " << i << "] z2: " << z2 << ", z1: " << z1;
			good = false;
		}
		
		if (z1 <= calibration.position[i].level12) {
			log.error() << "[position " << i << "] invalid calibration zblock1 <= level12";
			log.warn() << "[position " << i << "] z1: " << z1 << ", level12: " << calibration.position[i].level12;
			good = false;
		}
		if (z2 >= calibration.position[i].level12) {
			log.error() << "[position " << i << "] invalid calibration zblock2 >= level12";
			good = false;
		}
		
		if (z2 <= calibration.position[i].level23) {
			log.error() << "[position " << i << "] invalid calibration zblock2 <= level23";
			good = false;
		}
		if (z3 >= calibration.position[i].level23) {
			log.error() << "[position " << i << "] invalid calibration zblock3 >= level23";
			good = false;
		}
		
		if (z3 <= calibration.position[i].level30) {
			log.error() << "[position " << i << "] invalid calibration zblock3 <= level30";
			good = false;
		}
		if (z0 >= calibration.position[i].level30) {
			log.error() << "[position " << i << "] invalid calibration zblock0 >= level30";
			good = false;
		}
	}
	
	if (!good) return -1;
	if (calibration.save("/opt/eeros/etc/delta-sort.conf")) {
		log.info() << "calibration saved";
		for(int i = 0; i < 4; i++){
		    log.info() << "p" << i << "l12 = " << calibration.position[i].level12;
		    log.info() << "p" << i << "l23 = " << calibration.position[i].level23;
		    log.info() << "p" << i << "l30 = " << calibration.position[i].level30;
		    log.info() << "p" << i << "x = " << calibration.position[i].x;
		    log.info() << "p" << i << "y = " << calibration.position[i].y;
		    for(int j = 0; j < 4; j++){
		      log.info() << "p" << i << "z" << j << " = " << calibration.position[i].zblock[j];
		    }
		}
	}
	else {
		log.error() << "calibration could not be saved";
	}
	
	log.warn() << "Insert all blocks and press the blue button";
	waitForBlueButton();
	
	done = true;
	safetySys.triggerEvent(safetyProp.doSwInit);
	
}
