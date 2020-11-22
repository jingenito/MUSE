/*
	Author: Joe Ingenito
	Date Created: 6/25/2020
	Description: Provides methods to parse strings.
*/

#pragma once

#include <string>

namespace CPPMathLibrary {

	namespace StringParsing {

		const double pi = 3.14159265358979323846;
		const double e = 2.7182818284590452353602874713527;
		const double phi = 1.61803398874989484820458683436;

		// Returns a boolean indicating whether or not the string contains only numeric characters
		__declspec(dllexport) inline bool IsNumeric(const std::string& s);

		// Attempts to parse the string into either a number, or a recognized constant.
		__declspec(dllexport) double TryParseNumFromArgs(std::string s);

		// Attempts to parse the string to perform a recognized math function, or parse the string into a number.
		__declspec(dllexport) double TryParseMathFromArgs(std::string s);

		// Reverse the bits of an unsigned integer ignoring the leading 0's.
		__declspec(dllexport) double ReverseActualBits(size_t num);

		// Attempts to parse the string of an entire expression
		__declspec(dllexport) double TryParseExpression(std::string s);

	}

}