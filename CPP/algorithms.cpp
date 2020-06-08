#include <iostream>
#include <cmath> 

bool isInt(double x) { return long long(x) == x; }

long* continuedFractionExpansion(double gamma, unsigned& count, unsigned& op_count) {
	op_count = 0; //clear the op count
	long* arr = new long [count]; //needs to be a signed int because q_i can be negative
	arr[0] = floor(gamma);
	double gamma_n1 = 0;
	for (unsigned i = 1; i < count; ++i) {
		gamma_n1 = 1 / (gamma - arr[i - 1]);
		arr[i] = floor(gamma_n1);
		op_count += 2;
		if (isInt(gamma_n1) && gamma_n1 == arr[i]) {
			count = i - 1; //terminated early so update the count with the last q
			return arr;
		}
		gamma = gamma_n1;
	}
	return arr;
}

//Implements the Euclidean Algorithm for GCD.
unsigned greatestCommonDivisor(unsigned n1, unsigned n2) {
	//the algorithm is technically written with absolute values by Euclid... I'm using
	//unsigned for efficiency since the user can input absolute values 
	if (n1 == 0 || n2 == 0) { return 0; }
	unsigned r0 = fmax(n1, n2);
	unsigned r1 = r0 == n1 ? n2 : n1;
	while (r1 > 0) {
		unsigned temp = r1;
		r1 = r0 % r1;
		r0 = temp;
	}
	return r0; //remember it terminates when r1 == 0
}