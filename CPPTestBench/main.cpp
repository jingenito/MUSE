#include <iostream>
#include "CPPLibrary.h"
#include <tuple>
#include <vector>
#include <ctime>

void GramSchmidtTest();
void LLLTest();

int main(int argc, char** argv) {
	std::cout << "CPPLibrary Test Bench" << std::endl << std::endl;

	bool _run_gso_test_ = false, _run_lll_test_ = false;

	if (argc > 2) {
		_run_lll_test_ = argv[1] == "1";
	}
	if (argc > 1) {
		_run_gso_test_ = argv[0] == "1";
	}
	if (argc == 1) {
		std::cout << "No flags have been set... Running all tests..." << std::endl << std::endl;
		_run_gso_test_ = true;
		_run_lll_test_ = true;
	}

	if (_run_gso_test_) {
		GramSchmidtTest();
		std::cout << std::endl;
	}
	if (_run_lll_test_) {
		LLLTest();
		std::cout << std::endl;
	}

	return 0;
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

	start = clock();
	auto result = GramSchmidtOrthogonalization<double>(X);
	end = clock();
	duration = ((double)end - (double)start) / CLOCKS_PER_SEC;

	QSMatrix<double> Y = std::get<GSOType::GSO>(result);
	QSMatrix<double> Mu = std::get<GSOType::Mu>(result);
	std::vector<double> gamma = std::get<GSOType::Gamma>(result);

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

	start = clock();
	auto result = ReduceBasis_LLL<double>(X, 0.75);
	end = clock();
	duration = ((double)end - (double)start) / CLOCKS_PER_SEC;

	QSMatrix<double> Y = std::get<LLLType::LLL>(result);
	QSMatrix<long> C = std::get<LLLType::C>(result);

	std::cout << "Finished... Execution Time " << duration << " seconds" << std::endl << std::endl;
	std::cout << "Y:" << std::endl << Y << std::endl;
	std::cout << "C:" << std::endl << C << std::endl;

	std::cout << "Finished LLL Test." << std::endl;
}