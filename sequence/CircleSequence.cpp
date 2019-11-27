#include "CircleSequence.hpp"

using namespace eeduro::delta;

CircleSequence::CircleSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& safetyProp):
	Sequence(name, caller, true),
	controlSys(controlSys),
	safetySys(safetySys),
	safetyProp(safetyProp),
	wait("Wait in circle sequence", this),
	move("Move", this, controlSys),
	moveMouseCondition(controlSys),
	mouseExceptionSequence("Mouse Exception Sequence", this,  safetySys, safetyProp),
	moveMouseMonitor("MouseMoveMonitor", this, moveMouseCondition, SequenceProp::abort, &mouseExceptionSequence),
	blueButtonCondition(),
	blueButtonExceptionSequence("Blue button exception sequence", this, controlSys, safetySys, safetyProp),
	blueButtonMonitor("BlueButtonMonitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSequence) { 
		addMonitor(&blueButtonMonitor);
		addMonitor(&moveMouseMonitor);
	}


int CircleSequence::action() {
	moveMouseCondition.reset();
	controlSys.pathPlanner.setInitPos(controlSys.directKin.getOut().getSignal().getValue());
	controlSys.setPathPlannerInput();
	move({circleRadius, 0, tcpReady_z});
	controlSys.circlePlanner.setInitPos({circleRadius, 0, tcpReady_z});
	controlSys.setCircleInput();

	while (getRunningState() == SequenceState::running) {
		wait(0.2);
	}
}
