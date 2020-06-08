#include <iostream>
#include <string>
#include <fstream>

#include <Eigen/Dense>
#include <nlohmann/json.hpp>

#include "RationalNumber.h"

using namespace std;

using Eigen::MatrixXd;
using json = nlohmann::json;

const int MAXCOUNT = 100;
const double pi = 3.14159265358979323846;
const double e = 2.7182818284590452353602874713527;
const double phi = 1.61803398874989484820458683436;

extern long* continuedFractionExpansion(double, unsigned&, unsigned&);

void printArray(long*, unsigned);
void printConvergences(long*, unsigned);
RationalNumber findConvergence(long*, int);
json convertQSequenceToJSON(long*, unsigned, unsigned);

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
	unsigned count = 0;
	string filename = "";

	bool server_mode = false; //optimization for server use

	//for now im commenting out the couts, will most likely eventually be moving code to seperate files...
	//one for normal use, and the other to be used by the web service which will have no couts to slow down the performance

	if (argc < 4) {
		cout << "Not enough arguments were given... See instructions below" << endl;
		cout << "1) Number to approximate" << endl;
		cout << "2) Amount of iterations" << endl;
		cout << "3) Name of the output file (must be JSON)" << endl << endl;
		cout << "Stored constants:" << endl;
		cout << "1) pi" << endl;
		cout << "2) e" << endl;
		return 0;
	}

	string temp_num = *(argv + 1);
	if (temp_num == "pi") {
		num = pi;
	}
	else if (temp_num == "e") {
		num = e;
	}
	else if (temp_num == "phi") {
		num = phi;
	}
	else {
		num = stof(temp_num);
	}
	if (num == 0.0) {
		cout << "Number was not parsed correctly or 0 was entered." << endl;
		cout << "Check capitalization if entering a stored value and try again." << endl;
		return 0;
	}

	count = atoi(*(argv + 2)); //rememver this is a char array pointer, not a string
	if (count > MAXCOUNT) {
		cout << "Count cannot exceed " << MAXCOUNT << endl;
		return 0;
	}

	filename = *(argv + 3);

	if (argc >= 5) {
		string m = *(argv + 4);
		server_mode = m == "/s" || "/S";
	}

	unsigned op_count = 0;
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

void printArray(long* arr, unsigned count) {
	cout << "Array:" << endl;
	for (unsigned i = 0; i < count; ++i) {
		cout << *(arr + i) << endl;
	}
}

void printConvergences(long* qs, unsigned count) {
	cout << "Convergences:" << endl;
	for (unsigned i = 0; i < count; ++i) {
		cout << i + 1 << ") " << findConvergence(qs, i) << endl;
	}
}

RationalNumber findConvergence(long* qs, int stop_index) {
	if (stop_index < 0) { return RationalNumber(0, 0); /*this is treated as NaN*/ }
	RationalNumber r(*(qs + stop_index));
	if (stop_index == 0) { return r; }
	for (int i = stop_index; i > 0; --i) {
		r.Reciprocal();
		r = r + RationalNumber(*(qs + (i - 1)));
	}
	return r;
}

json convertQSequenceToJSON(long* qs, unsigned count, unsigned op_count) {
	long arr[MAXCOUNT] = { 0 };
	for (unsigned i = 0; i < count; ++i) {
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
