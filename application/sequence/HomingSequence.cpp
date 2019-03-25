#include "HomingSequence.hpp"


using namespace eeduro::delta;


HomingSequence::HomingSequence(std::string name, Sequencer& seq, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration): 
	Sequence(name, seq),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	wait("wait", this),
	move("move", this, controlSys),
	calibration(calibration){}

int HomingSequence::action()
{
	controlSys.setVoltageForInitializing({2,2,2,-7});
					
	wait(2.5);

	controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc4.callInputFeature<>("resetFqd");

	wait(0.1);

	controlSys.pathPlanner.setInitPos(controlSys.directKin.getOut().getSignal().getValue());

	wait(0.1);

	controlSys.enableAxis();
	
	move({0,0,calibration.transportation_height, pi/2});
	
	safetySys.triggerEvent(properties.homingDone);
}
