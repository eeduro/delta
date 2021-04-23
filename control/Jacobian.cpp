#include "Jacobian.hpp"

using namespace eeduro::delta;

Jacobian::Jacobian(Vector3 offset) : offset(offset) { }

Jacobian::~Jacobian() { }

Vector3 Jacobian::getCartesianVelo(const Vector3& q, const Vector3& tcp, const Vector3& jointvelo) {
  calculate(q,tcp);
  return jacobi * jointvelo;
}

Vector3 Jacobian::getJointVelo(const Vector3& q, const Vector3& tcp, const Vector3& cartesianvelo) {
  calculate(q,tcp);
  return !jacobi * cartesianvelo;
}

Vector3 Jacobian::getDrivetorque(const Vector3& q, const Vector3& tcp, const Vector3& F_tcp) {
  calculate(q, tcp);
  return getDrivetorque(F_tcp);
}

Vector3 Jacobian::getDrivetorque(const Vector3& F_tcp) {
  return jacobi.transpose() * F_tcp;
}

bool Jacobian::calculate(const Vector3& q, const Vector3& tcp_offset) {
  Vector3 tcp = (tcp_offset + offset);
  Vector3 temp1;
  temp1(0) = Kinematic::r+Kinematic::length_A*cos(q(0));
  temp1(1) = 0;
  temp1(2) = Kinematic::length_A*sin(q(0));
  
  Vector3 temp2;
  temp2(0) = Kinematic::r+Kinematic::length_A*cos(q(1));
  temp2(1) = 0;
  temp2(2) = Kinematic::length_A*sin(q(1));
  
  Vector3 temp3;
  temp3(0) = Kinematic::r+Kinematic::length_A*cos(q(2));
  temp3(1) = 0;
  temp3(2) = Kinematic::length_A*sin(q(2));
  
  Vector3 s1 = tcp - Kinematic::rotz1*temp1;
  Vector3 s2 = tcp - Kinematic::rotz2*temp2;
  Vector3 s3 = tcp - Kinematic::rotz3*temp3;
  
  Vector3 temp2_1;
  temp2_1(0) = -Kinematic::length_A*sin(q(0));
  temp2_1(1) = 0;
  temp2_1(2) = Kinematic::length_A*cos(q(0));
  
  Vector3 temp2_2;
  temp2_2(0) = -Kinematic::length_A*sin(q(1));
  temp2_2(1) = 0;
  temp2_2(2) = Kinematic::length_A*cos(q(1));
  
  Vector3 temp2_3;
  temp2_3(0) = -Kinematic::length_A*sin(q(2));
  temp2_3(1) = 0;
  temp2_3(2) = Kinematic::length_A*cos(q(2));
  
  Vector3 b1 = Kinematic::rotz1*temp2_1;
  Vector3 b2 = Kinematic::rotz2*temp2_2;
  Vector3 b3 = Kinematic::rotz3*temp2_3;
  
  Matrix<3,3> tempA;
  tempA(0,0) = s1.transpose()*b1;
  tempA(1,0) = 0;
  tempA(2,0) = 0;
  
  tempA(0,1) = 0;
  tempA(1,1) = s2.transpose()*b2;
  tempA(2,1) = 0;
  
  tempA(0,2) = 0;
  tempA(1,2) = 0;
  tempA(2,2) = s3.transpose()*b3;
  
  Matrix<3,3> tempS;
  tempS(0,0) = s1(0);
  tempS(1,0) = s2(0);
  tempS(2,0) = s3(0);
  
  tempS(0,1) = s1(1);
  tempS(1,1) = s2(1);
  tempS(2,1) = s3(1);
  
  tempS(0,2) = s1(2);
  tempS(1,2) = s2(2);
  tempS(2,2) = s3(2);
  
  if(!tempS.isInvertible()) {
    return false;
  }
  
  Matrix<3,3> tempSi;   // inverse tempS
  tempSi = !tempS;
  
  jacobi = tempSi*tempA;
  return true;
}

Matrix<3,3> Jacobian::getJacobian() {
  return jacobi;
}
