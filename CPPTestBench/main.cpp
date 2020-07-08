#include <iostream>
#include "CPPLibrary.h"
#include <tuple>
#include <vector>
#include <ctime>
#include <cmath>

void ContinuedFractionTest();
void GramSchmidtTest();
void LLLTest();
void SimultaneousDiophantineTest();
void SimultaneousDiophantineFromRealsTest();

int main(int argc, char** argv) {
	std::cout << "CPPLibrary Test Bench" << std::endl << std::endl;

	bool _run_cont_frac_test_ = false, _run_gso_test_ = false, _run_lll_test_ = false, _run_simult_dioph_test_ = false, _run_simult_dioph_reals_test_ = false;

	if (argc > 5) {
		_run_simult_dioph_reals_test_ = argv[5] == "1";
	}
	if (argc > 4) {
		_run_simult_dioph_test_ = argv[4] == "1";
	}
	if (argc > 3) {
		_run_lll_test_ = argv[3] == "1";
	}
	if (argc > 2) {
		_run_gso_test_ = argv[2] == "1";
	}
	if (argc > 1) {
		_run_cont_frac_test_ = argv[1] == "1";
	}
	if (argc == 1) {
		std::cout << "No flags have been set... Running all tests..." << std::endl << std::endl;
		_run_gso_test_ = true;
		_run_lll_test_ = true;
		_run_simult_dioph_test_ = true;
		_run_simult_dioph_reals_test_ = true;
		_run_cont_frac_test_ = true;
	}

	if (_run_cont_frac_test_) {
		ContinuedFractionTest();
		std::cout << std::endl;
	}
	if (_run_gso_test_) {
		GramSchmidtTest();
		std::cout << std::endl;
	}
	if (_run_lll_test_) {
		LLLTest();
		std::cout << std::endl;
	}
	if (_run_simult_dioph_test_) {
		SimultaneousDiophantineTest();
		std::cout << std::endl;
	}
	if (_run_simult_dioph_reals_test_) {
		SimultaneousDiophantineFromRealsTest();
		std::cout << std::endl;
	}

	return 0;
}

void ContinuedFractionTest() {
	clock_t start, end;
	double duration;
	double gamma = sqrt(2);
	size_t count = 10, opcount = 0;
	using namespace CPPMathLibrary;

	std::cout << "Starting Continued Fraction Test..." << std::endl;
	std::cout << "Gamma: " << gamma << std::endl;

	start = clock();
	int* qs = ContinuedFractionExpansion(gamma, count, opcount);
	end = clock();
	duration = ((double)end - (double)start) / CLOCKS_PER_SEC;
	std::cout << "Finished... Execution Time " << duration << " seconds" << std::endl << std::endl;
	PrintConvergents(qs, count);
	std::cout << "Finished Continued Fraction Test." << std::endl;
}

void GramSchmidtTest() {
	clock_t start, end;
	double duration;
	using namespace CPPMathLibrary;

	QSMatrix<double> X(3, 3, 0);
	X(0, 0) = 3;
	X(0, 1) = -1;
	X(0, 2) = 5;
	X(1, 0) = -5;
	X(1, 1) = 2;
	X(1, 2) = -1;
	X(2, 0) = -3;
	X(2, 1) = 9;
	X(2, 2) = 2;

	std::cout << "Starting GSO Test..." << std::endl;
	std::cout << "Matrix X:" << std::endl << X << std::endl;

	QSMatrix<double> Y(3, 3, 0);
	QSMatrix<double> Mu(3, 3, 0);
	std::vector<double> gamma(3);

	try {
		start = clock();
		auto result = GramSchmidtOrthogonalization<double>(X);
		end = clock();
		duration = ((double)end - (double)start) / CLOCKS_PER_SEC;

		Y = std::get<GSOType::GSO>(result);
		Mu = std::get<GSOType::Mu>(result);
		gamma = std::get<GSOType::Gamma>(result);
	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << idEx->getMessage() << std::endl;
		return;
	}

	std::cout << "Finished... Execution Time " << duration << " seconds" << std::endl << std::endl;
	std::cout << "Y:" << std::endl << Y << std::endl;
	std::cout << "Mu:" << std::endl << Mu << std::endl;
	std::cout << "Gamma:" << std::endl << gamma << std::endl << std::endl;

	std::cout << "Finished GSO Test." << std::endl;
}

void LLLTest() {
	clock_t start, end;
	double duration;
	using namespace CPPMathLibrary;

	QSMatrix<double> X(4, 4, 0);
	X(0, 0) = -2;
	X(0, 1) = 7;
	X(0, 2) = 7;
	X(0, 3) = -5;
	X(1, 0) = 3;
	X(1, 1) = -2;
	X(1, 2) = 6;
	X(1, 3) = -1;
	X(2, 0) = 2;
	X(2, 1) = -8;
	X(2, 2) = -9;
	X(2, 3) = -7;
	X(3, 0) = 8;
	X(3, 1) = -9;
	X(3, 2) = 6;
	X(3, 3) = -4;

	std::cout << "Starting LLL Test..." << std::endl << "Alpha = 0.75" << std::endl;
	std::cout << "Matrix X:" << std::endl << X << std::endl;

	QSMatrix<double> Y(4, 4, 0);
	QSMatrix<int> C(4, 4, 0);

	try {
		start = clock();
		auto result = ReduceBasis_LLL<double>(X, 0.75);
		end = clock();
		duration = ((double)end - (double)start) / CLOCKS_PER_SEC;

		Y = std::get<LLLType::LLL>(result);
		C = std::get<LLLType::C>(result);
	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << idEx->getMessage() << std::endl;
		return;
	}

	std::cout << "Finished... Execution Time " << duration << " seconds" << std::endl << std::endl;
	std::cout << "Y:" << std::endl << Y << std::endl;
	std::cout << "C:" << std::endl << C << std::endl;

	std::cout << "Finished LLL Test." << std::endl;
}

void SimultaneousDiophantineTest() {
	clock_t start, end;
	double duration;
	using namespace CPPMathLibrary;

	std::vector<double> preValues;
	preValues.push_back((double)239 / 169);
	preValues.push_back((double)265 / 153);
	preValues.push_back((double)682 / 305);
	preValues.push_back((double)590 / 223);

	std::cout << "Starting Simultaneous Diophantine Test..." << std::endl << std::endl;
	std::cout << "Initial Vector:" << std::endl << preValues << std::endl << std::endl;

	try {
		double epsilon = 1;
		double alpha = 0.75;
		for (size_t i = 0; i < 3; i++) {
			epsilon /= 10;

			std::cout << "Epsilon: " << epsilon << " Alpha: " << alpha << std::endl << std::endl;

			start = clock();
			QSMatrix<int> C = SimultaneousDiophantine::SameDivisor(preValues, alpha, epsilon);
			end = clock();
			duration = ((double)end - (double)start) / CLOCKS_PER_SEC;

			std::cout << "Execution Time " << duration << " seconds" << std::endl << std::endl;
			std::cout << "C:" << std::endl << C << std::endl << std::endl;
		}
	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << idEx->getMessage() << std::endl;
		return;
	}

	std::cout << "Finished Simultaneous Diophantine Test." << std::endl;
}

void SimultaneousDiophantineFromRealsTest() {
	clock_t start, end;
	double duration;
	using namespace CPPMathLibrary;

	std::vector<double> preValues;
	preValues.push_back(sqrt(2));
	preValues.push_back(sqrt(3));
	preValues.push_back(sqrt(5));
	preValues.push_back(sqrt(7));

	std::cout << "Starting Simultaneous Diophantine From Real Vector Test..." << std::endl << std::endl;
	std::cout << "Initial Vector:" << std::endl << preValues << std::endl << std::endl;

	try {
		double epsilon = 1;
		double alpha = 0.75;
		size_t pqCount = 5;
		for (size_t i = 0; i < 3; i++) {
			epsilon /= 10;

			std::cout << "Epsilon: " << epsilon << " Alpha: " << alpha << " Partial Quotient Count: " << pqCount << std::endl << std::endl;

			start = clock();
			QSMatrix<int> C = SimultaneousDiophantine::SameDivisorFromRealVector(preValues, alpha, epsilon, pqCount);
			end = clock();
			duration = ((double)end - (double)start) / CLOCKS_PER_SEC;

			std::cout << "Execution Time " << duration << " seconds" << std::endl << std::endl;
			std::cout << "C:" << std::endl << C << std::endl << std::endl;
		}
	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << idEx->getMessage() << std::endl;
		return;
	}

	std::cout << "Finished Simultaneous Diophantine From Real Vector Test." << std::endl;
}