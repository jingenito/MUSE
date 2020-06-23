/*
	Original Source: https://www.quantstart.com/articles/Matrix-Classes-in-C-The-Source-File/
	Modified By: Joe Ingenito
	Date Modified: 6/16/2020
	Description: Provides a class that implements a 2D std::vector as a matrix.
*/

#pragma once

#include <ostream>
#include <vector>
#include "IncorrectDimensionException.h"

template <typename T>
class QSMatrix {
private:
	std::vector<std::vector<T> > mat;
	size_t rows;
	size_t cols;

public:
	QSMatrix(size_t _rows, size_t _cols, const T& _initial) {
		mat.resize(_rows);
		for (size_t i = 0; i < mat.size(); i++) {
			mat[i].resize(_cols, _initial);
		}
		rows = _rows;
		cols = _cols;
	}

	QSMatrix(const QSMatrix<T>& rhs) {
		mat = rhs.mat;
		rows = rhs.get_rows();
		cols = rhs.get_cols();
	}

	template <typename TIn>
	QSMatrix(const QSMatrix<TIn>& rhs) {
		size_t r = rhs.get_rows();
		size_t c = rhs.get_cols();
		mat.resize(r);
		for (size_t i = 0; i < r; i++) {
			mat[i].resize(c);
			for (size_t j = 0; j < c; j++) {
				mat[i][j] = static_cast<T>(rhs(i, j));
			}
		}
		rows = r;
		cols = c;
	}

	virtual ~QSMatrix() {}

	// Operator overloading, for "standard" mathematical matrix operations                                                                                                                                                          
	QSMatrix<T>& operator=(const QSMatrix<T>& rhs) {
		if (&rhs == this)
			return *this;

		size_t new_rows = rhs.get_rows();
		size_t new_cols = rhs.get_cols();

		mat.resize(new_rows);
		for (size_t i = 0; i < mat.size(); i++) {
			mat[i].resize(new_cols);
		}

		for (size_t i = 0; i < new_rows; i++) {
			for (size_t j = 0; j < new_cols; j++) {
				mat[i][j] = rhs(i, j);
			}
		}
		rows = new_rows;
		cols = new_cols;

		return *this;
	}

	// Matrix mathematical operations                                                                                                                                                                                               
	QSMatrix<T> operator+(const QSMatrix<T>& rhs) {
		QSMatrix result(rows, cols, 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] + rhs(i, j);
			}
		}

		return result;
	}

	QSMatrix<T>& operator+=(const QSMatrix<T>& rhs) {
		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				this->mat[i][j] += rhs(i, j);
			}
		}

		return *this;
	}

	QSMatrix<T> operator-(const QSMatrix<T>& rhs) {
		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();
		QSMatrix<T> result(rows, cols, 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] - rhs(i, j);
			}
		}

		return result;
	}

	QSMatrix<T>& operator-=(const QSMatrix<T>& rhs) {
		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				this->mat[i][j] -= rhs(i, j);
			}
		}

		return *this;
	}

	QSMatrix<T> operator*(const QSMatrix<T>& rhs) {
		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();
		QSMatrix<T> result(rows, cols, 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				for (size_t k = 0; k < rows; k++) {
					result(i, j) += this->mat[i][k] * rhs(k, j);
				}
			}
		}

		return result;
	}

	QSMatrix<T>& operator*=(const QSMatrix<T>& rhs) {
		QSMatrix<T> result = (*this) * rhs;
		(*this) = result;
		return *this;
	}

	QSMatrix<T> transpose() {
		QSMatrix<T> result(rows, cols, 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[j][i];
			}
		}

		return result;
	}

	// Matrix/scalar operations                                                                                                                                                                                                     
	QSMatrix<T> operator+(const T& rhs) {
		QSMatrix<T> result(rows, cols, 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] + rhs;
			}
		}

		return result;
	}

	QSMatrix<T> operator-(const T& rhs) {
		QSMatrix<T> result(rows, cols, 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] - rhs;
			}
		}

		return result;
	}

	QSMatrix<T> operator*(const T& rhs) {
		QSMatrix<T> result(rows, cols, 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] * rhs;
			}
		}

		return result;
	}

	QSMatrix<T> operator/(const T& rhs) {
		QSMatrix<T> result(rows, cols, 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] / rhs;
			}
		}

		return result;
	}

	// Matrix/vector operations                                                                                                                                                                                                     
	//std::vector<T> operator*(const std::vector<T>& rhs); - this method doesnt make sense because these vectors are row vectors and not column vectors - JI

	// Returns the main diagonal as a row vector
	std::vector<T> diag_vec() {
		std::vector<T> result(rows, 0.0);

		for (size_t i = 0; i < rows; i++) {
			result[i] = this->mat[i][i];
		}

		return result;
	}

	// Access the individual elements                                                                                                                                                                                               
	inline T& operator()(const size_t& row, const size_t& col) {
		return this->mat[row][col];
	}

	inline const T& operator()(const size_t& row, const size_t& col) const {
		return this->mat[row][col];
	}

	// Access the row and column sizes                                                                                                                                                                                              
	inline size_t get_rows() const {
		return this->rows;
	}

	inline size_t get_cols() const {
		return this->cols;
	}

	/*
	------------------------------------------------------------------------------------------------------------
	This section and below is added functionality to the implementation found at the link at the top of the file
	*/

	// ostream operations
	friend std::ostream& operator<<(std::ostream& os, const QSMatrix<T>& rhs) {
		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				os << rhs(i, j) << ", ";
			}
			os << std::endl;
		}

		return os;
	}

	// Vector * Matrix
	friend std::vector<T> operator*(const std::vector<T>& lhs, const QSMatrix<T>& rhs) throw (IncorrectDimensionException*) {
		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();
		if (lhs.size() != rows) { throw new IncorrectDimensionException("Dimension of the left side must equal the number of rows"); }
		std::vector<T> result(lhs.size(), 0);

		for (size_t j = 0; j < cols; j++) {
			for (size_t i = 0; i < rows; i++) {
				result[j] += lhs[j] * rhs(i, j);
			}
		}

		return result;
	}

	// Get Row Vector at specified index
	template <typename TOut>
	inline std::vector<TOut> getRowVector(const size_t& i) throw (IncorrectDimensionException*) {
		if (i >= this->mat.size()) { throw new IncorrectDimensionException("Index Out of Range"); }
		std::vector<TOut> r(this->cols);
		for (size_t j = 0; j < this->cols; j++)
			r[j] = static_cast<TOut>(this->mat[i][j]);
		return r;
	}

	// Set Row Vector at specified index
	template <typename Tin>
	inline void setRowVector(const std::vector<Tin>& vec, const size_t& i) throw (IncorrectDimensionException*) {
		if (i >= this->mat.size()) { throw new IncorrectDimensionException("Index Out of Range"); }
		for (size_t j = 0; j < vec.size(); j++)
			this->mat[i][j] = vec[j];
	}

	inline static QSMatrix<int> GetIdentityMatrix(const size_t& m, const size_t& n) {
		QSMatrix<int> i(m, n, 0);
		for (size_t k = 0; k < m; k++)
			for (size_t l = 0; l < n; l++)
				i(k, l) = k == l ? 1 : 0;
		return i;
	}
};
