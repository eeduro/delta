#pragma once

#include <eeros/math/Matrix.hpp>

#include "Kinematics.hpp"

namespace eeduro {
  namespace delta {

		class Kinematic : public Kinematics<3,3> {
		public:
			/** A new kinematic Delta-Robot.
			* @param offsetv	Offset TCP for example, the length of the magnet in the z-axis
			*/
			Kinematic(const eeros::math::Vector3 offsetv);
			Kinematic();

			virtual ~Kinematic();

			/** Forward kinematics of the delta robot.
			* @param q	joint coordinates [rad]
			* @param tcp	TCP values ​​in the Cartesian coordinate system [m]
			*/
			virtual bool forward(const eeros::math::Vector3 q, eeros::math::Vector3& tcp);

			/** Inverse kinematics of the delta robot.
			* @param q	joint coordinates [rad]
			* @param tcp	TCP values ​​in the Cartesian coordinate system [m]
			*/
			virtual bool inverse(const eeros::math::Vector3 tcp, eeros::math::Vector3& q);
			
			virtual const eeros::math::Vector3& get_offset();

			static const double q_min;
			static const double q_max;

			static const double tcp_min;  /**< min x,y coordinate of workspace [m]*/
			static const double tcp_max;  /**< max x,y coordinate of workspace [m]*/
			static const double tcpz_min; /**< min z coordinate of workspace [m]*/
			static const double tcpz_max; /**< max z coordinate of workspace [m]*/

			static const double length_A; /**< Length of the small arm [m]*/
			static const double length_B; /**< Length of the large arm [m]*/
			static const double r_A; 	  /**< Radius of motors mount*/
			static const double r_B;	  /**< Radius of TCP*/
			static const double r;
			static const double alpha1;   /**< Rotation of the first arm [rad]*/
			static const double alpha2;   /**< Rotation of the second arm [rad]*/
			static const double alpha3;   /**< Rotation of the third arm [rad]*/

			static const eeros::math::Matrix<3,3> rotz1; /**< Rotation Matrix of the first arm [rad]*/
			static const eeros::math::Matrix<3,3> rotz2; /**< Rotation Matrix of the second arm [rad]*/
			static const eeros::math::Matrix<3,3> rotz3; /**< Rotation Matrix of the third arm [rad]*/

		private:
			eeros::math::Vector3 offset; 		  /**< Offset TCP for example, the length of the magnet in the z-axis*/
		};
  }
}