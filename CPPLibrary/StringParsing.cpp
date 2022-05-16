#include "pch.h"
#include "StringParsing.h"
#include <sstream>
#include <stack>

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
		if (num == 0.0) {
			num = stod(s);
		}
	}
	return num;
}

bool CPPMathLibrary::StringParsing::IsOperation(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

std::vector<std::string> CPPMathLibrary::StringParsing::SplitString(std::string s, std::string delimiter) {
	std::vector<std::string> answer;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		answer.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	answer.push_back(s); //need to add the last string
	return answer;
}

double CPPMathLibrary::StringParsing::EvaluatePrefix(std::string exprsn)
{
	//implementation for Prefix Evaluation is found at geeksforgeeks.com
	//https://www.geeksforgeeks.org/evaluation-prefix-expressions/
	std::stack<double> Stack;

	for (int j = exprsn.size() - 1; j >= 0; j--) {

		// if jth character is the delimiter ( which is
		// space in this case) then skip it
		if (exprsn[j] == ' ')
			continue;

		// Push operand to Stack
		// To convert exprsn[j] to digit subtract
		// '0' from exprsn[j].
		// edited by JI: checking for !IsOperation to add support for arbitrary 
		// mathematical stored functions
		if (!IsOperation(exprsn[j])) {

			// there may be more than
			// one digits in a number
			double num = 0, i = j;
			while (j < exprsn.size() && !IsOperation(exprsn[j]))
				j--;
			j++;

			// from [j, i] exprsn contains a number
			// process the substring in the range [j,i]
			std::vector<std::string> strings = CPPMathLibrary::StringParsing::SplitString(exprsn.substr(j, i - j + 1), " ");
			for (std::string s : strings) {
				if (s.find_first_not_of(' ') != std::string::npos)
				{
					// There's a non-space. (Only want to process non-white spaces)
					num = TryParseExpression(s);
					Stack.push(num);
				}
			}
		}
		else {

			// Operator encountered
			// Pop two elements from Stack
			double o1 = Stack.top();
			Stack.pop();
			double o2 = Stack.top();
			Stack.pop();

			// Use switch case to operate on o1
			// and o2 and perform o1 O o2.
			switch (exprsn[j]) {
			case '+':
				Stack.push(o1 + o2);
				break;
			case '-':
				Stack.push(o1 - o2);
				break;
			case '*':
				Stack.push(o1 * o2);
				break;
			case '/':
				Stack.push(o1 / o2);
				break;
			}
		}
	}

	return Stack.top();
}