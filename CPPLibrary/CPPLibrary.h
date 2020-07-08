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

// Managed Code Ports

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_GetGCD(int n1, int n2) {
	return CPPMathLibrary::GreatestCommonDivisor(n1, n2);
}

extern "C" __declspec(dllexport) int* CPPMathLibrary_ManagedPort_ContinuedFraction(double gamma, int& count) {
	size_t theCount = (size_t)count;
	size_t opcount = 0; //not important
	// do NOT delete qs before returning, it is up to the caller using this API to call CPPMathLibrary_ManagedPort_ReleaseMemory
	int* qs = CPPMathLibrary::ContinuedFractionExpansion(gamma, theCount, opcount);
	count = (int)theCount;
	return CPPMathLibrary::ContinuedFractionExpansion(gamma, theCount, opcount);
}

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_PrintConvergents(int* qs, int count) {
	CPPMathLibrary::PrintConvergents(qs, (size_t)count);
	return 0;
}

extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_ReleaseMemory(int* pArray)
{
	delete[] pArray;
	return 0;
}
