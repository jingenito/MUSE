/*
	Author: Joe Ingenito
	Date Created: 6/24/2020
	Description: General Algorithms implemented and used by CPPMathLibrary.
*/

#pragma once

namespace CPPMathLibrary {

	// Check if the input double is an integer
	__declspec(dllexport) inline bool IsInt(double x);

	//Implements the Euclidean Algorithm for GCD.
	__declspec(dllexport) size_t GreatestCommonDivisor(const size_t& n1, const size_t& n2);

}