#include "pch.h"
#include "StringParsing.h"
#include <sstream>

inline bool CPPMathLibrary::StringParsing::IsNumeric(const std::string& s)
{
	return !s.empty() && find_if(s.begin(),
		s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

std::vector<int> CPPMathLibrary::StringParsing::SplitNumericString(const std::string& s) {
	std::vector<int> vect;
	std::stringstream ss(s);

	for (int i; ss >> i;) {
		vect.push_back(i);
		if (ss.peek() == ',')
			ss.ignore();
	}
	return vect;
}

double CPPMathLibrary::StringParsing::TryParseNumFromArgs(std::string s) {
	if (s == "pi") {
		return pi;
	}
	else if (s == "e") {
		return e;
	}
	else if (s == "phi") {
		return phi;
	}
	return 0.0;
}

double CPPMathLibrary::StringParsing::TryParseMathFromArgs(std::string s) {
	size_t pos1 = s.find("(");
	size_t pos2 = s.find(")");
	if (pos1 == std::string::npos || pos2 == std::string::npos || pos2 < pos1) { return 0.0; }

	size_t dist = pos2 - pos1 - 1; //length of the substd::string needed to extract the number
	if (dist == 0) { return 0.0; }

	std::string temp_num = s.substr(pos1 + 1, dist); //extract the number from the parenthesis
	double num = TryParseNumFromArgs(temp_num);
	if (num == 0.0 && IsNumeric(temp_num)) {
		num = stod(temp_num);
	}

	std::string m = s.substr(0, pos1); //extract the mode
	if (m == "sqrt" && num >= 0) {
		return sqrt(num);
	}
	else {
		size_t und = s.find("_");
		if (und == std::string::npos) {
			//still need to check for log
			if (m == "log" && num > 0) {
				return log(num);
			}
		}

		m = s.substr(0, und);
		if (m == "log" && num > 0) {
			dist = pos1 - und - 1;
			temp_num = s.substr(und + 1, dist);
			if (IsNumeric(temp_num)) {
				size_t ind = stoi(temp_num);
				if (ind > 0) {
					return log(num) / log(ind);
				}
			}
		}
	}

	return 0.0;
}

double CPPMathLibrary::StringParsing::ReverseActualBits(size_t num) {
	//algorithm was found here: https://www.geeksforgeeks.org/reverse-actual-bits-given-number/

	size_t result = 0;

	// traversing bits of 'n' from the right 
	while (num > 0)
	{
		// bitwise left shift  
		// 'rev' by 1 
		result <<= 1;

		// if current bit is '1' 
		if ((num & 1) == 1)
			result ^= 1;

		// bitwise right shift  
		// 'n' by 1 
		num >>= 1;

	}

	// required number 
	return result;
}

double CPPMathLibrary::StringParsing::TryParseExpression(std::string s) {
	double num = CPPMathLibrary::StringParsing::TryParseNumFromArgs(s);
	if (num == 0.0) {
		num = CPPMathLibrary::StringParsing::TryParseMathFromArgs(s);
		if (num == 0.0 && CPPMathLibrary::StringParsing::IsNumeric(s)) {
			num = stod(s);
		}
	}
	return num;
}