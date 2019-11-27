#include "MouseSequence.hpp"
#include "../safety/DeltaSafetyProperties.hpp"
#include "ExceptionSequence.hpp"

using namespace eeduro::delta;

MouseSequence::MouseSequence(std::string name, Sequence* caller, DeltaControlSystem& controlSys, SafetySystem& safetySys, DeltaSafetyProperties& safetyProp) :
	Sequence(name, caller, true),
	controlSys(controlSys),
	safetySys(safetySys),
	safetyProp(safetyProp),
	wait("Wait in mouse move sequence", this),
	mouseTimeoutSequence("Mouse TimeOut Exception Sequence", this, safetySys, safetyProp),
	blueButtonCondition(),
	blueButtonExceptionSequence("Blue button exception sequence", this, controlSys, safetySys, safetyProp),
	blueButtonMonitor("BlueButtonMonitor", this, blueButtonCondition, SequenceProp::abort, &blueButtonExceptionSequence) {
		setTimeoutTime(2.0);
		setTimeoutExceptionSequence(mouseTimeoutSequence);
		setTimeoutBehavior(SequenceProp::abort);
		addMonitor(&blueButtonMonitor);
	}

int MouseSequence::action() {
	controlSys.setMouseInput();
	auto pos = controlSys.circlePlanner.getOut().getSignal().getValue();
	controlSys.mouse.setInitPos(pos[0], pos[1], pos[2], 0);
	mousePosPrev = controlSys.redVect.getOut().getSignal().getValue();

	while (getRunningState() == SequenceState::running) {
		auto pos = controlSys.redVect.getOut().getSignal().getValue();	
		if (pos != mousePosPrev) resetTimeout();
		mousePosPrev = pos;
		wait(0.1);
	}
}

