#include "pch.h"
#include "SimultaneousDiophantine.h"
#include "QSMatrix.h"
#include <cmath>

std::vector<CPPMathLibrary::RationalNumber> CPPMathLibrary::SimultaneousDiophantine::SameDivisor(const std::vector<RationalNumber>& x, const double& alpha, const double& epsilon) {
	size_t n = x.size();
	size_t n1 = n + 1;
	double beta = 4 / ( (4 * alpha) - 1 );
	double beta_e = pow(beta, -1 * (double)n * (double)n1 / 4) * pow(epsilon, n1);

	QSMatrix<RationalNumber> X(n1, n1, 0); //initialize an (n+1)x(n+1) matrix to all 0s
	// initialize the rest of the matrix
	X(0, 0) = beta_e;
	for (size_t i = 0; i < n; i++) {
		X(0, i + 1) = x[i];
	}
	for (size_t i = 1; i < n1; i++) {
		for (size_t j = 0; j < n1; j++) {
			if (i == j) {
				X(i, j) = -1;
			}
		}
	}


}