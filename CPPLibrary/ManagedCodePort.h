#pragma once

// This layer is for projects written in managed languages

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_GetGCD(int n1, int n2);

extern "C" __declspec(dllexport) int* CPPMathLibrary_ManagedPort_ContinuedFraction(double gamma, int count);

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_PrintConvergents(int* qs, int count);

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_ReleaseMemoryIntArr(int* pArray);

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_ReleaseMemoryIntPtrPtr(int** arr, int n);

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_ReleaseMemoryDoubleArr(double** arr, int n);

extern "C" __declspec(dllexport) double** CPPMathLibrary_ManagedPort_GramSchmidt(const double** arr, int n);

extern "C" __declspec(dllexport) double** CPPMathLibrary_ManagedPort_ReduceBasis_LLL(const double** arr, int n, double alpha);

extern "C" __declspec(dllexport) double** CPPMathLibrary_ManagedPort_ReduceBasis_LLL_C(const double** arr, int n, double alpha);

extern "C" __declspec(dllexport) int** CPPMathLibrary_ManagedPort_Simultaneous_SameDivisor(const double* arr, int n, double alpha, double epsilon);