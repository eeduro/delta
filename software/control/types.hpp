#pragma once

#include <eeros/math/Matrix.hpp>
#include "constants.hpp"

namespace eeduro {
  namespace delta {
		using AxisVector = eeros::math::Matrix<nofAxis, 1>;
		using AxisSquareMatrix = eeros::math::Matrix<nofAxis, nofAxis>;
  }
}