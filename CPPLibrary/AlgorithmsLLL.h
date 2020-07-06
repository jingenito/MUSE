/*
	Author: Joe Ingenito
	Date Created: 7/5/2020
	Description: All algorithms pertaining to implementing the LLL Algorithm.
	Remarks: Definitions to functions declared with a template argument are provided by this file, definitions that do not require a template
			 argument are provided by AlgorithmsLLL.cpp
*/

#pragma once

#include <tuple>
#include <cmath>
#include "QSMatrix.h"
#include "VectorImports.h"
#include "IncorrectDimensionException.h"
#include "RationalNumber.h"

namespace CPPMathLibrary {

	// Indeces for the tuple returned by GramSchmidtOrthogonalization
	enum GSOType { GSO = 0, Mu = 1, Gamma = 2 };

	// Implements the GramSchmidtOrthogonalization for the input matrix, only accepts a square matrix as input. Does not normalize the vectors.
	template <typename T>
	__declspec(dllexport) std::tuple< QSMatrix<double>, QSMatrix<double>, std::vector<double> > GramSchmidtOrthogonalization(const QSMatrix<T>& matrix) throw (IncorrectDimensionException*) {
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
	__declspec(dllexport) void _reduce(QSMatrix<double>& y, QSMatrix<double>& mu, std::vector<double>& gamma, QSMatrix<double>& C, const size_t& k, const size_t& l) throw (IncorrectDimensionException*);

	// This method should not be called outside of LLL since it has no meaning outside of LLL
	__declspec(dllexport) void _exchange(QSMatrix<double>& y, QSMatrix<double>& mu, std::vector<double>& gamma, QSMatrix<double>& C, const size_t& k);

	// Indeces for the tuple returned by LLL
	enum LLLType { LLL = 0, C = 1 };

	// Implements the LLL algorithm to find an alpha reduced basis. Input matrix must be square.
	template <typename T>
	__declspec(dllexport) std::tuple< QSMatrix<double>, QSMatrix<double> > ReduceBasis_LLL(const QSMatrix<T>& matrix, const double& alpha) throw (IncorrectDimensionException*) {
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