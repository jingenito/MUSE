/*
	Author: Joe Ingenito
	Date Created: 06/23/2020
	Description: Include this header file in a cpp file to gain access to the files included below.
*/

#pragma once

#include "IncorrectDimensionException.h"
#include "QSMatrix.h"
#include "VectorImports.h"
#include "RationalNumber.h"
#include "Algorithms.h"
#include "StringParsing.h"
#include "ContinuedFraction.h"
#include "AlgorithmsLLL.h"
#include "SimultaneousDiophantine.h"

// private helper functions

template<typename T>
std::vector<T> ConvertArrToVector(T* arr, size_t n) {
	std::vector<T> v;
	for (size_t i = 0; i < n; i++) {
		v.push_back(*(arr + i));
	}
	return v;
}

template<typename T>
QSMatrix<T> ConvertArrToQSMatrix(T** arr, size_t n) {
	QSMatrix<T> matrix(n, n, 0);
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			matrix(i, j) = *(*(arr + i) + j);
		}
	}
	return matrix;
}

template<typename T>
T** ConvertQSMatrixToArr(QSMatrix<T> matrix) {
	size_t n = matrix.get_rows();
	T** arr = new T * [n];
	for (size_t i = 0; i < n; i++) {
		arr[i] = new T[n];
		for (size_t j = 0; j < n; j++) {
			arr[i][j] = matrix(i, j);
		}
	}
	return arr;
}

// Managed Code Ports

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_GetGCD(int n1, int n2) {
	return CPPMathLibrary::GreatestCommonDivisor(n1, n2);
}

extern "C" __declspec(dllexport) int* CPPMathLibrary_ManagedPort_ContinuedFraction(double gamma, int& count) {
	size_t theCount = (size_t)count;
	size_t opcount = 0; //not important
	// do NOT delete qs before returning, it is up to the caller using this API to call CPPMathLibrary_ManagedPort_ReleaseMemory
	int* qs = CPPMathLibrary::ContinuedFraction::ContinuedFractionExpansion(gamma, theCount, opcount);
	count = (int)theCount;
	return CPPMathLibrary::ContinuedFraction::ContinuedFractionExpansion(gamma, theCount, opcount);
}

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_PrintConvergents(int* qs, int count) {
	CPPMathLibrary::ContinuedFraction::PrintConvergents(qs, (size_t)count);
	return 0;
}

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_ReleaseMemoryIntArr(int* pArray)
{
	delete[] pArray;
	return 0;
}

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_ReleaseMemoryIntPtrPtr(int** arr, int n) {
	for (size_t i = 0; i < n; i++) {
		delete[] * (arr + i);
	}
	delete[] arr;
	return 0;
}

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_ReleaseMemoryDoubleArr(double** arr, int n) {
	for (size_t i = 0; i < n; i++) {
		delete[] *(arr + i);
	}
	delete[] arr;
	return 0;
}

extern "C" __declspec(dllexport) double** CPPMathLibrary_ManagedPort_GramSchmidt(double** arr, int n) throw (IncorrectDimensionException*) {
	if (n < 0)
		throw new IncorrectDimensionException(); // not including a message because the managed code will just get a generic excetion

	QSMatrix<double> matrix = ConvertArrToQSMatrix(arr, (size_t)n);
	auto result = CPPMathLibrary::LLL::GramSchmidtOrthogonalization(matrix);
	QSMatrix<double> gso = std::get<CPPMathLibrary::LLL::GSOType::GSO>(result);
	return ConvertQSMatrixToArr(gso);
}

extern "C" __declspec(dllexport) double** CPPMathLibrary_ManagedPort_ReduceBasis_LLL(double** arr, int n, double alpha) throw (IncorrectDimensionException*) {
	if (n < 0)
		throw new IncorrectDimensionException(); // not including a message because the managed code will just get a generic excetion

	QSMatrix<double> matrix = ConvertArrToQSMatrix(arr, (size_t)n);
	auto result = CPPMathLibrary::LLL::ReduceBasis_LLL(matrix, alpha);
	QSMatrix<double> lll = std::get<CPPMathLibrary::LLL::LLLType::LLL>(result);
	return ConvertQSMatrixToArr(lll);
}

extern "C" __declspec(dllexport) double** CPPMathLibrary_ManagedPort_ReduceBasis_LLL_C(double** arr, int n, double alpha) throw (IncorrectDimensionException*) {
	if (n < 0)
		throw new IncorrectDimensionException(); // not including a message because the managed code will just get a generic excetion

	QSMatrix<double> matrix = ConvertArrToQSMatrix(arr, (size_t)n);
	auto result = CPPMathLibrary::LLL::ReduceBasis_LLL(matrix, alpha);
	QSMatrix<double> c = std::get<CPPMathLibrary::LLL::LLLType::C>(result);
	return ConvertQSMatrixToArr(c);
}

extern "C" __declspec(dllexport) int** CPPMathLibrary_ManagedPort_Simultaneous_SameDivisor(double* arr, int n, double alpha, double epsilon) throw (IncorrectDimensionException*) {
	if (n < 0)
		throw new IncorrectDimensionException(); // not including a message because the managed code will just get a generic excetion

	std::vector<double> x = ConvertArrToVector(arr, n);
	QSMatrix<int> result = CPPMathLibrary::SimultaneousDiophantine::SameDivisor(x, alpha, epsilon);
	return ConvertQSMatrixToArr(result);
}
