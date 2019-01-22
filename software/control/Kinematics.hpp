#pragma once

#include <eeros/math/Matrix.hpp>

namespace eeduro {
	namespace delta {
		
		template < int M, int N >
		class Kinematics{
			public:
				virtual bool forward(const eeros::math::Matrix<N> q, eeros::math::Matrix<M> &tcp) = 0;
				virtual bool inverse(const eeros::math::Matrix<M> tcp, eeros::math::Matrix<N> &q) = 0;
		};
		
	}
}