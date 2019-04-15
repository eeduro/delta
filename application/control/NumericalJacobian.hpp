#pragma once

#include <eeros/math/Matrix.hpp>

#include "Kinematics.hpp"

using namespace eeros::math;

namespace eeduro {
	namespace delta {
		template < int M, int N, typename T = double >
		class NumericalJacobian {
			public:
				NumericalJacobian(Kinematics<M,N> &kinematics, T delta) :
					kinematics(kinematics), delta(delta) { }

				virtual bool calculate(const Matrix<N>& q) {
					Matrix<M> tcp;
					if (!kinematics.forward(q, tcp))
						return false;
					
					Matrix<M,N> P;
					Matrix<N,N> Q;
					
					for (int i = 0; i < N; i++) {
						Matrix<N> q_new(q);
						q_new[i] += delta;
						
						Matrix<M> tcp_new;
						if (!kinematics.forward(q_new, tcp_new))
							return false;
						
						Matrix<N> dq;
						Matrix<M> dtcp;
						
						dq = (q_new - q);
						dtcp = (tcp_new - tcp);
						
						for (int j = 0; j < M; j++) P(j,i) = dtcp(j);
						for (int j = 0; j < N; j++) Q(j,i) = dq(j);
					}
					
					jacobian = P * !Q;
					return true;
				}

				virtual const Matrix<M,N>& getJacobian() { return jacobian; }

			private:
				Matrix<M,N> jacobian;
				Kinematics<M,N> &kinematics;
				T delta;
		};
	}
}