#include "CPPHeader.h"

int main(int argc, char** argv)
{
	//for now this commented section remains an example of how to use eigen
	/*MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.5;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	cout << m << endl;*/

	double num = 0.0;
	size_t count = 0;
	string filename = "";

	bool server_mode = false; //optimization for server use

	if (argc < 4) {
		cout << "Not enough arguments were given... See instructions below" << endl;
		cout << "1) Number to approximate" << endl;
		cout << "2) Amount of iterations" << endl;
		cout << "3) Name of the output file (must be JSON)" << endl << endl;
		cout << "Modes (optional):" << endl;
		cout << "1) /s - Server Mode" << endl << endl;
		cout << "Stored constants:" << endl;
		cout << "1) pi" << endl;
		cout << "2) e" << endl << endl;
		cout << "Recognized functions (parenthesis may need to be escaped with quotes):" << endl;
		cout << "1) sqrt(x)" << endl;
		cout << "2) log(x)" << endl;
		cout << "3) log_a(b)" << endl;
		return 0;
	}

	string temp_num = *(argv + 1);
	num = tryParseNumFromArgs(temp_num);
	if (num == 0.0) {
		num = tryParseMathFromArgs(temp_num);
		if (num == 0.0 && is_number(temp_num)) {
			num = stod(temp_num);
		}
	}
	if (num == 0.0) {
		cout << "Number was not parsed correctly or 0 was entered." << endl;
		cout << "Check capitalization, and be sure to escape parenthesis with quotes if needed." << endl;
		return 0;
	}

	temp_num = *(argv + 2);
	if (!is_number(temp_num)) {
		cout << "Count is not a number." << endl;
		return 0;
	}
	count = stoi(temp_num);
	if (count > MAXCOUNT) {
		cout << "Count cannot exceed " << MAXCOUNT << endl;
		return 0;
	}

	filename = *(argv + 3);

	if (argc >= 5) {
		string m = *(argv + 4);
		server_mode = m == "/s" || m == "/S";
	}

	size_t op_count = 0;
	long* qs = continuedFractionExpansion(num, count, op_count);
	if (!server_mode) {
		cout << "Operation Count: " << op_count << endl;
		printArray(qs, count);
		printConvergences(qs, count);
	}

	cout << "Writing array to " << filename << endl;
	json j = convertQSequenceToJSON(qs, count, op_count);
	ofstream out(filename);
	out << j << endl;

	cout << "Finished." << endl;
	return 0;
}

double tryParseNumFromArgs(string s) {
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

double tryParseMathFromArgs(string s) {
	size_t pos1 = s.find("(");
	size_t pos2 = s.find(")");
	if (pos1 == string::npos || pos2 == string::npos || pos2 < pos1) { return 0.0; }

	size_t dist = pos2 - pos1 - 1; //length of the substring needed to extract the number
	if (dist == 0) { return 0.0; }

	string temp_num = s.substr(pos1 + 1, dist); //extract the number from the parenthesis
	double num = tryParseNumFromArgs(temp_num);
	if (num == 0.0 && is_number(temp_num)) {
		num = stod(temp_num);
	}

	string m = s.substr(0, pos1); //extract the mode
	if (m == "sqrt" && num >= 0) {
		return sqrt(num);
	}
	else {
		size_t und = s.find("_");
		if (und == string::npos) {
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
