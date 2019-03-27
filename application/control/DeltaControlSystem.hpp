#pragma once

#include <eeros/control/Sum.hpp>
#include <eeros/control/D.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/MouseInput.hpp>
#include <eeros/control/Saturation.hpp>
#include <eeros/control/Switch.hpp>
#include <eeros/control/Mux.hpp>
#include <eeros/control/DeMux.hpp>

#include "types.hpp"
#include "Kinematic.hpp"
#include "Jacobian.hpp"
#include "DirectKinematic.hpp"
#include "Jacobi.hpp"
#include "Inertia.hpp"
#include "MotorModel.hpp"
#include "constants.hpp"
#include "PathPlanner.hpp"


using namespace eeros::control;
using namespace eeduro::delta;

namespace eeduro{
	namespace delta{

		class DeltaControlSystem {
			public:
				DeltaControlSystem();
				~DeltaControlSystem();

				void start();
				void stop();

				/*
				* ###
				* Methods for the sequencer
				* ###
				*/
				void enableAxis();
				void disableAxis();
				void setVoltageForInitializing(AxisVector u);
				bool switchToPosControl();
				bool axisHomed();
				AxisVector getTcpPos();
				AxisVector getAxisPos();
				void goToPos(double x, double y, double z, double phi);
				bool allAxisStopped(double maxSpeed = 0.001);
				void setMouseInput();
				void setPathPlannerInput();

				bool homed;


				/*
				* ###
				* Components of the Controlsystem
				* ###
				*/
				AxisVector i;
				AxisVector kM;
				AxisVector RA;
				Kinematic kinematic;
				Jacobian jacobian;

				MouseInput mouse;
				PathPlanner pathPlanner;

				Switch<2,AxisVector> inputSwitch;

				PeripheralInput<double> enc1;
				PeripheralInput<double> enc2;
				PeripheralInput<double> enc3;
				PeripheralInput<double> enc4;

				PeripheralOutput<double> mot1;
				PeripheralOutput<double> mot2;
				PeripheralOutput<double> mot3;
				PeripheralOutput<double> mot4;

				Mux<4, double, AxisVector> muxEnc;
				DeMux<4, double, AxisVector> demuxMot;

				eeros::control::D<AxisVector> posDiff;

				Sum<2, AxisVector> posSum;
				Gain<AxisVector> posController;

				Sum<3, AxisVector> speedSum;
				Saturation<AxisVector> speedLimitation;
				Gain<AxisVector> speedController;

				Sum<2,AxisVector> accSum;

				Inertia inertia;
				Jacobi jacobi;

				Saturation<AxisVector> forceLimitation;

				Saturation<AxisVector> torqueLimitation;

				MotorModel motorModel;

				Switch<2, AxisVector> voltageSwitch;
				Constant<AxisVector> voltageSetPoint;

				PeripheralOutput<bool> emag;
				Constant<bool> emagVal;  

				DirectKinematic directKin;	

				TimeDomain timedomain;

		};
	} 
}