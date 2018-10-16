#pragma once

#include <eeros/math/Matrix.hpp>

namespace eeduro {
  namespace delta {

		class Jacobian {
		public:
			Jacobian(eeros::math::Vector3 offset);
			virtual ~Jacobian();
			
			/** Calculate jacobi Matrix of the delta robot.
			* @param q	joint coordinates [rad]
			* @param tcp	TCP values ​​in the Cartesian coordinate system [m]
			*/
			virtual bool calculate(const eeros::math::Vector3& q, const eeros::math::Vector3& tcp);
			
			/** Returns Cartesian velocity of the TCP [m/s].
			* @param q	joint coordinates [rad]
			* @param tcp	TCP values ​​in the Cartesian coordinate system [m]
			* @param jointvelo	joint velocity [rad/s]
			*/
			virtual eeros::math::Vector3 getCartesianVelo(const eeros::math::Vector3& q, const eeros::math::Vector3& tcp, const eeros::math::Vector3& jointvelo);
			
			/** Returns joint velocity [rad/s].
			* @param q	joint coordinates [rad]
			* @param tcp	TCP values ​​in the Cartesian coordinate system [m]
			* @param cartesianvelo	Cartesian velocity [m/s]
			*/
			virtual eeros::math::Vector3 getJointVelo(const eeros::math::Vector3& q, const eeros::math::Vector3& tcp, const eeros::math::Vector3& cartesianvelo);
			
			/** Returns drive torque [Nm].
			* @param q	joint coordinates [rad]
			* @param tcp	TCP values ​​in the Cartesian coordinate system [m]
			* @param F_tcp	Cartesian force [N]
			*/
			virtual eeros::math::Vector3 getDrivetorque(const eeros::math::Vector3& q, const eeros::math::Vector3& tcp, const eeros::math::Vector3& F_tcp);
			virtual eeros::math::Vector3 getDrivetorque(const eeros::math::Vector3& F_tcp);
			
			virtual eeros::math::Matrix<3,3> getJacobian();
			
		private:
			eeros::math::Matrix<3,3> jacobi;
			eeros::math::Vector3 offset;
		};
  }
}

