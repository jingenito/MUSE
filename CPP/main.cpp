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
	unsigned count = 0;
	string filename = "";

	bool server_mode = false; //optimization for server use

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
		server_mode = m == "/s" || m == "/S";
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
