#include "Kinematic.hpp"
#include <cmath>

using namespace eeros::math;
using namespace eeduro::delta;

const double Kinematic::q_min = -1.75;
const double Kinematic::q_max = 0.85;

const double Kinematic::tcp_min = -0.02;
const double Kinematic::tcp_max = 0.02;
const double Kinematic::tcpz_min = -0.14;
const double Kinematic::tcpz_max = -0.05;

const double Kinematic::length_A(0.05);
const double Kinematic::length_B(0.1);
const double Kinematic::r_A(0.0525);
const double Kinematic::r_B(0.026);
const double Kinematic::r(r_A-r_B);
const double Kinematic::alpha1(0);
const double Kinematic::alpha2(2*M_PI/3);
const double Kinematic::alpha3(-2*M_PI/3);

const Matrix<3, 3> Kinematic::rotz1 = Matrix<3, 3>::createRotZ(Kinematic::alpha1);
const Matrix<3, 3> Kinematic::rotz2 = Matrix<3, 3>::createRotZ(Kinematic::alpha2);
const Matrix<3, 3> Kinematic::rotz3 = Matrix<3, 3>::createRotZ(Kinematic::alpha3);

Kinematic::Kinematic(const Vector3 offsetv) {
	offset = offsetv;
}

Kinematic::Kinematic() {
	offset.zero();
	
	Vector3 tcp;
	Vector3 q;
	q.zero();
	
	if (forward(q, tcp)) {
		offset = tcp;
	}
}

Kinematic::~Kinematic() { }

bool Kinematic::forward(const Vector3 q, Vector3& tcp) {
	Vector3 temp1;
	temp1(0) = r + length_A * cos(q(0));
	temp1(1) = 0;
	temp1(2) = length_A * sin(q(0));
	
	Vector3 temp2;
	temp2(0) = r + length_A * cos(q(1));
	temp2(1) = 0;
	temp2(2) = length_A * sin(q(1));
	
	Vector3 temp3;
	temp3(0) = r + length_A * cos(q(2));
	temp3(1) = 0;
	temp3(2) = length_A * sin(q(2));
	
	Vector3 pc1 = rotz1 * temp1;
	Vector3 pc2 = rotz2 * temp2;
	Vector3 pc3 = rotz3 * temp3;
	
	double rspher = length_B;
	
	double w1 = pc1.transpose() * pc1;
	double w2 = pc2.transpose() * pc2;
	double w3 = pc3.transpose() * pc3;
	
	double x1 = pc1(0);
	double y1 = pc1(1);
	double z1 = pc1(2);
	
	Vector3 vektordiff = pc2-pc1;
	double A = vektordiff(0);
	double B = vektordiff(1);
	double C = -vektordiff(2);
	double G = (w2-w1)/2;
	
	vektordiff = pc3-pc1;
	double D = vektordiff(0);
	double E = vektordiff(1);
	double F = -vektordiff(2);
	double H = (w3-w1)/2;
	
	double dnm = A*E-B*D;
	
	// x = (a1*z + b1)
	double a1 = (C*E-F*B)/dnm;
	double b1 = (G*E-H*B)/dnm;
	
	// y = (a2*z + b2)
	double a2 = (A*F - C*D)/dnm;
	double b2 = (A*H - G*D)/dnm;
	
	// a*z^2 + b*z + c = 0
	double a = a1*a1 + a2*a2 + 1;
	double b = 2*(a1*(b1-x1) + a2*(b2-y1) - z1);
	double c = -rspher*rspher + w1  +b1*(b1-2*x1) +b2*(b2-2*y1);
	
	double d = b*b - 4.0*a*c;
	
	// no solution exist
	if (d < 0) {
		return false;
	}
	else {
		// take only the solution with the lower value for Z
		double poseZ = (-b - sqrt(d))/(2*a);
		tcp(0) = a1*poseZ + b1 - offset(0);
		tcp(1) = a2*poseZ + b2 - offset(1);
		tcp(2) = poseZ - offset(2);
		return true;
	}
}

bool Kinematic::inverse(const Vector3 tcp, Vector3& q) {
	Vector3 tempTCP;

	tempTCP(0) = tcp(0) + offset(0);
	tempTCP(1) = tcp(1) + offset(1);
	tempTCP(2) = tcp(2) + offset(2);

	Vector3 tcp_1 = rotz1*tempTCP;
	Vector3 tcp_2 = rotz3*tempTCP;                  							   // invers rotz_2
	Vector3 tcp_3 = rotz2*tempTCP;										   // invers rotz_3
	double x1 = r;
	double LA_sqr = length_A*length_A;

	// 1. arm:

	double x0_1 = tcp_1(0);
	double y0_1 = tcp_1(1);
	double z0_1 = tcp_1(2);

	double a_1 = (x0_1*x0_1 + y0_1*y0_1 + z0_1*z0_1 + LA_sqr - length_B*length_B - x1*x1)/(2*z0_1 );
	double b_1 = (x1-x0_1)/z0_1;

	double A_1 = (b_1*b_1 +1);
	double B_1 = 2*(a_1*b_1 - x1);
	double C_1 = x1*x1 +a_1*a_1 - LA_sqr;
	// discriminant
	double d_1 = B_1*B_1 - 4*A_1*C_1;

	// no solution exist
	if(d_1 < 0) {
		return false;
	}
	else {
		double xj_1 = (-B_1 + sqrt(d_1))/(2*A_1);      							 // choosing outer point
		double zj_1 = a_1 + b_1*xj_1;
		
		// 1. joint coordinate
		q(0) = atan2(zj_1,(xj_1 - x1));
		
		// 2. arm:
		double x0_2 = tcp_2(0);
		double y0_2 = tcp_2(1);
		double z0_2 = tcp_2(2);
		
		double a_2 = (x0_2*x0_2 + y0_2*y0_2 + z0_2*z0_2 + LA_sqr - length_B*length_B - x1*x1)/(2*z0_2 );
		double b_2 = (x1-x0_2)/z0_2;
		
		double A_2 = (b_2*b_2 +1);
		double B_2 = 2*(a_2*b_2 - x1);
		double C_2 = x1*x1 +a_2*a_2 - LA_sqr;
		// discriminant
		double d_2 = B_2*B_2 - 4*A_2*C_2;
		
		// no solution exist
		if(d_2 < 0) {
			return false;
		}
		else {
			double xj_2 = (-B_2 + sqrt(d_2))/(2*A_2); 							// choosing outer point
			double zj_2 = a_2 + b_2*xj_2;
			
			// 2. joint coordinate
			q(1) = atan2(zj_2,(xj_2 - x1));
			
			// 3. arm:
			double x0_3 = tcp_3(0);
			double y0_3 = tcp_3(1);
			double z0_3 = tcp_3(2);
			
			double a_3 = (x0_3*x0_3 + y0_3*y0_3 + z0_3*z0_3 + LA_sqr - length_B*length_B - x1*x1)/(2*z0_3 );
			double b_3 = (x1-x0_3)/z0_3;
			
			double A_3 = (b_3*b_3 +1);
			double B_3 = 2*(a_3*b_3 - x1);
			double C_3 = x1*x1 +a_3*a_3 - LA_sqr;
			// discriminant
			double d_3 = B_3*B_3 - 4*A_3*C_3;
			
			// no solution exist
			if(d_3 < 0) {
				return false;
			}
			else {
				double xj_3 = (-B_3 + sqrt(d_3))/(2*A_3);  					    // choosing outer point
				double zj_3 = a_3 + b_3*xj_3;
				
				// 3. joint coordinate
				q(2) = atan2(zj_3,(xj_3 - x1));
				
				return true;
			}
		}
	}
}

const eeros::math::Vector3& Kinematic::get_offset() {
	return offset;
}
