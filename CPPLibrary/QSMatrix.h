/*
	Original Source: https://www.quantstart.com/articles/Matrix-Classes-in-C-The-Source-File/ - At this point at least every method has been modified, specifications of authors are now just old artifacts as of 4/24/21
	Modified By: Joe Ingenito
	Date Modified: 6/16/2020
	Description: Provides a class that implements a 2D std::vector<T> as a matrix, and can be used with any type T that can be converted to a double.

				 The precision parameter determines the precision when formatting floating point values, and thus will only affect the formatted precision leaving the original value unmodified.
				 When performing operations between two matrices, the highest value precision will be kept for the resulting matrix except when copying one matrix to another, in which case the values of the coppied matrix are kept.
				 The precision will be lost when the result is a vector instead of a matrix... This will change however when the column vector support is eventually added, and all vectors will be treated as matrices.
*/

#pragma once

#include <ostream>
#include <iomanip>
#include <vector>
#include "IncorrectDimensionException.h"

#define QSMATRIX_DEFAULT_PRECISION 5;

template <typename T>
class QSMatrix {
private:
	std::vector<std::vector<T> > mat;
	size_t rows;
	size_t cols;

	// added by Joe I ================

	size_t precision;

public:
	// constructors ==============================================================

	virtual ~QSMatrix() {}

	QSMatrix(const size_t& _rows, const size_t& _cols, const T& _initial) {
		mat.resize(_rows);
		for (size_t i = 0; i < mat.size(); i++) {
			mat[i].resize(_cols, _initial);
		}
		rows = _rows;
		cols = _cols;
		precision = QSMATRIX_DEFAULT_PRECISION;
	}

	QSMatrix(const QSMatrix<T>& rhs) {
		size_t r = rhs.get_rows();
		size_t c = rhs.get_cols();
		mat.resize(r);
		for (size_t i = 0; i < r; i++) {
			mat[i].resize(c);
			for (size_t j = 0; j < c; j++) {
				mat[i][j] = rhs(i, j);
			}
		}
		rows = r;
		cols = c;
		precision = rhs.get_precision();
	}

	// Modified by Joe I
	template <typename TIn>
	QSMatrix(const QSMatrix<TIn>& rhs) {
		size_t r = rhs.get_rows();
		size_t c = rhs.get_cols();
		mat.resize(r);
		for (size_t i = 0; i < r; i++) {
			mat[i].resize(c);
			for (size_t j = 0; j < c; j++) {
				mat[i][j] = rhs(i, j);
			}
		}
		rows = r;
		cols = c;
		precision = rhs.get_precision();
	}

	// Added by Joe I
	QSMatrix(const T**& arr, const size_t& _rows, const size_t& _cols) {
		precision = QSMATRIX_DEFAULT_PRECISION;
		rows = _rows;
		cols = _cols;
		mat.resize(rows);
		for (size_t i = 0; i < rows; i++) {
			mat[i].resize(cols);
			for (size_t j = 0; j < cols; j++) {
				mat[i][j] = arr[i][j];
			}
		}
	}

	QSMatrix(const T**& arr, const size_t& _rows, const size_t& _cols, const size_t& _precision) {
		precision = _precision;
		rows = _rows;
		cols = _cols;
		mat.resize(rows);
		for (size_t i = 0; i < rows; i++) {
			mat[i].resize(cols);
			for (size_t j = 0; j < cols; j++) {
				mat[i][j] = arr[i][j];
			}
		}
	}

	QSMatrix(const size_t& _rows, const size_t& _cols, const size_t& _precision, const T& _initial) {
		precision = _precision;
		mat.resize(_rows);
		for (size_t i = 0; i < mat.size(); i++) {
			mat[i].resize(_cols, _initial);
		}
		rows = _rows;
		cols = _cols;
	}


	// Get/Set methods ========================================================================================================

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

	// Get the precision that is used for printing floating point values.
	inline size_t get_precision() const { return this->precision; }

	// Set the precision to be used when printing floating point values.
	inline void set_precision(const size_t& _precision) { this->precision = _precision; }

	// Get Row Vector at specified index
	template <typename TOut>
	inline std::vector<TOut> getRowVector(const size_t& i) const {
		if (i >= this->rows) { throw new IncorrectDimensionException("Index Out of Range"); }
		std::vector<TOut> r(this->cols);
		for (size_t j = 0; j < this->cols; j++)
			r[j] = static_cast<TOut>(this->mat[i][j]);
		return r;
	}

	// Set Row Vector at specified index.
	template <typename Tin>
	inline void setRowVector(const std::vector<Tin>& vec, const size_t& i) {
		if (vec.size() > this->cols) { throw new IncorrectDimensionException("Vector length must be less than or equal to the amound of columns."); }
		if (i >= this->rows) { throw new IncorrectDimensionException("Index Out of Range"); }
		for (size_t j = 0; j < vec.size(); j++)
			this->mat[i][j] = vec[j];
	}

	// Get the Column Vector at the specified index as a Row Vector.
	template <typename TOut>
	inline std::vector<TOut> getColumnVectorAsRow(const size_t& j) const {
		if (j >= this->cols) { throw new IncorrectDimensionException("Index Out of Range"); }
		std::vector<TOut> r(this->rows);
		for (size_t i = 0; i < this->rows; i++)
			r[i] = static_cast<TOut>(this->mat[i][j]);
		return r;
	}

	// Set the Column Vector at the specified index from a Row Vector.
	template <typename Tin>
	inline void setColumnVectorFromRow(const std::vector<Tin>& vec, const size_t& j) {
		if (vec.size() > this->rows) { throw new IncorrectDimensionException("Vector length must be less than or equal to the amound of rows."); }
		if (j >= this->cols) { throw new IncorrectDimensionException("Index Out of Range"); }
		for (size_t i = 0; i < vec.size(); i++)
			this->mat[i][j] = vec[i];
	}

	inline static QSMatrix<int> GetIdentityMatrix(const size_t& m, const size_t& n) {
		QSMatrix<int> I(m, n, 0);
		for (size_t k = 0; k < m; k++)
			for (size_t l = 0; l < n; l++)
				I(k, l) = k == l ? 1 : 0;
		return I;
	}
	// ==========================================================================================================================


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
		precision = rhs.get_precision();

		return *this;
	}

	// Matrix mathematical operations                                                                                                                                                                                               
	QSMatrix<T> operator+(const QSMatrix<T>& rhs) {
		QSMatrix result(rows, cols, precision > rhs.get_precision() ? precision : rhs.get_precision(), 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] + rhs(i, j);
			}
		}

		return result;
	}

	QSMatrix<T>& operator+=(const QSMatrix<T>& rhs) {
		precision = precision > rhs.get_precision() ? precision : rhs.get_precision();

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
		QSMatrix<T> result(rows, cols, precision > rhs.get_precision() ? precision : rhs.get_precision(), 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] - rhs(i, j);
			}
		}

		return result;
	}

	QSMatrix<T>& operator-=(const QSMatrix<T>& rhs) {
		precision = precision > rhs.get_precision() ? precision : rhs.get_precision();

		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				this->mat[i][j] -= rhs(i, j);
			}
		}

		return *this;
	}

	QSMatrix<T> operator*(const QSMatrix<T>& rhs) throw (IncorrectDimensionException*) {
		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();

		if (this->cols != rows)
			throw new IncorrectDimensionException("The amount of columns on the left does not equal the amount of rows on the right.");

		QSMatrix<T> result(this->rows, cols, precision > rhs.get_precision() ? precision : rhs.get_precision(), 0.0);

		for (size_t i = 0; i < this->rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				for (size_t k = 0; k < this->cols; k++) {
					result(i, j) += this->mat[i][k] * rhs(k, j);
				}
			}
		}

		return result;
	}

	std::vector<T> operator*(const std::vector<T>& rhs) throw (IncorrectDimensionException*) {
		if (this->cols != rhs.size())
			throw new IncorrectDimensionException("The amount of columns on the left does not equal the amount of rows on the right.");

		std::vector<T> result(rhs.size(), 0.0);

		for (size_t i = 0; i < this->rows; i++) {
			for (size_t j = 0; j < this->cols; j++) {
				result[i] += this->mat[i][j] * rhs[j];
			}
		}

		return result;
	}

	QSMatrix<T>& operator*=(const QSMatrix<T>& rhs) {
		QSMatrix<T> result = (*this) * rhs;
		(*this) = result;
		return *this;
	}

	// Computes the transpose of the current instance.
	QSMatrix<T> transpose() const {
		QSMatrix<T> result(cols, rows, precision, 0.0);

		for (size_t i = 0; i < cols; i++) {
			for (size_t j = 0; j < rows; j++) {
				result(i, j) = static_cast<T>(this->mat[j][i]);
			}
		}

		return result;
	}

	// Matrix/scalar operations                                                                                                                                                                                                     
	QSMatrix<T> operator+(const T& rhs) const {
		QSMatrix<T> result(rows, cols, precision > rhs.get_precision() ? precision : rhs.get_precision(), 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] + rhs;
			}
		}

		return result;
	}

	QSMatrix<T> operator-(const T& rhs) const {
		QSMatrix<T> result(rows, cols, precision > rhs.get_precision() ? precision : rhs.get_precision(), 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] - rhs;
			}
		}

		return result;
	}

	QSMatrix<T> operator*(const T& rhs) const {
		QSMatrix<T> result(rows, cols, precision > rhs.get_precision() ? precision : rhs.get_precision(), 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] * rhs;
			}
		}

		return result;
	}

	QSMatrix<T> operator/(const T& rhs) const {
		QSMatrix<T> result(rows, cols, precision > rhs.get_precision() ? precision : rhs.get_precision(), 0.0);

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				result(i, j) = this->mat[i][j] / rhs;
			}
		}

		return result;
	}

	// Matrix/vector operations                                                                                                                                                                                                     
	//std::vector<T> operator*(const std::vector<T>& rhs); - this method doesnt make sense because these vectors are row vectors and not column vectors - JI 6/16/20 
	// planning on adding column vector support soon - JI 4/24/21

	// Returns the main diagonal as a row vector
	std::vector<T> diag_vec() const {
		std::vector<T> result(this->rows, 0.0);

		for (size_t i = 0; i < this->rows; i++) {
			result[i] = this->mat[i][i];
		}

		return result;
	}

	// ostream operations
	friend std::ostream& operator<<(std::ostream& os, const QSMatrix<T>& rhs) {
		size_t rows = rhs.get_rows();
		size_t cols = rhs.get_cols();
		size_t precision = rhs.get_precision();

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				os << std::fixed << std::setw(precision + 5) << std::setprecision(precision) << rhs(i, j) << " ";
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
		std::vector<T> result(cols, 0);

		for (size_t j = 0; j < cols; j++) {
			for (size_t i = 0; i < rows; i++) {
				result[j] += lhs[i] * rhs(i, j);
			}
		}

		return result;
	}

};
