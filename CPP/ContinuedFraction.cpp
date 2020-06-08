#include "RationalNumber.h"
#include <cmath> 

using namespace std;

bool isInt(double x) { return long (x) == x; }

long* continuedFractionExpansion(double gamma, unsigned& count, unsigned& op_count) {
	op_count = 0; //clear the op count
	long* arr = new long[count]; //needs to be a signed int because q_i can be negative
	arr[0] = (long)floor(gamma);
	double gamma_n1 = 0;
	for (unsigned i = 1; i < count; ++i) {
		gamma_n1 = 1 / (gamma - arr[i - 1]);
		arr[i] = (long)floor(gamma_n1);
		op_count += 2;
		if (isInt(gamma_n1) && gamma_n1 == arr[i]) {
			count = i - 1; //terminated early so update the count with the last q
			return arr;
		}
		gamma = gamma_n1;
	}
	return arr;
}

void printArray(long* arr, unsigned count) {
	cout << "Array:" << endl;
	for (unsigned i = 0; i < count; ++i) {
		cout << *(arr + i) << endl;
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

void printConvergences(long* qs, unsigned count) {
	cout << "Convergences:" << endl;
	for (unsigned i = 0; i < count; ++i) {
		cout << i + 1 << ") " << findConvergence(qs, i) << endl;
	}
}