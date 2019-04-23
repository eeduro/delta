#include "HomingSequence.hpp"

using namespace eeduro::delta;

HomingSequence::HomingSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& properties, Calibration& calibration): 
	Sequence(name, caller, true),
	controlSys(controlSys),
	properties(properties),
	safetySys(safetySys),
	wait("wait", this),
	move("move", this, controlSys),
	calibration(calibration)
	{}

int HomingSequence::action()
{
	controlSys.setVoltageForInitializing({2,2,2,7});
					
	wait(2.5);

	controlSys.enc1.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc2.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc3.callInputFeature<>("setFqdPos", q012homingOffset);
	controlSys.enc4.callInputFeature<>("setFqdPos", q3homingOffset);

	wait(0.1);
	controlSys.pathPlanner.setInitPos(controlSys.directKin.getOut().getSignal().getValue());

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
