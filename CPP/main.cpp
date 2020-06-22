#include "CPPHeader.h"

int main(int argc, char** argv)
{
	double num = 0.0;
	size_t count = 0;
	std::string filename = "";

	bool server_mode = false; //optimization for server use

	if (argc < 4) {
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
		std::cout << "3) log_a(b)" << std::endl;
		return 0;
	}

	if (argc >= 5) {
		std::string m = *(argv + 4);
		server_mode = m == "/s" || m == "/S";
	}

	std::string temp_num = *(argv + 1);
	num = tryParseNumFromArgs(temp_num);
	if (num == 0.0) {
		num = tryParseMathFromArgs(temp_num);
		if (num == 0.0 && is_number(temp_num)) {
			num = stod(temp_num);
		}
	}

	if (num == 0) {
		std::cout << "Number was not parsed correctly or 0 was entered." << std::endl;
		std::cout << "Check capitalization, and be sure to escape parenthesis with quotes if needed." << std::endl;
		return 0;
	}

	temp_num = *(argv + 2);
	if (!is_number(temp_num)) {
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
	long* qs = continuedFractionExpansion(num, count, op_count);
	if (!server_mode) {
		std::cout << "Operation Count: " << op_count << std::endl;
		printArray(qs, count);
		printConvergences(qs, count);
		std::cout << "Writing array to " << filename << std::endl;
	}

	json j = convertQSequenceToJSON(qs, count, op_count);
	std::ofstream out(filename);
	out << j << std::endl;

	return 0;
}

double tryParseNumFromArgs(std::string s) {
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

double tryParseMathFromArgs(std::string s) {
	size_t pos1 = s.find("(");
	size_t pos2 = s.find(")");
	if (pos1 == std::string::npos || pos2 == std::string::npos || pos2 < pos1) { return 0.0; }

	size_t dist = pos2 - pos1 - 1; //length of the substd::string needed to extract the number
	if (dist == 0) { return 0.0; }

	std::string temp_num = s.substr(pos1 + 1, dist); //extract the number from the parenthesis
	double num = tryParseNumFromArgs(temp_num);
	if (num == 0.0 && is_number(temp_num)) {
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
			if (is_number(temp_num)) {
				size_t ind = stoi(temp_num);
				if (ind > 0) {
					return log(num) / log(ind);
				}
			}
		}
	}

	return 0.0;
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
