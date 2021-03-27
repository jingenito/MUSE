/*
	Author: Joe Ingenito
	Date Created: 7/6/2020
	Description: Provides an implementation of the Simultaneous Diohpantine Approximation.
*/

#pragma once

#include <vector>
#include "IncorrectDimensionException.h"
#include "QSMatrix.h"

namespace CPPMathLibrary {

	namespace SimultaneousDiophantine {

		// Compute a Simultaneous Diophantine Approximation with a common divisor for all numerators. Assumes the vector x contains the desired accuracy of all real numbers.
		__declspec(dllexport) QSMatrix<int> SameDivisor(const std::vector<double>& x, const double& alpha, const double& epsilon) throw (IncorrectDimensionException*);

		// Compute a Simultaneous Diophantine Approximation with a common divisor for all numerators. Will compute a continued fraction approximation on the vector x.
		__declspec(dllexport) QSMatrix<int> SameDivisorFromRealVector(const std::vector<double>& x, const double& alpha, const double& epsilon, const size_t& quotientCount) throw (IncorrectDimensionException*);

		// Run a dyadic version of iterated LLL on the given matrix of real numbers with the specified upperbound qmax > 1, will automatically rationalize the matrix values.
		__declspec(dllexport) std::vector< QSMatrix<int> > IteratedLLL_Dyadic(const QSMatrix<double>& matrix, const double& alpha, const double& epsilon, const size_t& qmax, const size_t& M);

		// Calculate the Dirichlet Coefficient of the approximation matrix compared to the real vaues
		__declspec(dllexport) double DirichletCoefficient(const QSMatrix<double>& matrix, const QSMatrix<double>& real_values);

	}

}