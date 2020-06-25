#include "Algorithms.h"

namespace CPPMathLibrary {

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

	namespace ContinuedFraction {

		inline bool IsInt(double x) { return long(x) == x; }

		long* ContinuedFractionExpansion(double gamma, size_t& count, size_t& op_count) {
			op_count = 0; //clear the op count
			long* arr = new long[count]; //needs to be a signed int because q_i can be negative
			arr[0] = (long)floor(gamma);
			double gamma_n1 = 0;
			for (size_t i = 1; i < count; ++i) {
				gamma_n1 = 1 / (gamma - arr[i - 1]);
				arr[i] = (long)floor(gamma_n1);
				op_count += 2;
				if (IsInt(gamma_n1) && gamma_n1 == arr[i]) {
					count = i - 1; //terminated early so update the count with the last q
					return arr;
				}
				gamma = gamma_n1;
			}
			return arr;
		}

		RationalNumber FindConvergent(long* qs, size_t stop_index) {
			if (stop_index < 0) { return RationalNumber(0, 0); /*this is treated as NaN*/ }
			RationalNumber r(*(qs + stop_index));
			if (stop_index == 0) { return r; }
			//needs to be int because size_t can't be negative - so would be an infinite loop
			for (int i = stop_index; i > 0; --i) {
				r.Reciprocal();
				r = r + RationalNumber(*(qs + (i - 1)));
			}
			return r;
		}

		void PrintConvergents(long* qs, size_t count) {
			std::cout << "Convergences:" << std::endl;
			for (size_t i = 0; i < count; ++i) {
				std::cout << i + 1 << ") " << FindConvergent(qs, i) << std::endl;
			}
		}

	}

}