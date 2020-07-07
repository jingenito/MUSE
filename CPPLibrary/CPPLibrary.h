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

//Managed Code Port
extern "C" __declspec(dllexport) int CPPMathLibrary_ManagedPort_GetGCD(int n1, int n2) {
	return CPPMathLibrary::GreatestCommonDivisor(n1, n2);
}
