/*
	Author: Joe Ingenito
	Date Created: 7/6/2020
	Description: Provides an implementation of the Simultaneous Diohpantine Approximation.
*/

#pragma once

#include <vector>
#include "RationalNumber.h"

namespace CPPMathLibrary {

	namespace SimultaneousDiophantine {

		std::vector<RationalNumber> SameDivisor(const std::vector<RationalNumber>& x, const double& alpha, const double& epsilon);

	}

}