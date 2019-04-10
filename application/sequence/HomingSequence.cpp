#include "HomingSequence.hpp"


using namespace eeduro::delta;


HomingSequence::HomingSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration): 
	Sequence(name, caller, true),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	wait("wait", this),
	move("move", this, controlSys),
	calibration(calibration)//,
	//ec(safetySys, properties),
	//emergencyLevel("Emergency Level Monitor", this, ec, eeros::sequencer::SequenceProp::abort)
	{
		//addMonitor(&emergencyLevel);
	}

int HomingSequence::action()
{
	controlSys.setVoltageForInitializing({2,2,2,-7});
					
	wait(2.5);

	controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc4.callInputFeature<>("setFqdPos", q3homingOffset);

	wait(0.1);
	log.warn() << "DK: " << controlSys.directKin.getOut().getSignal().getValue();
	controlSys.pathPlanner.setInitPos(controlSys.directKin.getOut().getSignal().getValue());
	log.warn() << "PP: " << controlSys.pathPlanner.getLastPoint();

	wait(0.1);

	controlSys.enableAxis();
	controlSys.setPathPlannerInput();
	
	AxisVector p = controlSys.pathPlanner.getLastPoint();
	p[0] = 0;
	p[1] = 0;
	p[2] = calibration.transportation_height;
	
	move(p);
	
	safetySys.triggerEvent(properties.homingDone);

}
