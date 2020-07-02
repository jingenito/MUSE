/*
	Author: Joe Ingenito
	Date Created: 6/16/2020
	Description: Provides a library of overloads for std::vector operations so it can be used like a row vector.
*/

#pragma once

#include <cmath>
#include <iomanip>
#include <ostream>
#include <vector>
#include "IncorrectDimensionException.h"

// Vector Addition
template <typename T>
__declspec(dllexport) inline std::vector<T> operator+(const std::vector<T>& lhs, const std::vector<T>& rhs) throw (IncorrectDimensionException*) {
	if (lhs.size() != rhs.size()) { throw new IncorrectDimensionException("Dimension of the left side must equal the dimension of the right side"); }
	std::vector<T> result(lhs.size(), 0);

	for (size_t i = 0; i < lhs.size(); i++) {
		result[i] = lhs[i] + rhs[i];
	}

	return result;
}

// Vector Subtraction
template <typename T>
__declspec(dllexport) inline std::vector<T> operator-(const std::vector<T>& lhs, const std::vector<T>& rhs) throw (IncorrectDimensionException*) {
	if (lhs.size() != rhs.size()) { throw new IncorrectDimensionException("Dimension of the left side must equal the dimension of the right side"); }
	std::vector<T> result(lhs.size(), 0);

	for (size_t i = 0; i < lhs.size(); i++) {
		result[i] = lhs[i] - rhs[i];
	}

	return result;
}

// Vector Dot Product - Using Euclidean Dot Product
template <typename T>
__declspec(dllexport) inline T operator*(const std::vector<T>& lhs, const std::vector<T>& rhs) throw (IncorrectDimensionException*) {
	if (lhs.size() != rhs.size()) { throw new IncorrectDimensionException("Dimension of the left side must equal the dimension of the right side"); }
	T sum = 0;

	for (size_t i = 0; i < lhs.size(); i++) {
		sum = sum + (lhs[i] * rhs[i]);
	}

	return sum;
}

// Scalar Multiplication
template <typename T>
__declspec(dllexport) inline std::vector<T> operator*(const std::vector<T>& lhs, const T& rhs) {
	std::vector<T> result(lhs.size(), 0);

	for (size_t i = 0; i < lhs.size(); i++) {
		result[i] = lhs[i] * rhs;
	}

	return result;
}

template <typename T>
__declspec(dllexport) inline std::vector<T> operator*(const T& lhs, const std::vector<T>& rhs) {
	return rhs * lhs;
}

// Project the left vector onto the right vector
template <typename T>
__declspec(dllexport) inline T operator/(const std::vector<T>& lhs, const std::vector<T>& rhs) throw (IncorrectDimensionException*) {
	try {
		return (lhs * rhs) / (rhs * rhs);
	}
	catch (IncorrectDimensionException* idEx) {
		throw idEx;
	}
}

// Magnitude of Vector - Using Euclidean Distance
template <typename T>
__declspec(dllexport) inline std::vector<T> get_magnitude(const std::vector<T>& vec) {
	return sqrt(vec * vec);
}

// Print the Vector
template <typename T>
__declspec(dllexport) inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& rhs) {
	for (size_t i = 0; i < rhs.size(); i++) {
		os << std::setw(8) << std::setprecision(5) << rhs[i] << " ";
	}

	return os;
}
