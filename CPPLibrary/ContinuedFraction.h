/*
	Author: Joe Ingenito
	Date Created: 7/5/2020
	Description: All algorithms pertaining to the Continued Fraction Expansion of a Real Number.
*/

#pragma once

#include "RationalNumber.h"

namespace CPPMathLibrary {

	// Implements the ContinuedFractionExpansion algorithm, returns an array of partial quotients
	__declspec(dllexport) long* ContinuedFractionExpansion(double gamma, size_t& count, size_t& op_count);

	// Return the convergent calculated by the array of partial quotients up to the stop index
	__declspec(dllexport) RationalNumber FindConvergent(long* qs, size_t stop_index);

	// Print all convergents calculated for the entire array of partial quotients
	__declspec(dllexport) void PrintConvergents(long* qs, size_t count);

}