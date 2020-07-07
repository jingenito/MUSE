/*
	Author: Joe Ingenito
	Date Created: 7/6/2020
	Description: Provides an implementation of the Simultaneous Diohpantine Approximation.
*/

#pragma once

#include <vector>
#include "IncorrectDimensionException.h"

namespace CPPMathLibrary {

	namespace SimultaneousDiophantine {

		__declspec(dllexport) QSMatrix<long> SameDivisor(const std::vector<double>& x, const double& alpha, const double& epsilon) throw (IncorrectDimensionException*);

	}

}