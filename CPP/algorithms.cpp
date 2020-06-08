#include <cmath> 

//Implements the Euclidean Algorithm for GCD.
size_t greatestCommonDivisor(size_t n1, size_t n2) {
	//the algorithm is technically written with absolute values by Euclid... I'm using
	//size_t for efficiency since the user can input absolute values 
	if (n1 == 0 || n2 == 0) { return 0; }
	size_t r0 = fmax(n1, n2);
	size_t r1 = r0 == n1 ? n2 : n1;
	while (r1 > 0) {
		size_t temp = r1;
		r1 = r0 % r1;
		r0 = temp;
	}
	return r0; //remember it terminates when r1 == 0
}