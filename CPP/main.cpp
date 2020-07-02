#include "CPPHeader.h"

int main(int argc, char** argv)
{
	double num = 0.0;
	size_t count = 0;
	std::string filename = "";

	bool server_mode = false; //optimization for server use

	if (argc < 4) {
		std::cout << std::endl;
		std::cout << "Not enough arguments were given... See instructions below" << std::endl;
		std::cout << "1) Number to approximate" << std::endl;
		std::cout << "2) Amount of iterations" << std::endl;
		std::cout << "3) Name of the output file (must be JSON)" << std::endl;
		std::cout << "4) Optional Mode" << std::endl << std::endl;
		std::cout << "Modes (optional):" << std::endl;
		std::cout << "1) /s - Server Mode" << std::endl << std::endl;
		std::cout << "Stored constants:" << std::endl;
		std::cout << "1) pi" << std::endl;
		std::cout << "2) e" << std::endl << std::endl;
		std::cout << "Recognized functions (parenthesis may need to be escaped with quotes):" << std::endl;
		std::cout << "1) sqrt(x)" << std::endl;
		std::cout << "2) log(x)" << std::endl;
		std::cout << "3) log_a(b)" << std::endl << std::endl;
		return 0;
	}

	if (argc >= 5) {
		std::string m = *(argv + 4);
		server_mode = m == "/s" || m == "/S";
	}

	std::string temp_num = *(argv + 1);
	num = CPPMathLibrary::TryParseNumFromArgs(temp_num);
	if (num == 0.0) {
		num = CPPMathLibrary::TryParseMathFromArgs(temp_num);
		if (num == 0.0 && CPPMathLibrary::IsNumeric(temp_num)) {
			num = stod(temp_num);
		}
	}

	if (num == 0) {
		std::cout << "Number was not parsed correctly or 0 was entered." << std::endl;
		std::cout << "Check capitalization, and be sure to escape parenthesis with quotes if needed." << std::endl;
		return 0;
	}

	temp_num = *(argv + 2);
	if (!CPPMathLibrary::IsNumeric(temp_num)) {
		std::cout << "Count is not a number." << std::endl;
		return 0;
	}
	count = stoi(temp_num);
	if (count > MAXCOUNT) {
		std::cout << "Count cannot exceed " << MAXCOUNT << std::endl;
		return 0;
	}

	filename = *(argv + 3);

	size_t op_count = 0;
	long* qs = CPPMathLibrary::ContinuedFractionExpansion(num, count, op_count);
	if (!server_mode) {
		std::cout << "Operation Count: " << op_count << std::endl;
		/*ContinuedFraction::PrintArray(qs, count);
		ContinuedFraction::PrintConvergences(qs, count);*/
		std::cout << "Writing array to " << filename << std::endl;
	}

	json j = convertQSequenceToJSON(qs, count, op_count);
	std::ofstream out(filename);
	out << j << std::endl;

	return 0;
}

json convertQSequenceToJSON(long* qs, size_t count, size_t op_count) {
	long arr[MAXCOUNT] = { 0 };
	for (size_t i = 0; i < count; ++i) {
		long temp = *(qs + i);
		if (temp > 0) {
			arr[i] = temp;
		}
		else {
			break;
		}
	}

	json j;
	j["qSequence"] = arr;
	j["Count"] = count;
	j["OpCount"] = op_count;
	return j;
}
