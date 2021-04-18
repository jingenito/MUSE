#include "pch.h"
#include "ManagedCodePort.h"

#include "IncorrectDimensionException.h"
#include "QSMatrix.h"
#include "ContinuedFraction.h"
#include "AlgorithmsLLL.h"
#include "SimultaneousDiophantine.h"

// helper functions

template<typename T>
std::vector<T> ConvertArrToVector(const T*& arr, const size_t n) {
	std::vector<T> v;
	for (size_t i = 0; i < n; i++) {
		v.push_back(*(arr + i));
	}
	return v;
}

template<typename T>
T** ConvertQSMatrixToArr(const QSMatrix<T>& matrix) {
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

// managed code layer implementations

int CPPMathLibrary_ManagedPort_GetGCD(int n1, int n2) {
	return CPPMathLibrary::GreatestCommonDivisor(n1, n2);
}

int* CPPMathLibrary_ManagedPort_ContinuedFraction(double gamma, int count) {
	size_t theCount = (size_t)count;
	size_t opcount = 0; //not important
	// do NOT delete qs before returning, it is up to the caller using this API to call CPPMathLibrary_ManagedPort_ReleaseMemory
	int* qs = CPPMathLibrary::ContinuedFraction::ContinuedFractionExpansion(gamma, theCount, opcount);
	return qs;
}

int CPPMathLibrary_ManagedPort_PrintConvergents(int* qs, int count) {
	CPPMathLibrary::ContinuedFraction::PrintConvergents(qs, (size_t)count);
	return 0;
}

int CPPMathLibrary_ManagedPort_ReleaseMemoryIntArr(int* pArray)
{
	delete[] pArray;
	return 0;
}

int CPPMathLibrary_ManagedPort_ReleaseMemoryIntPtrPtr(int** arr, int n) {
	for (size_t i = 0; i < n; i++) {
		delete[] * (arr + i);
	}
	delete[] arr;
	return 0;
}

int CPPMathLibrary_ManagedPort_ReleaseMemoryDoubleArr(double** arr, int n) {
	for (size_t i = 0; i < n; i++) {
		delete[] * (arr + i);
	}
	delete[] arr;
	return 0;
}

double** CPPMathLibrary_ManagedPort_GramSchmidt(const double** arr, int n) {
	if (n < 0)
		throw new IncorrectDimensionException(); // not including a message because the managed code will just get a generic excetion

	QSMatrix<double> matrix(arr, (size_t)n, (size_t)n);
	auto result = CPPMathLibrary::LLL::GramSchmidtOrthogonalization(matrix);
	QSMatrix<double> gso = std::get<CPPMathLibrary::LLL::GSOType::GSO>(result);
	return ConvertQSMatrixToArr(gso);
}

double** CPPMathLibrary_ManagedPort_ReduceBasis_LLL(const double** arr, int n, double alpha) {
	if (n < 0)
		throw new IncorrectDimensionException(); // not including a message because the managed code will just get a generic excetion

	QSMatrix<double> matrix(arr, (size_t)n, (size_t)n);
	auto result = CPPMathLibrary::LLL::ReduceBasis_LLL(matrix, alpha);
	QSMatrix<double> lll = std::get<CPPMathLibrary::LLL::LLLType::LLL>(result);
	return ConvertQSMatrixToArr(lll);
}

double** CPPMathLibrary_ManagedPort_ReduceBasis_LLL_C(const double** arr, int n, double alpha) {
	if (n < 0)
		throw new IncorrectDimensionException(); // not including a message because the managed code will just get a generic excetion

	QSMatrix<double> matrix(arr, (size_t)n, (size_t)n);
	auto result = CPPMathLibrary::LLL::ReduceBasis_LLL(matrix, alpha);
	QSMatrix<double> c = std::get<CPPMathLibrary::LLL::LLLType::C>(result);
	return ConvertQSMatrixToArr(c);
}

int** CPPMathLibrary_ManagedPort_Simultaneous_SameDivisor(const double* arr, int n, double alpha, double epsilon) {
	if (n < 0)
		throw new IncorrectDimensionException(); // not including a message because the managed code will just get a generic excetion

	std::vector<double> x = ConvertArrToVector(arr, n);
	QSMatrix<int> result = CPPMathLibrary::SimultaneousDiophantine::SameDivisor(x, alpha, epsilon);
	return ConvertQSMatrixToArr(result);
}