#include "pch.h"
#include "SimultaneousDiophantine.h"
#include <cmath>
#include <stdexcept>
#include <tuple>
#include "AlgorithmsLLL.h"
#include "ContinuedFraction.h"

QSMatrix<int> CPPMathLibrary::SimultaneousDiophantine::SameDivisor(const std::vector<double>& x, const double& alpha, const double& epsilon) throw (IncorrectDimensionException*) {
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
	QSMatrix<int> C(n1, n1, 0);
	try {
		auto result = LLL::ReduceBasis_LLL<double>(X, alpha);
		Y = std::get<LLL::LLLType::LLL>(result);
		C = std::get<LLL::LLLType::C>(result);
	}
	catch (IncorrectDimensionException* idEx) {
		throw idEx;
	}

	return C;
}

QSMatrix<int> CPPMathLibrary::SimultaneousDiophantine::SameDivisorFromRealVector(const std::vector<double>& x, const double& alpha, const double& epsilon, const size_t& quotientCount) throw (IncorrectDimensionException*) {
	size_t cuntCount = 10, cuntOpCount = 0; 
	int* qs; //initializing once 
	std::vector<double> rats;
	for (size_t i = 0; i < x.size(); i++) {
		qs = ContinuedFraction::ContinuedFractionExpansion(x[i], cuntCount, cuntOpCount);
		RationalNumber r = ContinuedFraction::FindConvergent(qs, quotientCount);
		rats.push_back((double)r);
	}
	try {
		return SameDivisor(rats, alpha, epsilon);
	}
	catch (IncorrectDimensionException* idEx) {
		throw idEx;
	}
}

QSMatrix<int> CPPMathLibrary::SimultaneousDiophantine::IteratedLLL(const QSMatrix<double>& matrix, const double& alpha, const double& epsilon, const size_t& qmax, const size_t& nmax) {
	if (alpha < 0.25) {
		throw new std::invalid_argument("alpha < 0.25");
	}
	if (alpha > 1) {
		throw new std::invalid_argument("alpha > 1");
	}
	if (epsilon <= 0) {
		throw new std::invalid_argument("epsilon <= 0");
	}
	if (epsilon >= 1) {
		throw new std::invalid_argument("epsilon >= 1");
	}

	size_t m = matrix.get_rows();
	size_t n = matrix.get_cols();
	size_t nm = n + m;

	double e = epsilon;
	double beta = 4 / ((4 * alpha) - 1);
	double c = pow(pow(beta, -1 * ((double)nm - 1) / 4) * e, (double)nm / m);

	QSMatrix<double> B(nm, nm, 0); //initialize an (n+m)x(n+m) matrix to all 0s
	// initialize the rest of the matrix
	for (size_t i = 0; i < nm; i++) {
		for (size_t j = 0; j < nm; j++) {
			if (i == j) {
				// main diagonal
				if (i < m) {
					B(i, j) = c;
				}
				else {
					B(i, j) = 1;
				}
			}
			else if (i < m && j >= m && j <= n) {
				size_t k = m - i - 1; // need to fill bottom up
				B(k, j) = matrix(i, j - m);
			}
			else {
				B(i, j) = 0;
			}
		}
	}

	QSMatrix<int> C(nm, nm, 0);
	size_t j = nmax;
	while (--j >= 0) {
		try {
			auto result = CPPMathLibrary::LLL::ReduceBasis_LLL<double>(B, alpha);
			C = std::get<LLL::LLLType::C>(result);

			double val = 1 / pow(beta, (double)nm / m);
			for (size_t i = 0; i < m; i++) {
				// divide the first m columns by beta ^ (n + m) / m
				std::vector<double> col = B.getColumnVector<double>(i);
				col = col * val;
				B.setColumnVector(col, i);
			}

			// check the new max q guaranteed with the new value for c
			c = B(0, 0);
			double upper_bound = pow(beta, ((double)nm - 1) / 4) * pow(c, (-1.0 * n) / nm);
			if (upper_bound > qmax) {
				break;
			}

		}
		catch (IncorrectDimensionException* idEx) {
			throw idEx;
		}
	}

	return C;
}