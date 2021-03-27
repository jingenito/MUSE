#include <iostream>
#include "CPPLibrary.h"
#include <tuple>
#include <vector>
#include <ctime>
#include <cmath>

// the order of bits in input string will be flipped to conform to human readability
// since the bits will be flipped, the first test to run is the most signficant bit and the last test is the least significant bit
const int ITERATED_LLL_STRESS = 1;
const int ITERATED_LLL = ITERATED_LLL_STRESS << 1;
const int SIMULT_DIOPH_REALS = ITERATED_LLL << 1;
const int SIMULT_DIOPH = SIMULT_DIOPH_REALS << 1;
const int LLL = SIMULT_DIOPH << 1;
const int GSO = LLL << 1;
const int CONT_FRAC = GSO << 1;

void ContinuedFractionTest();
void GramSchmidtTest();
void LLLTest();
void SimultaneousDiophantineTest();
void SimultaneousDiophantineFromRealsTest();
void IteratedLLLTest();
void IteratedILLLStressTest();
double GetILLLRandomNumber();
QSMatrix<double> GetILLLRandomizedMatrix(const size_t, const size_t);

int main(int argc, char** argv) {
	std::cout << "CPPLibrary Test Bench" << std::endl << std::endl;

	bool _run_cont_frac_test_ = true, _run_gso_test_ = true, _run_lll_test_ = true, _run_simult_dioph_test_ = true, _run_simult_dioph_reals_test_ = true, _run_iterated_lll_ = true, _run_iterated_lll_stress_ = false;

	if (argc > 1) {
		int input = std::stoi(argv[1], 0, 2);
		input = (int)CPPMathLibrary::StringParsing::ReverseActualBits((size_t)input);
		_run_cont_frac_test_ = (input & CONT_FRAC) == CONT_FRAC;
		_run_gso_test_ = (input & GSO) == GSO;
		_run_lll_test_ = (input & LLL) == LLL;
		_run_simult_dioph_test_ = (input & SIMULT_DIOPH) == SIMULT_DIOPH;
		_run_simult_dioph_reals_test_ = (input & SIMULT_DIOPH_REALS) == SIMULT_DIOPH_REALS;
		_run_iterated_lll_ = (input & ITERATED_LLL) == ITERATED_LLL;
		_run_iterated_lll_stress_ = (input & ITERATED_LLL_STRESS) == ITERATED_LLL_STRESS;
	}
	if (argc == 1) {
		std::cout << "No flags have been set... Running all tests..." << std::endl << std::endl;
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
	if (_run_iterated_lll_) {
		IteratedLLLTest();
		std::cout << std::endl;
	}
	if (_run_iterated_lll_stress_) {
		IteratedILLLStressTest();
		std::cout << std::endl;
	}

	return 0;
}

void ContinuedFractionTest() {
	clock_t start, end;
	double duration;
	double gamma = sqrt(2);
	size_t count = 10, opcount = 0;
	using namespace CPPMathLibrary::ContinuedFraction;

	std::cout << "Starting Continued Fraction Test..." << std::endl;
	std::cout << "Gamma: " << gamma << std::endl;

	start = clock();
	int* qs = ContinuedFractionExpansion(gamma, count, opcount);
	end = clock();
	duration = ((double)end - (double)start) / CLOCKS_PER_SEC;
	std::cout << "Finished... Execution Time " << duration << " seconds" << std::endl << std::endl;
	PrintConvergents(qs, count);
	delete[] qs;
	std::cout << "Finished Continued Fraction Test." << std::endl;
}

void GramSchmidtTest() {
	clock_t start, end;
	double duration;
	using namespace CPPMathLibrary::LLL;

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
	using namespace CPPMathLibrary::LLL;

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

void IteratedLLLTest() {
	clock_t start, end;
	double duration;
	using namespace CPPMathLibrary;

	size_t m = 1, n = 4;

	QSMatrix<double> preValues(m, n, 0);
	preValues(0, 0) = 239.0 / 169;
	preValues(0, 1) = 265.0 / 153;
	preValues(0, 2) = 682.0 / 305;
	preValues(0, 3) = 590.0 / 223;

	std::cout << "Starting Iterated LLL Test..." << std::endl << std::endl;
	std::cout << "Initial Matrix:" << std::endl << preValues << std::endl << std::endl;

	try {
		double epsilon = 1.0 / 2;
		double alpha = 0.75;
		size_t qmax = 20000;

		std::cout << "Epsilon: " << epsilon << " Alpha: " << alpha << " qmax: " << qmax << std::endl << std::endl;

		start = clock();
		std::vector< QSMatrix<int> > result = SimultaneousDiophantine::IteratedLLL(preValues, alpha, epsilon, qmax);
		end = clock();
		duration = ((double)end - (double)start) / CLOCKS_PER_SEC;

		std::cout << "Execution Time " << duration << " seconds" << std::endl << std::endl;

		for (size_t i = 0; i < result.size(); i++) {
			std::cout << "Approximation " << i + 1 << ":" << std::endl;
			std::cout << result[i] << std::endl << std::endl;

			//calculate the dirichlet coefficient
			double dir_coef = SimultaneousDiophantine::DirichletCoefficient(result[i], preValues);

			std::cout << "Dirichlet Coefficient: " << dir_coef << std::endl << std::endl;
		}

	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << idEx->getMessage() << std::endl;
		return;
	}

	std::cout << "Finished Iterated LLL Test." << std::endl;
}

void IteratedILLLStressTest() {
	using namespace CPPMathLibrary::SimultaneousDiophantine;

	clock_t start, end;
	double duration;
	size_t m = 2, n = 2;
	size_t nm = n + m;
	QSMatrix<double> preValues(m, n, 0);

	double epsilon = 1.0 / 2;
	double alpha = 0.75;
	size_t M = 0;
	double val = ((nm * nm) / m) - ((double)nm / m * log(epsilon));
	M = (size_t)ceil(val) + 30;
	size_t qmax = pow(2, M) - 1;
	size_t k_prime = (size_t)ceil(((-1.0 * (nm - 1) * nm) / (4.0 * n)) + (((double)m / n) * (log(qmax) / log(2))));

	std::srand(std::time(nullptr)); // use current time as seed for random generator
	std::cout << "Starting ILLL Dyadic stress test...\n\n M = " << M << "\tQ = " << qmax << "\t k' = " << k_prime << std::endl;

	try {
		preValues = GetILLLRandomizedMatrix(m, n);
		std::cout << "Input Matrix:\n" << preValues << std::endl;

		std::vector< QSMatrix<int> > result = IteratedLLL_Dyadic(preValues, alpha, epsilon, qmax, M);
		for (size_t k = 0; k < result.size(); k++) {
			std::cout << "Result " << k + 1 << ":\n" << result[k] << std::endl;
		}
	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << "Errors occured during the ILLL approximations." << std::endl;
		std::cout << idEx->getMessage() << std::endl;
	}

	std::cout << "Finished test." << std::endl;
}

double GetILLLRandomNumber() {
	double x = ((double)std::rand() / (RAND_MAX)); //random number between 0 and 1
	while (x <= 0.01 || x >= 0.99) {
		x = ((double)std::rand() / (RAND_MAX));
	}
	return x;
}

QSMatrix<double> GetILLLRandomizedMatrix(const size_t m, const size_t n) {
	QSMatrix<double> result = QSMatrix<double>(m, n, 0);
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++) {
			result(i, j) = GetILLLRandomNumber();
		}
	}
	return result;
}