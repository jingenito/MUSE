#pragma once

#include <tuple>
#include <cmath>
#include "QSMatrix.h"
#include "VectorImports.h"
#include "IncorrectDimensionException.h"

namespace CPPMathLibrary {

	//Implements the Euclidean Algorithm for GCD.
	size_t GreatestCommonDivisor(const size_t& n1, const size_t& n2) {
		//the algorithm is technically written with absolute values by Euclid... I'm using
		//size_t for efficiency since the user can input absolute values 
		if (n1 == 0 || n2 == 0) { return 0; }
		size_t r0 = fmax(n1, n2);
		size_t r1 = r0 == n1 ? n2 : n1;
		while (r1 > 0) {
			size_t temp = r1;
			r1 = r0 % r1;
			r0 = temp;
		}
		return r0; //remember it terminates when r1 == 0
	}

	// Indeces for the tuple returned by GramSchmidtOrthogonalization
	enum GSOType { GSO = 0, Mu = 1, Gamma = 2 };

	// Implements the GramSchmidtOrthogonalization for the input matrix, only accepts a square matrix as input. Does not normalize the vectors.
	template <typename T>
	std::tuple< QSMatrix<double>, QSMatrix<double>, std::vector<double> > GramSchmidtOrthogonalization(const QSMatrix<T>& matrix) throw (IncorrectDimensionException*) {
		size_t rows = matrix.get_rows();
		size_t cols = matrix.get_cols();
		if (rows != cols) { throw new IncorrectDimensionException("Matrix is not a square matrix"); }

		QSMatrix<double> result(matrix);
		//skipping over the first row so the intialization has to happen before the loop
		QSMatrix<double> mu(rows, cols, 0);
		std::vector<double> gamma(rows, 0);
		mu(0, 0) = 1;
		gamma[0] = result.getRowVector<double>(0) * result.getRowVector<double>(0);

		try {
			for (size_t i = 1; i < rows; i++) {
				std::vector<double> vec(cols, 0);
				for (size_t j = 0; j < i; j++) {
					double u_ij = result.getRowVector<double>(i) / result.getRowVector<double>(j); //project vector i onto j - see VectorImports.h
					mu(i, j) = u_ij;
					vec = vec + (u_ij * result.getRowVector<double>(j));
				}
				mu(i, i) = 1; //the projection will always be 1 on the main diagonal - vector projected onto itself 
				vec = result.getRowVector<double>(i) - vec;
				result.setRowVector(vec, i);
				gamma[i] = vec * vec;
			}
		}
		catch (IncorrectDimensionException* idEx) {
			throw idEx;
		}

		return std::make_tuple(result, mu, gamma);
	}

	// This method should not be called outside of LLL since it has no meaning outside of LLL
	void _reduce(QSMatrix<double>& y, QSMatrix<double>& mu, std::vector<double>& gamma, QSMatrix<double>& C, const size_t& k, const size_t& l) throw (IncorrectDimensionException*) {
		if (mu.get_rows() != y.get_rows() || mu.get_cols() != y.get_cols()) { throw new IncorrectDimensionException("Matrices mu and y have different dimensions"); }
		if (k >= mu.get_rows() || l >= mu.get_cols()) { throw new IncorrectDimensionException("Index out of range"); }

		try {
			if (abs(mu(k, l)) > 0.5) {
				double nearestInt = int(round(mu(k, l)));

				y.setRowVector<double>(y.getRowVector<double>(k) - (nearestInt * y.getRowVector<double>(l)), k);
				C.setRowVector<double>(C.getRowVector<double>(k) - (nearestInt * C.getRowVector<double>(l)), k);

				for (size_t j = 0; j < l; j++) {
					mu(k, j) = mu(k, j) - (nearestInt * mu(l, j));
				}

				mu(k, l) = mu(k, l) - nearestInt;
			}
		}
		catch (IncorrectDimensionException* idEx) {
			throw idEx;
		}
	}

	// This method should not be called outside of LLL since it has no meaning outside of LLL
	void _exchange(QSMatrix<double>& y, QSMatrix<double>& mu, std::vector<double>& gamma, QSMatrix<double>& C, const size_t& k) {
		if (mu.get_rows() != y.get_rows() || mu.get_cols() != y.get_cols()) { throw new IncorrectDimensionException("Matrices mu and y have different dimensions"); }
		if (k >= mu.get_rows()) { throw new IncorrectDimensionException("Index out of range"); }
		size_t rows = mu.get_rows();

		std::vector<double> z = y.getRowVector<double>(k - 1);
		y.setRowVector(y.getRowVector<double>(k), k - 1);
		y.setRowVector(z, k);

		z = C.getRowVector<double>(k - 1);
		C.setRowVector(C.getRowVector<double>(k), k - 1);
		C.setRowVector(z, k);

		double nu = mu(k, k - 1);
		double delta = gamma[k] + (nu * nu * gamma[k - 1]);
		mu(k, k - 1) = nu * gamma[k - 1] / delta;
		gamma[k] = gamma[k] * gamma[k - 1] / delta;
		gamma[k - 1] = delta;
		for (size_t j = 0; j < k - 1; j++) {
			double t = mu(k - 1, j);
			mu(k - 1, j) = mu(k, j);
			mu(k, j) = t;
		}
		for (size_t i = k + 1; i < rows; i++) {
			double xi = mu(i, k);
			mu(i, k) = mu(i, k - 1) - (nu * mu(i, k));
			mu(i, k - 1) = (mu(k, k - 1) * mu(i, k)) + xi;
		}
	}

	// Indeces for the tuple returned by LLL
	enum LLLType { LLL = 0, C = 1 };

	// Implements the LLL algorithm to find an alpha reduced basis. Input matrix must be square.
	template <typename T>
	std::tuple< QSMatrix<double>, QSMatrix<double> > ReduceBasis_LLL(const QSMatrix<T>& matrix, const double& alpha) throw (IncorrectDimensionException*) {
		size_t rows = matrix.get_rows();
		size_t cols = matrix.get_cols();
		if (rows != cols) { throw new IncorrectDimensionException("Matrix is not a square matrix"); }

		try {
			QSMatrix<double> y(matrix);
			QSMatrix<double> C(QSMatrix<T>::GetIdentityMatrix(rows, cols));
			auto gso = GramSchmidtOrthogonalization<T>(y);
			QSMatrix<double> y_star = std::get<GSOType::GSO>(gso);
			QSMatrix<double> mu = std::get<GSOType::Mu>(gso);
			std::vector<double> gamma = std::get<GSOType::Gamma>(gso);

			size_t k = 1;
			while (k < rows) {
				_reduce(y, mu, gamma, C, k, k - 1);
				double threshold = (alpha - (mu(k, k - 1) * mu(k, k - 1))) * gamma[k - 1];
				if (gamma[k] >= threshold) {
					for (int l = k - 2; l >= 0; l--) {
						_reduce(y, mu, gamma, C, k, l);
					}
					k++;
				}
				else {
					_exchange(y, mu, gamma, C, k);
					if (k > 1)
						k--;
				}
			}

			return std::make_tuple(y, C);
		}
		catch (IncorrectDimensionException* idEx) {
			throw idEx;
		}
	}

}