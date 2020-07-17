#include "pch.h"
#include "ContinuedFraction.h"
#include "Algorithms.h"

__declspec(dllexport) int* CPPMathLibrary::ContinuedFraction::ContinuedFractionExpansion(double gamma, size_t& count, size_t& op_count) {
	op_count = 0; //clear the op count
	int* arr = new int[count]; //needs to be a signed int because q_i can be negative
	arr[0] = (int)floor(gamma);
	double gamma_n1 = 0;
	for (size_t i = 1; i < count; ++i) {
		gamma_n1 = 1 / (gamma - arr[i - 1]);
		arr[i] = (int)floor(gamma_n1);
		op_count += 2;
		if (IsInt(gamma_n1) && gamma_n1 == arr[i]) {
			count = i - 1; //terminated early so update the count with the last q
			return arr;
		}
		gamma = gamma_n1;
	}
	return arr;
}

__declspec(dllexport) CPPMathLibrary::RationalNumber CPPMathLibrary::ContinuedFraction::FindConvergent(int* qs, size_t stop_index) {
	if (stop_index < 0) { return RationalNumber(0, 0); /*this is treated as NaN*/ }
	RationalNumber r(*(qs + stop_index));
	if (stop_index == 0) { return r; }
	//needs to be int because size_t can't be negative - so would be an infinite loop
	for (int i = stop_index; i > 0; --i) {
		r.Reciprocal();
		r = r + RationalNumber(*(qs + (i - 1)));
	}
	return r;
}

__declspec(dllexport) void CPPMathLibrary::ContinuedFraction::PrintConvergents(int* qs, size_t count) {
	std::cout << "Convergents:" << std::endl;
	for (size_t i = 0; i < count; ++i) {
		std::cout << i + 1 << ") " << FindConvergent(qs, i) << std::endl;
	}
}