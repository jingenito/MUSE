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
	size_t count = 10, opcount = 0;
	int* qs; //initializing once 
	std::vector<double> rats;
	for (size_t i = 0; i < x.size(); i++) {
		qs = ContinuedFraction::ContinuedFractionExpansion(x[i], count, opcount);
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

	double beta = 4 / ((4 * alpha) - 1);
	double c = pow(pow(beta, -1.0 * ((double)nm - 1.0) / 4.0) * epsilon, (double)nm / m);
	double threshold = (pow(nm, 2.0) / m) - ((double)nm / m * log(epsilon));

	double divisor = pow(2, M);
	if (M <= threshold) {
		throw new std::invalid_argument("M <= threshold");
	}
	if (qmax >= divisor) {
		throw new std::invalid_argument("qmax >= divisor");
	}
	//passed threshold test, rationalize c
	c = ceil(divisor * c) / divisor;
	std::vector<double> cHats; // need to keep track of cHat at every iteration since they will not be saved in the matrix with this version
	cHats.push_back(c);

	QSMatrix<double> newMatrix(m, n, 0);
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++) {
			newMatrix(i, j) = ceil(matrix(i, j) * divisor) / divisor;
		}
	}

	QSMatrix<double> B(nm, nm, 0); //initialize an (n+m)x(n+m) matrix to all 0s
	// c block
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < m; j++) {
			if (i == j) {
				B(i, j) = c;
			}
		}
	}

	// input block
	for (size_t i = 0; i < m; i++) {
		for (size_t j = m; j < nm; j++) {
			size_t k = m - i - 1; // need to fill bottom up
			double x = newMatrix(k, j - m);
			if (x > 0 && x < 1) {
				B(i, j) = x;
			}
			else {
				throw new std::invalid_argument("One of the values in the input matrix was not between 0 and 1");
			}
		}
	}

	// -1 block
	for (size_t i = m; i < nm; i++) {
		for (size_t j = m; j < nm; j++) {
			if (i == j) {
				B(i, j) = -1.0;
			}
		}
	}

	QSMatrix<int> C(nm, nm, 0);
	double d = 1.0 / epsilon;
	int k_prime = (size_t)ceil(((-1.0 * (nm - 1) * nm) / (4.0 * n)) + (((double)m / n) * (log(qmax) / log(2))));
	double tempVal = divisor * pow(d, -1.0 * (double)nm / m);
	//std::cout << "K_prime: " << k_prime << std::endl;

	std::vector< QSMatrix<int> > outputVec;
	for (size_t k = 0; k < k_prime; k++) {
		try {
			double upper_bound = abs(pow(2, (((double)nm - 1.0) * nm) / (4.0 * (double)m)) * pow(d, ((double)(k + 1.0) * n) / m));
			if (upper_bound > qmax) {
				break;
			}

			double theRealVal = 0.0;
			if (k >= 1) {
				double cHat = (double)ceil(tempVal * cHats[k - 1]) / divisor;
				cHats.push_back(cHat);
				theRealVal = cHat / cHats[k - 1];
			}
			else {
				theRealVal = cHats[0];
			}
			//std::cout << "cHat[k-1] / cHat[k] : " << theRealVal << std::endl;

			auto result = CPPMathLibrary::LLL::ReduceBasis_LLL<double>(B, alpha);
			C = std::get<LLL::LLLType::C>(result);

			QSMatrix<int> temp(m, nm, 0); //will store the approximation before being added to the output vector
			for (size_t i = 0; i < m; i++) {
				size_t j = m - i - 1; //need to flip the output matrix
				temp.setRowVector<int>(C.getRowVector<int>(j), i); //set the ith row of temp with the jth row of C

				// multiply the first m columns by cHat[k - 1] / cHat[k]
				std::vector<double> col = B.getColumnVectorAsRow<double>(i);
				col = col * theRealVal;
				B.setColumnVectorFromRow(col, i);
			}
			outputVec.push_back(temp);
		}
		catch (IncorrectDimensionException* idEx) {
			throw idEx;
		}
	}

	return outputVec;
}

double CPPMathLibrary::SimultaneousDiophantine::DirichletCoefficient(const QSMatrix<double>& matrix, const QSMatrix<double>& real_values) {
	size_t m = real_values.get_rows();
	size_t n = real_values.get_cols();

	double dir_coef = 0.0;
	double q = 0.0;

	//calculate the dirichlet coefficient
	for (size_t j = 0; j < matrix.get_rows(); j++) {
		double temp_coef = 0.0;
		double qi = abs(matrix(j, 0));
		double maxDist = 0.0;
		if (qi > q)
			q = qi;

		double tempDist = 0.0;
		for (size_t k = 0; k < n; k++) {
			double x = qi * real_values(j, k);
			tempDist = abs(round(x) - x);
			if (tempDist > maxDist)
				maxDist = tempDist;
		}

		dir_coef += maxDist;
	}

	return dir_coef * pow(q, (double)m / n);
}