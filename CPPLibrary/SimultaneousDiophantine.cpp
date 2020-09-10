#include "pch.h"
#include "SimultaneousDiophantine.h"
#include "StringParsing.h"
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

std::vector< QSMatrix<int> > CPPMathLibrary::SimultaneousDiophantine::IteratedLLL(const QSMatrix<double>& matrix, const double& alpha, const double& epsilon, const size_t& qmax) {
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
	double d = 1.0 / epsilon;
	int k_prime = ceil(((-1.0 * (nm - 1) * nm) / (4.0 * n)) + (m * (log(qmax) / log(2)) / n));

	double val = 1.0 / pow(d, (double)nm / m);
	std::vector< QSMatrix<int> > outputVec;
	for (size_t k = 0; k < k_prime; k++) {
		try {
			auto result = CPPMathLibrary::LLL::ReduceBasis_LLL<double>(B, alpha);
			C = std::get<LLL::LLLType::C>(result);

			QSMatrix<int> temp(m, nm, 0); //will store the approximation before being added to the output vector
			for (size_t i = 0; i < m; i++) {
				size_t j = m - i - 1; //need to flip the output matrix
				temp.setRowVector<int>(C.getRowVector<int>(j), i); //set the ith row of temp with the jth row of C

				// divide the first m columns by beta ^ (n + m) / m
				std::vector<double> col = B.getColumnVector<double>(i);
				col = col * val;
				B.setColumnVector(col, i);
			}
			outputVec.push_back(temp);

			double upper_bound = abs(pow(beta, (((double)nm - 1) * nm) / (4 * (double)m)) * pow(d, ((double)(k + 1.0) * n) / m));
			if (upper_bound > qmax) {
				break;
			}
		}
		catch (IncorrectDimensionException* idEx) {
			throw idEx;
		}
	}

	return outputVec;
}

std::vector< QSMatrix<int> > CPPMathLibrary::SimultaneousDiophantine::IteratedLLL_Dyadic(const QSMatrix<double>& matrix, const double& alpha, const double& epsilon, const size_t& qmax, const size_t& M) {
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
	double threshold = (pow(nm, 2) / m) - (double)nm / m * (log(e) / log(CPPMathLibrary::StringParsing::e));

	double divisor = pow(2, M);
	if (M <= threshold) {
		throw new std::invalid_argument("M <= threshold");
	}
	if (qmax >= divisor) {
		throw new std::invalid_argument("qmax >= divisor");
	}
	//passed threshold test, rationalize c
	c = ceil(divisor * c) / divisor;

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
	double d = 1.0 / epsilon;
	int k_prime = ceil(((-1.0 * (nm - 1) * nm) / (4.0 * n)) + (m * (log(qmax) / log(2)) / n));

	double val = pow(2, (double)M - ((double)nm / m));
	std::vector< QSMatrix<int> > outputVec;
	for (size_t k = 0; k < k_prime; k++) {
		try {
			auto result = CPPMathLibrary::LLL::ReduceBasis_LLL<double>(B, alpha);
			C = std::get<LLL::LLLType::C>(result);

			QSMatrix<int> temp(m, nm, 0); //will store the approximation before being added to the output vector
			for (size_t i = 0; i < m; i++) {
				size_t j = m - i - 1; //need to flip the output matrix
				temp.setRowVector<int>(C.getRowVector<int>(j), i); //set the ith row of temp with the jth row of C

				// divide the first m columns by beta ^ (n + m) / m
				std::vector<double> col = B.getColumnVector<double>(i);
				col = col * val;
				//take the ceiling of the col
				for (size_t l = 0; l < col.size(); l++)
					col[l] = 1 / ceil(col[l] * val) * pow(2, M);

				B.setColumnVector(col, i);
			}
			outputVec.push_back(temp);

			double upper_bound = abs(pow(beta, (((double)nm - 1) * nm) / (4 * (double)m)) * pow(d, ((double)(k + 1.0) * n) / m));
			if (upper_bound > qmax) {
				break;
			}
		}
		catch (IncorrectDimensionException* idEx) {
			throw idEx;
		}
	}

	return outputVec;
}