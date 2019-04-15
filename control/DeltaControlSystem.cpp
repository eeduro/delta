#include "DeltaControlSystem.hpp"

DeltaControlSystem::DeltaControlSystem() : 
	mouse("/dev/input/event1"),
	pathPlanner({1, 1, 1, 5}, {10, 10, 10, 50}, dt),
	i(i1524, i1524, i1524, i0816),
	kM(kM1524, kM1524, kM1524, kM0816),
	RA(RA1524, RA1524, RA1524, RA0816),
	posSwitch(0),
	velSwitch(0),
	accSwitch(0),

	emagVal(false),
	emag("emag"),

	jacobian(kinematic.get_offset()),

	posController(kp),
	speedController(kd),

	inertia(jacobian),
	jacobi(jacobian),
	motorModel(kM, RA),
	voltageSwitch(1),
	directKin(kinematic),

	voltageSetPoint({0.0,0.0,0.0,0.0}),
	velSetPoint({0,0,0,0}),
	accSetPoint({0,0,0,0}),

	enc1("enc1"),
	enc2("enc2"),
	enc3("enc3"),
	enc4("enc4"),

	mot1("motor1"),
	mot2("motor2"),
	mot3("motor3"),
	mot4("motor4"),
	timedomain("Main time domain", dt, true) {
		
		
	/*
	* ###
	* label the blocks
	* ###
	*/
	pathPlanner.setName("Pathplanner");
	mouse.setName("Mouse input");
	posSwitch.setName("Position Switch");
	velSwitch.setName("Velocity Switch");
	accSwitch.setName("Acceleration Switch");
	
	velSetPoint.setName("Velocity set point");
	accSetPoint.setName("Acceleration set point");

	enc1.setName("Encoder axismotor 1");
	enc2.setName("Encoder axismotor 2");
	enc3.setName("Encoder axismotor 3");
	enc4.setName("Encoder tcp motor");
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
	mot4.setName("TCP Motor");

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

	mouse.getOut().getSignal().setName("mousexDes");		// Mouse Position
	mouse.getButtonOut().getSignal().setName("mouseButtons"); 	//3 Mouse Buttons

	posSwitch.getOut().getSignal().setName("xDes");
	velSwitch.getOut().getSignal().setName("dxDes");
	accSwitch.getOut().getSignal().setName("ddxDes");
	
	velSetPoint.getOut().getSignal().setName("dxDes");
	accSwitch.getOut().getSignal().setName("ddxDes");

	enc1.getOut().getSignal().setName("phiAct1");			// Actual position of encoder 1 in radian
	enc2.getOut().getSignal().setName("phiAct2");			// Actual position of encoder 2 in radian
	enc3.getOut().getSignal().setName("phiAct3");			// Actual position of encoder 3 in radian
	enc4.getOut().getSignal().setName("phiAct4");			// Actual position of encoder 4 in radian

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
	demuxMot.getOut(3).getSignal().setName("UDes TCP motor");


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
	muxEnc.getIn(3).connect(enc4.getOut());

	posSwitch.getIn(0).connect(pathPlanner.getPosOut());
	posSwitch.getIn(1).connect(mouse.getOut());
	
	velSwitch.getIn(0).connect(pathPlanner.getVelOut());
	velSwitch.getIn(1).connect(velSetPoint.getOut());
	
	accSwitch.getIn(0).connect(pathPlanner.getAccOut());
	accSwitch.getIn(1).connect(accSetPoint.getOut());

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
	mot4.getIn().connect(demuxMot.getOut(3));

	emag.getIn().connect(emagVal.getOut());
	
	posSwitch.connect(velSwitch);
	posSwitch.connect(accSwitch);


	/*
	* ###
	* add all blocks to the timedomain
	* ###
	*/
	timedomain.addBlock(mouse);
	timedomain.addBlock(pathPlanner);
	timedomain.addBlock(posSwitch);
	
	timedomain.addBlock(enc1);
	timedomain.addBlock(enc2);
	timedomain.addBlock(enc3);
	timedomain.addBlock(enc4);
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
	timedomain.addBlock(mot4);

	timedomain.addBlock(emagVal);
	timedomain.addBlock(emag);


	/*
	* ###
	* add timedomain to the executor
	* ###
	*/
	Executor::instance().add(timedomain);
}

void DeltaControlSystem::start() {
	timedomain.start();
}

void DeltaControlSystem::stop() {
	timedomain.stop();
}

void DeltaControlSystem::enableAxis() {
	voltageSwitch.switchToInput(0);
}

void DeltaControlSystem::disableAxis() {
	voltageSwitch.switchToInput(1);
	voltageSetPoint.setValue({0.0,0.0,0.0,0.0});
}

void DeltaControlSystem::setVoltageForInitializing(AxisVector u) {
	voltageSwitch.switchToInput(1);
	voltageSetPoint.setValue(u);
}

bool DeltaControlSystem::switchToPosControl() {
	if(homed || !allAxisStopped()){
	  return false;
	}
	setVoltageForInitializing({0, 0, 0, 0});
	homed = true;
	return true;
}


void DeltaControlSystem::goToPos(double x, double y, double z, double phi) {
	AxisVector p;
	p << x, y, z, phi;
	pathPlanner.gotoPoint(p);
}

AxisVector DeltaControlSystem::getTcpPos() {
	return directKin.getOut().getSignal().getValue();
}

AxisVector DeltaControlSystem::getAxisPos() {
	return muxEnc.getOut().getSignal().getValue();
}

bool DeltaControlSystem::allAxisStopped(double maxSpeed) {
	for(int i = 0; i < (nofAxis-1); i++) {
		if(posDiff.getOut().getSignal().getValue()[i] > maxSpeed) return false;
	}
	return true;
}

bool DeltaControlSystem::axisHomed() {
	return homed;
}

void DeltaControlSystem::setMouseInput()
{
	mouse.setInitPos(directKin.getOut().getSignal().getValue());
	pathPlanner.setInitPos(directKin.getOut().getSignal().getValue());		//disable pathplanner by setting the desired position to the current position
	posSwitch.switchToInput(1);							// set input to mouseinput, also switches the velSwitch and accSwitch
	voltageSwitch.switchToInput(0);
}

void DeltaControlSystem::setPathPlannerInput()
{
	posSwitch.switchToInput(0);		// set input to pathplanner, also switches the velSwitch and accSwitch
	voltageSwitch.switchToInput(0);
}



DeltaControlSystem::~DeltaControlSystem() { }