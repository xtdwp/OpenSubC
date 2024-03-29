//! \file constants.h
//! A collection of constants

#ifndef OPENSUBC_CONSTANTS_H
#define OPENSUBC_CONSTANTS_H

#include <array>
#include <cmath>
#include <limits>
#include <vector>

namespace opensubc {

	//定义的全局常量都放在这里

	// Run modes
	constexpr int RUN_MODE_CALCULATING{ 1 };
	constexpr int RUN_MODE_PLOTTING{ 2 };

	// Math Constants
	constexpr double PI{ 3.141592653589793};
	constexpr double g{ 9.80665 };

} // namespace opensubc

#endif // OPENSUBC_CONSTANTS_H
