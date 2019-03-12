#pragma once

#include <eeros/math/Matrix.hpp>

using namespace eeros::math;

namespace eeduro {
	namespace delta {
		
		template < int M, int N >
		class Kinematics{
			public:
				virtual bool forward(const Matrix<N> q, Matrix<M> &tcp) = 0;
				virtual bool inverse(const Matrix<M> tcp, Matrix<N> &q) = 0;
		};
		
	}
}