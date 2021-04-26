#include "pch.h"
#include "SimultaneousDiophantine.h"
#include "StringParsing.h"
#include <cmath>
#include <stdexcept>
#include <tuple>
#include "AlgorithmsLLL.h"
#include "ContinuedFraction.h"
#include <algorithm>

std::vector<double> ABS(const std::vector<double>& V) {
	std::vector<double> Y(V.size());
	for (size_t i = 0; i < V.size(); i++) {
		Y[i] = std::abs(V[i]);
	}
	return Y;
}

QSMatrix<double> ABS(const QSMatrix<double>& M) {
	QSMatrix<double> Y(M);
	for (size_t i = 0; i < M.get_rows(); i++) {
		for (size_t j = 0; j < M.get_cols(); j++) {
			Y(i, j) = std::abs(M(i, j));
		}
	}
	return Y;
}

QSMatrix<double> ROUND(const QSMatrix<double>& M) {
	QSMatrix<double> Y(M);
	for (size_t i = 0; i < M.get_rows(); i++) {
		for (size_t j = 0; j < M.get_cols(); j++) {
			Y(i, j) = std::round(M(i, j));
		}
	}
	return Y;
}

QSMatrix<double> CPPMathLibrary::SimultaneousDiophantine::SameDivisor(const std::vector<double>& x, const double& alpha, const double& epsilon) throw (IncorrectDimensionException*) {
	size_t n = x.size();
	size_t n1 = n + 1;

	double e = epsilon;
	double beta = 4 / ((4 * alpha) - 1);
	double beta_e = std::pow(beta, -1 * (double)n * n1 / 4) * std::pow(e, n1);

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

	QSMatrix<double> Y(n1, n1, 0.0);
	QSMatrix<double> C(n1, n1, 0.0);
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

QSMatrix<double> CPPMathLibrary::SimultaneousDiophantine::SameDivisorFromRealVector(const std::vector<double>& x, const double& alpha, const double& epsilon, const size_t& quotientCount) throw (IncorrectDimensionException*) {
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

std::vector< QSMatrix<double> > CPPMathLibrary::SimultaneousDiophantine::IteratedLLL_Dyadic(const QSMatrix<double>& matrix, const double& alpha, const double& epsilon, const size_t& qmax, const size_t& M) {
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
	double c = std::pow(std::pow(beta, -1.0 * ((double)nm - 1.0) / 4.0) * epsilon, (double)nm / m);
	double threshold = (std::pow(nm, 2.0) / m) - ((double)nm / m * std::log(epsilon));

	double divisor = std::pow(2, M);
	if (M <= threshold) {
		throw new std::invalid_argument("M <= threshold");
	}
	if (qmax >= divisor) {
		throw new std::invalid_argument("qmax >= divisor");
	}
	//passed threshold test, rationalize c
	c = std::ceil(divisor * c) / divisor;
	std::vector<double> cHats; // need to keep track of cHat at every iteration since they will not be saved in the matrix with this version
	cHats.push_back(c);

	QSMatrix<double> newMatrix(m, n, 0.0);
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++) {
			newMatrix(i, j) = std::ceil(matrix(i, j) * divisor) / divisor;
		}
	}

	QSMatrix<double> B(nm, nm, 0.0); //initialize an (n+m)x(n+m) matrix to all 0s
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

	QSMatrix<double> C(nm, nm, 0.0);
	double d = 1.0 / epsilon;
	int k_prime = (int)std::ceil(((-1.0 * (nm - 1) * nm) / (4.0 * n)) + (((double)m / n) * (std::log(qmax) / std::log(2))));
	double tempVal = divisor * std::pow(d, -1.0 * (double)nm / m);

	std::vector< QSMatrix<double> > outputVec;
	std::vector<double> C_diag;
	std::vector<double> current_row;
	QSMatrix<double> tempMat(m, n + 1, matrix.get_precision(), 0.0); // will store the approximation before being added to the output vector
	std::vector<double> q_vec(m, 0.0); // will store the current q vector to be inserted into tempMat
	std::vector<double> temp_row(n + 1, 0.0); // will store the current row being processed for tempMat

	for (int k = 0; k < k_prime; k++) {
		try {
			double upper_bound = std::abs(std::pow(2, (((double)nm - 1.0) * nm) / (4.0 * (double)m)) * std::pow(d, ((double)(k + 1.0) * n) / m));
			if (upper_bound > qmax) {
				break;
			}

			double theRealVal = 0.0;
			if (k >= 1) {
				double cHat = (double)std::ceil(tempVal * cHats[k - 1]) / divisor;
				cHats.push_back(cHat);
				theRealVal = cHat / cHats[k - 1];
			}
			else {
				theRealVal = cHats[0];
			}
			//std::cout << "cHat[k-1] / cHat[k] : " << theRealVal << std::endl;

			auto result = CPPMathLibrary::LLL::ReduceBasis_LLL<double>(B, alpha);
			C = std::get<LLL::LLLType::C>(result);

			for (size_t i = 0; i < m; i++) {
				size_t j = m - i - 1; //need to flip the output matrix

				// copy the section corresponding to "input" block in B
				current_row = C.getRowVector<double>(j);
				std::copy(current_row.begin() + m, current_row.end(), temp_row.begin() + 1);
				tempMat.setRowVector<double>(temp_row, j);

				// multiply the first m columns of B by cHat[k] / cHat[k - 1]
				QSMatrix<double> col = B.getColumnVector<double>(i);
				col = col * theRealVal;
				B.setColumnVector(col, i);
			}

			// extract the q values from the main diagonal of C - doing this after the loop so they dont get overwritten in tempMat when the row vectors get assigned
			C_diag = C.get_MainDiag(); // the q values live in the main diagonal
			std::copy(C_diag.begin(), C_diag.end() - n, q_vec.begin());
			tempMat.setColumnVectorFromRow(q_vec, 0); // insert the q values into the first column of the output matrix

			outputVec.push_back(tempMat);
		}
		catch (IncorrectDimensionException* idEx) {
			throw idEx;
		}
	}

	return outputVec;
}

double CPPMathLibrary::SimultaneousDiophantine::DirichletCoefficient(const QSMatrix<double>& matrix_LLL, const QSMatrix<double>& real_values) {
	size_t m = real_values.get_rows();
	size_t n = real_values.get_cols();

	if (matrix_LLL.get_rows() != m || matrix_LLL.get_cols() != n + 1)
		throw new IncorrectDimensionException("The LLL matrix should be size m by n + 1 where m is the number of rows and n is the number of columns in real_values.");

	QSMatrix<double> LLL = ABS(matrix_LLL);
	QSMatrix<double> A = ABS(real_values);

	std::vector<double> q_vec = LLL.getColumnVectorAsRow<double>(0); // need an std vector to find max
	double max_q = *std::max_element(q_vec.begin(), q_vec.end());
	QSMatrix<double> q_dbls(q_vec);
	
	QSMatrix<double> result = q_dbls * A;
	std::cout << "Result Vec 1:\n" << result << std::endl;
	result = ABS(result - ROUND(result)); // computes the distance to the nearest integer

	std::vector<double> lin_combs = result.getRowVector<double>(0);
	double max_value = *std::max_element(lin_combs.begin(), lin_combs.end());

	std::cout << "Result Vec 2:\n" << result << std::endl;

	return std::pow(max_q, (double)m / n) * max_value;
}