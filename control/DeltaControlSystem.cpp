#include "DeltaControlSystem.hpp"

DeltaControlSystem::DeltaControlSystem() : 
	mouse("/dev/input/event1"),
	pathPlanner({1, 1, 1}, {10, 10, 10}, dt),
	circlePlanner(circleRadius, 3.1415 / 2),
	kM(kM1524, kM1524, kM1524),
	RA(RA1524, RA1524, RA1524),
	posSwitch(0),
	velSwitch(0),
	accSwitch(0),

	jacobian(kinematic.get_offset()),

	posController(kp),
	speedController(kd),

	inertia(jacobian),
	jacobi(jacobian),
	motorModel(kM, RA),
	voltageSwitch(1),
	directKin(kinematic),

	voltageSetPoint({0,0,0}),
	velSetPoint({0,0,0,}),
	accSetPoint({0,0,0}),

	enc1("enc1"),
	enc2("enc2"),
	enc3("enc3"),

	mot1("motor1"),
	mot2("motor2"),
	mot3("motor3"),
	timedomain("Main time domain", dt, true) {
		
		
	/*
	* ###
	* label the blocks
	* ###
	*/
	pathPlanner.setName("Pathplanner");
	mouse.setName("Mouse input");
	redVect.setName("Reduce vector");
	posSwitch.setName("Position Switch");
	velSwitch.setName("Velocity Switch");
	accSwitch.setName("Acceleration Switch");
	
	velSetPoint.setName("Velocity set point");
	accSetPoint.setName("Acceleration set point");

	enc1.setName("Encoder axismotor 1");
	enc2.setName("Encoder axismotor 2");
	enc3.setName("Encoder axismotor 3");
	muxEnc.setName("Mux encoder signal");
	directKin.setName("Direct kinematic");

	posSum.setName("Position difference");
	posController.setName("Position controller");
	posDiff.setName("Actual position derivation");

	speedSum.setName("Speed difference");
	speedLimitation.setName("Speed limitation");
	speedController.setName("Speed controller");
	accSum.setName("Acceleration difference");

	inertia.setName("Inertia");
	forceLimitation.setName("Force limitation");
	jacobi.setName("Jacobi");

	torqueLimitation.setName("Torque limitations");

	motorModel.setName("Motor model");
	voltageSetPoint.setName("Voltage setpoint");
	voltageSwitch.setName("Voltage switch");

	demuxMot.setName("Demux motors");

	mot1.setName("Axismotor 1");
	mot2.setName("Axismotor 2");
	mot3.setName("Axismotor 3");

	/*
	* ###
	* label the signals,
	* Des -> Desired,
	* A -> Actual,
	* x -> Value Kartesian Coordinatesystem,
	* phi -> Value in radian
	* diff -> difference between desired and actual
	* f -> Force
	* t -> Torque
	* V -> Voltage
	* ###
	*/
	pathPlanner.getPosOut().getSignal().setName("xDes");
	pathPlanner.getVelOut().getSignal().setName("dxDes");		// dx/dt -> Speed
	pathPlanner.getAccOut().getSignal().setName("ddxDes");		// ddx/dt² -> Acceleration

	mouse.getOut().getSignal().setName("mousePosition");
	redVect.getOut().getSignal().setName("mousePosition");

	posSwitch.getOut().getSignal().setName("xDes");
	velSwitch.getOut().getSignal().setName("dxDes");
	accSwitch.getOut().getSignal().setName("ddxDes");
	
	velSetPoint.getOut().getSignal().setName("dxDes");
	accSwitch.getOut().getSignal().setName("ddxDes");

	enc1.getOut().getSignal().setName("phiAct1");			// Actual position of encoder 1 in radian
	enc2.getOut().getSignal().setName("phiAct2");			// Actual position of encoder 2 in radian
	enc3.getOut().getSignal().setName("phiAct3");			// Actual position of encoder 3 in radian

	muxEnc.getOut().getSignal().setName("phiAct");			// all 4 encoder values combined in an AxisVector[4]

	directKin.getOut().getSignal().setName("xAct");			// Actual Position

	posSum.getOut().getSignal().setName("xDiff");
	posController.getOut().getSignal().setName("dxDes");
	posDiff.getOut().getSignal().setName("dxAct");

	speedSum.getOut().getSignal().setName("dxDiff");
	speedLimitation.getOut().getSignal().setName("dxDiff");
	speedController.getOut().getSignal().setName("ddxDes");

	accSum.getOut().getSignal().setName("ddxDiff");
	inertia.getOut().getSignal().setName("fxDes");
	forceLimitation.getOut().getSignal().setName("fxDes");

	jacobi.getOut().getSignal().setName("txDes");
	torqueLimitation.getOut().getSignal().setName("txDes");

	motorModel.getOut().getSignal().setName("UDes");
	voltageSetPoint.getOut().getSignal().setName("UDes");
	voltageSwitch.getOut().getSignal().setName("UDes");

	demuxMot.getOut(0).getSignal().setName("UDes Axismotor 1");
	demuxMot.getOut(1).getSignal().setName("UDes Axismotor 2");
	demuxMot.getOut(2).getSignal().setName("UDes Axismotor 3");


	/*
	* ###
	* configure blocks	  
	* ###
	*/
	torqueLimitation.enable();

	posSum.negateInput(1);
	speedSum.negateInput(2);


	/*
	* ###
	* connect blocks
	* ###
	*/
	muxEnc.getIn(0).connect(enc1.getOut());
	muxEnc.getIn(1).connect(enc2.getOut());
	muxEnc.getIn(2).connect(enc3.getOut());

	redVect.getIn().connect(mouse.getOut());
	posSwitch.getIn(0).connect(pathPlanner.getPosOut());
	posSwitch.getIn(1).connect(redVect.getOut());
	posSwitch.getIn(2).connect(circlePlanner.getOut());
	
	velSwitch.getIn(0).connect(pathPlanner.getVelOut());
	velSwitch.getIn(1).connect(velSetPoint.getOut());
	velSwitch.getIn(2).connect(velSetPoint.getOut());
	
	accSwitch.getIn(0).connect(pathPlanner.getAccOut());
	accSwitch.getIn(1).connect(accSetPoint.getOut());
	accSwitch.getIn(2).connect(accSetPoint.getOut());

	posSum.getIn(0).connect(posSwitch.getOut());
	posSum.getIn(1).connect(directKin.getOut());
	posController.getIn().connect(posSum.getOut());
	posDiff.getIn().connect(directKin.getOut());

	speedSum.getIn(0).connect(velSwitch.getOut());
	speedSum.getIn(1).connect(posController.getOut());
	speedSum.getIn(2).connect(posDiff.getOut());
	speedLimitation.getIn().connect(speedSum.getOut());
	speedController.getIn().connect(speedLimitation.getOut());

	accSum.getIn(0).connect(speedController.getOut());
	accSum.getIn(1).connect(accSwitch.getOut());

	inertia.getAccelerationInput().connect(accSum.getOut());
	inertia.getJointPosInput().connect(muxEnc.getOut());
	inertia.getTcpPosInput().connect(directKin.getOut());

	forceLimitation.getIn().connect(inertia.getOut());

	jacobi.getForceInput().connect(forceLimitation.getOut());
	jacobi.getJointPosInput().connect(muxEnc.getOut());
	jacobi.getTcpPosInput().connect(directKin.getOut());

	torqueLimitation.getIn().connect(jacobi.getOut());

	motorModel.getTorqueIn().connect(torqueLimitation.getOut());
	motorModel.getSpeedIn().connect(posDiff.getOut());

	voltageSwitch.getIn(0).connect(motorModel.getOut());
	voltageSwitch.getIn(1).connect(voltageSetPoint.getOut());

	directKin.getIn().connect(muxEnc.getOut());

	demuxMot.getIn().connect(voltageSwitch.getOut());

	mot1.getIn().connect(demuxMot.getOut(0));
	mot2.getIn().connect(demuxMot.getOut(1));
	mot3.getIn().connect(demuxMot.getOut(2));

	posSwitch.combine(velSwitch);
	posSwitch.combine(accSwitch);


	/*
	* ###
	* add all blocks to the timedomain
	* ###
	*/
	timedomain.addBlock(mouse);
	timedomain.addBlock(redVect);
	timedomain.addBlock(pathPlanner);
	timedomain.addBlock(circlePlanner);
	timedomain.addBlock(posSwitch);
	
	timedomain.addBlock(enc1);
	timedomain.addBlock(enc2);
	timedomain.addBlock(enc3);
	timedomain.addBlock(muxEnc);

	timedomain.addBlock(directKin);

	timedomain.addBlock(posSum);
	timedomain.addBlock(posDiff);
	timedomain.addBlock(posController);
	
	timedomain.addBlock(velSetPoint);
	timedomain.addBlock(velSwitch);

	timedomain.addBlock(speedSum);
	timedomain.addBlock(speedLimitation);
	timedomain.addBlock(speedController);
	
	timedomain.addBlock(accSetPoint);
	timedomain.addBlock(accSwitch);

	timedomain.addBlock(accSum);

	timedomain.addBlock(inertia);
	timedomain.addBlock(forceLimitation);
	timedomain.addBlock(jacobi);
	timedomain.addBlock(torqueLimitation);

	timedomain.addBlock(motorModel);
	timedomain.addBlock(voltageSetPoint);
	timedomain.addBlock(voltageSwitch);

	timedomain.addBlock(demuxMot);
	timedomain.addBlock(mot1);
	timedomain.addBlock(mot2);
	timedomain.addBlock(mot3);

	/*
	* ###
	* add timedomain to the executor
	* ###
	*/
	Executor::instance().add(timedomain);
}

void DeltaControlSystem::start() {
	log.info() << "Start control system";
	timedomain.start();
}

void DeltaControlSystem::stop() {
	log.info() << "Stop control system";
	timedomain.stop();
}

void DeltaControlSystem::setMouseInput() {
//	mouse.setInitPos(directKin.getOut().getSignal().getValue());
	pathPlanner.setInitPos(directKin.getOut().getSignal().getValue());   // disable pathplanner by setting the desired position to the current position
	posSwitch.switchToInput(1);                                          // set input to mouseinput, also switches the velSwitch and accSwitch
	voltageSwitch.switchToInput(0);
}

void DeltaControlSystem::setCircleInput() {
	circlePlanner.setInitPos(directKin.getOut().getSignal().getValue());
	pathPlanner.setInitPos(directKin.getOut().getSignal().getValue());   // disable pathplanner by setting the desired position to the current position
	posSwitch.switchToInput(2);                                          // set input to mouseinput, also switches the velSwitch and accSwitch
}

void DeltaControlSystem::setPathPlannerInput() {
	posSwitch.switchToInput(0);                                          // set input to pathplanner, also switches the velSwitch and accSwitch
}


DeltaControlSystem::~DeltaControlSystem() { }