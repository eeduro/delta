#pragma once

#include <cmath>
#include <eeros/math/Matrix.hpp>
#include "Kinematic.hpp"

using namespace eeros::math;

namespace eeduro {
namespace delta {
  
class Jacobian {
 public:
  Jacobian(Vector3 offset);
  virtual ~Jacobian();
  
  /** Calculate jacobi Matrix of the delta robot.
  * @param q	joint coordinates [rad]
  * @param tcp	TCP values ​​in the Cartesian coordinate system [m]
  */
  virtual bool calculate(const Vector3& q, const Vector3& tcp);
  
  /** Returns Cartesian velocity of the TCP [m/s].
  * @param q	joint coordinates [rad]
  * @param tcp	TCP values ​​in the Cartesian coordinate system [m]
  * @param jointvelo	joint velocity [rad/s]
  */
  virtual Vector3 getCartesianVelo(const Vector3& q, const Vector3& tcp, const Vector3& jointvelo);
  
  /** Returns joint velocity [rad/s].
  * @param q	joint coordinates [rad]
  * @param tcp	TCP values ​​in the Cartesian coordinate system [m]
  * @param cartesianvelo	Cartesian velocity [m/s]
  */
  virtual Vector3 getJointVelo(const Vector3& q, const Vector3& tcp, const Vector3& cartesianvelo);
  
  /** Returns drive torque [Nm].
  * @param q	joint coordinates [rad]
  * @param tcp	TCP values ​​in the Cartesian coordinate system [m]
  * @param F_tcp	Cartesian force [N]
  */
  virtual Vector3 getDrivetorque(const Vector3& q, const Vector3& tcp, const Vector3& F_tcp);
  virtual Vector3 getDrivetorque(const Vector3& F_tcp);
  
  virtual Matrix<3,3> getJacobian();
  
 private:
  Matrix<3,3> jacobi;
  Vector3 offset;
};

}
}

