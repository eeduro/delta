#include "CalibrateSequence.hpp"

using namespace eeduro::delta;

CalibrateSequence::CalibrateSequence(std::string name, eeros::sequencer::Sequence* caller, DeltaControlSystem& cs, Calibration& cal) :
  Sequence(name, caller, true),
  controlSys(cs),
  calibration(cal),
  wait("Wait", this) {
    HAL& hal = HAL::instance();
    buttonBlue = hal.getLogicInput("buttonBlue", false);
    ledBlue = hal.getLogicOutput("ledBlue", false);
    ledGreen = hal.getLogicOutput("ledGreen", false);
  }

int CalibrateSequence::action() {
  ledGreen->set(false);
  ledBlue->set(true);
  log.info() << "Start calibration";
  
  const char *block[] = { "[no block]", "[block 1]", "[block 2]", "[block 3]" };
  controlSys.voltageSetPoint.setValue({0, 0, 0, 0});	// allow to move freely
  controlSys.voltageSwitch.switchToInput(1);	// choose fix voltage setpoint
  wait(3);
  
  for (int i = 0; i < 4; i++) {		// i = blocks
    for (int j = 0; j < 4; j++) {	// j = positions
      double minx = 100.0;
      double maxx = -100.0;
      double miny = 100.0;
      double maxy = -100.0;
      double minz = 100.0;
      double maxz = -100.0;
      log.warn() << "move TCP to position " << j << " with block " << block[i] << " present and press the blue button when done";
      while (!buttonBlue->get());
      wait(1);
      log.warn() << "move TCP inside area of block " << block[i] << " in position " << j << " and press the blue button when done";
      AxisVector p;
      while (!buttonBlue->get()) {
        p = controlSys.directKin.getOut().getSignal().getValue();
        
        if (p[2] < minz) minz = p[2];
        else if (p[2] > maxz) maxz = p[2];
        
        if (i == 3) {	// determine x / y positions only for block 3
          if (p[0] < minx) minx = p[0];
          else if (p[0] > maxx) maxx = p[0];
          if (p[1] < miny) miny = p[1];
          else if (p[1] > maxy) maxy = p[1];
        }
      }
      log.info() << "z min: " << minz * 1000 << "\tz max: " << maxz * 1000 << "\tdiff: " << (maxz - minz) * 1000;
      log.info() << "position: " << p;
      calibration.position[j].zblockmin[i] = minz;
      calibration.position[j].zblockmax[i] = maxz;
      if (i == 3) {
        calibration.position[j].x = (maxx + minx) / 2;
        calibration.position[j].y = (maxy + miny) / 2;
      }
      wait(2);
    }
  }

  for (int i = 0; i < 4; i++) {
    calibration.position[i].level12 = (calibration.position[i].zblockmin[1] + calibration.position[i].zblockmax[2]) / 2;
    calibration.position[i].level23 = (calibration.position[i].zblockmin[2] + calibration.position[i].zblockmax[3]) / 2;
    calibration.position[i].level30 = (calibration.position[i].zblockmin[3] + calibration.position[i].zblockmax[0]) / 2;
  }
  
  if (calibration.save("/home/ost/bin/delta-sort.conf")) {
    log.info() << "calibration saved";
    for (int i = 0; i < 4; i++) {
      log.info() << "p" << i << "l12 = " << calibration.position[i].level12;
      log.info() << "p" << i << "l23 = " << calibration.position[i].level23;
      log.info() << "p" << i << "l30 = " << calibration.position[i].level30;
      log.info() << "p" << i << "x = " << calibration.position[i].x;
      log.info() << "p" << i << "y = " << calibration.position[i].y;
      for (int j = 0; j < 4; j++) {
        log.info() << "p" << i << "z" << j << "min = " << calibration.position[i].zblockmin[j];
        log.info() << "p" << i << "z" << j << "max = " << calibration.position[i].zblockmax[j];
      }
    }
  } else log.error() << "calibration could not be saved";
  
  log.warn() << "Insert all blocks and press the blue button";
  while (!buttonBlue->get());
  ledBlue->set(true);
  controlSys.voltageSwitch.switchToInput(0);	// choose controller setpoint
  return(0);
}
