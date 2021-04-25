#include "pch.h"
#include "AlgorithmsLLL.h"

void CPPMathLibrary::LLL::_reduce(QSMatrix<double>& y, QSMatrix<double>& mu, std::vector<double>& gamma, QSMatrix<int>& C, const size_t& k, const size_t& l) throw (IncorrectDimensionException*) {
	if (mu.get_rows() != y.get_rows() || mu.get_cols() != y.get_cols()) { throw new IncorrectDimensionException("Matrices mu and y have different dimensions"); }
	if (k >= mu.get_rows() || l >= mu.get_cols()) { throw new IncorrectDimensionException("Index out of range"); }

	try {
		if (std::abs(mu(k, l)) > 0.5) {
			double nearestInt = int(std::round(mu(k, l)));

			y.setRowVector<double>(y.getRowVector<double>(k) - (nearestInt * y.getRowVector<double>(l)), k);
			C.setRowVector<int>(C.getRowVector<int>(k) - ((int)nearestInt * C.getRowVector<int>(l)), k);

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

void CPPMathLibrary::LLL::_exchange(QSMatrix<double>& y, QSMatrix<double>& mu, std::vector<double>& gamma, QSMatrix<int>& C, const size_t& k) {
	if (mu.get_rows() != y.get_rows() || mu.get_cols() != y.get_cols()) { throw new IncorrectDimensionException("Matrices mu and y have different dimensions"); }
	if (k >= mu.get_rows()) { throw new IncorrectDimensionException("Index out of range"); }
	size_t rows = mu.get_rows();

	std::vector<double> z = y.getRowVector<double>(k - 1);
	y.setRowVector(y.getRowVector<double>(k), k - 1);
	y.setRowVector(z, k);

	std::vector<int> t = C.getRowVector<int>(k - 1);
	C.setRowVector(C.getRowVector<int>(k), k - 1);
	C.setRowVector(t, k);

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