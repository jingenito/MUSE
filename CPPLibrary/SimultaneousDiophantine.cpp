#include "pch.h"
#include "SimultaneousDiophantine.h"
#include "QSMatrix.h"
#include <cmath>
#include <tuple>
#include "AlgorithmsLLL.h"
#include "ContinuedFraction.h"

QSMatrix<long> CPPMathLibrary::SimultaneousDiophantine::SameDivisor(const std::vector<double>& x, const double& alpha, const double& epsilon) throw (IncorrectDimensionException*) {
	size_t n = x.size();
	size_t n1 = n + 1;

	double e = epsilon;
	double beta = 4 / ((4 * alpha) - 1);
	double beta_e = pow(beta, -1 * (double)n * n1 / 4) * pow(e, n1);

	QSMatrix<double> X(n1, n1, 0); //initialize an (n+1)x(n+1) matrix to all 0s
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

	QSMatrix<double> Y(n1, n1, 0);
	QSMatrix<long> C(n1, n1, 0);
	try {
		auto result = ReduceBasis_LLL<double>(X, alpha);
		Y = std::get<LLLType::LLL>(result);
		C = std::get<LLLType::C>(result);
	}
	catch (IncorrectDimensionException* idEx) {
		throw idEx;
	}

	return C;
}

QSMatrix<long> CPPMathLibrary::SimultaneousDiophantine::SameDivisorFromRealVector(const std::vector<double>& x, const double& alpha, const double& epsilon, const size_t convergentCount) throw (IncorrectDimensionException*) {
	size_t cuntCount, cuntOpCount; //dont care about these - only needed for ContinuedFractionExpansion
	long* qs; //initializing once 
	std::vector<double> rats;
	for (size_t i = 0; i < x.size(); i++) {
		qs = ContinuedFractionExpansion(x[i], cuntCount, cuntOpCount);
		RationalNumber r = FindConvergent(qs, convergentCount);
		rats.push_back((double)r);
	}
	try {
		return SameDivisor(rats, alpha, epsilon);
	}
	catch (IncorrectDimensionException* idEx) {
		throw idEx;
	}
}