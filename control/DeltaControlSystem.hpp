#pragma once

#include <eeros/core/Executor.hpp>
#include <eeros/logger/Logger.hpp>
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
#include <eeros/control/PathPlannerConstAcc.hpp>

#include "types.hpp"
#include "Kinematic.hpp"
#include "Jacobian.hpp"
#include "DirectKinematic.hpp"
#include "Jacobi.hpp"
#include "Inertia.hpp"
#include "MotorModel.hpp"
#include "constants.hpp"

using namespace eeros::control;
using namespace eeros::logger;

namespace eeduro {
namespace delta {
  
class DeltaControlSystem {
 public:
  DeltaControlSystem();

  void start();
  void stop();

  /*
   * Methods for the sequencer
   */
  void setPathPlannerInput();

  /*
   * Components of the Controlsystem
   */
  AxisVector kM;
  AxisVector RA;

  PathPlannerConstAcc<AxisVector> pathPlanner;

  PeripheralInput<double> enc1;
  PeripheralInput<double> enc2;
  PeripheralInput<double> enc3;

  PeripheralOutput<double> mot1;
  PeripheralOutput<double> mot2;
  PeripheralOutput<double> mot3;

  Constant<AxisVector> voltageSetPoint;
  Constant<AxisVector> forceSetPoint;

  Switch<2, AxisVector> voltageSwitch;

  Mux<3, double, AxisVector> muxEnc;
  DeMux<3, double, AxisVector> demuxMot;

  DirectKinematic directKin;
  Kinematic kinematic;
  Jacobian jacobian;
  Inertia inertia;
  Jacobi jacobi;
  control::D<AxisVector> posDiff;
  control::D<AxisVector> posDiff2;
  Gain<AxisVector> posController;
  Gain<AxisVector> speedController;

  Sum<2, AxisVector> posSum;
  Sum<3, AxisVector> speedSum;
  Sum<2,AxisVector> accSum;
  Sum<2, AxisVector> forceSum;

  Saturation<AxisVector> speedLimitation;
  Saturation<AxisVector> accLimitation;
  Saturation<AxisVector> forceLimitation;
  Saturation<AxisVector> torqueLimitation;

  MotorModel motorModel;
  

  TimeDomain timedomain;
  Logger log;
};

} 
}
