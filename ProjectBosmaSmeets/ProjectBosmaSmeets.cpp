#include <iostream>
#include "CPPLibrary.h"
#include <fstream>
#include <tuple>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <string>

using namespace CPPMathLibrary;
using json = nlohmann::json;

std::vector<double> DiscritizeInterval(double start, double end, double delta);
double SearchVectorDist(QSMatrix<double> vec, double value);
double F(double z);
json convertJSONData(std::vector<double> z_vector, std::vector<double> vec);
double GetILLLRandomNumber();
QSMatrix<double> GetILLLRandomizedMatrix(const size_t, const size_t);

const int ITERATIONS = 1;

int main(int argc, char** argv)
{
	std::srand(std::time(nullptr)); // use current time as seed for random generator

	size_t N = ITERATIONS;
	size_t m = 3, n = 2;
	size_t nm = n + m;
	double deltaX = 1.0 / 1000;

	QSMatrix<double> preValues(m, n, 0);

	double epsilon = 1.0 / 2;
	double alpha = 0.75;

	size_t M = 0;
	double val = (pow(nm, 2) / m) - ((double)nm / m * log(epsilon));
	M = (size_t)ceil(val) + 30;
	size_t qmax = pow(2, M) - 1;
	size_t k_prime = (size_t)ceil((-1.0 * (nm - 1) * nm) / (4.0 * n) + (m * log(qmax) / (log(2) * n)));

	QSMatrix<double> dir_coefs(N, k_prime, 0);
	QSMatrix<double> dir_coefs_nonrepeated(N, k_prime, 0);

	std::cout << "Starting " << N << " ILLL approximations..." << std::endl;

	try {
		std::vector<int> current_row;

		for (size_t i = 0; i < N; i++) {
			preValues = GetILLLRandomizedMatrix(m, n);
			
			std::vector< QSMatrix<int> > result = SimultaneousDiophantine::IteratedLLL_Dyadic(preValues, alpha, epsilon, qmax, M);
			QSMatrix<double> all_values(m, result.size(), 0);

			for (size_t k = 0; k < result.size(); k++) {
				std::cout << "Result " << k + 1 << ":\n" << result[k] << std::endl;

				for (size_t j = 0; j < result[k].get_rows(); j++) {
					all_values(j, k) = result[k](j, 0);
					current_row = result[k].getRowVector<int>(j);

					double dir_coef = SimultaneousDiophantine::DirichletCoefficient(result[k], preValues);
					if (std::find(current_row.begin(), current_row.end(), all_values(j, k)) == current_row.end()) {
						dir_coefs_nonrepeated(i, k) = dir_coef;
					}
					else {
						dir_coefs_nonrepeated(i, k) = 0;
					}
					dir_coefs(i, k) = dir_coef;
				}

			}
		}
	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << "Errors occured during the ILLL approximations." << std::endl;
		std::cout << idEx->getMessage() << std::endl;
		return 0;
	}

	double delta = 0.0125;
	std::cout << "Finished ILLL approximations." << std::endl;
	std::cout << "Discritizing the interval [0,1] by delta = " << delta << std::endl;
	std::vector<double> z_vector = DiscritizeInterval(0, 0.5, delta);
	std::cout << "Processing dirichlet coefficients..." << std::endl;

	std::vector<double> freqs;
	std::vector<double> freqs_nonrepeated;
	std::vector<double> f_vector;

	for (size_t k = 0; k < z_vector.size(); k++) {
		double z = z_vector[k];
		freqs.push_back(SearchVectorDist(dir_coefs, z));
		freqs_nonrepeated.push_back(SearchVectorDist(dir_coefs_nonrepeated, z));
		f_vector.push_back(F(z));
	}

	std::cout << "Finished processing." << std::endl;
	std::cout << "Serializing data..." << std::endl;

	json z1 = convertJSONData(z_vector, freqs);
	json z2 = convertJSONData(z_vector, freqs_nonrepeated);
	json z3 = convertJSONData(z_vector, f_vector);

	std::string filename = "C:\\Users\\jinge\\Downloads\\freqs.json";
	std::cout << "Writing data to " << filename << std::endl;
	std::ofstream out1(filename);
	out1 << z1 << std::endl;
	out1.close();

	filename = "C:\\Users\\jinge\\Downloads\\freqs_nonrepeated.json";
	std::cout << "Writing data to " << filename << std::endl;
	std::ofstream out2(filename);
	out2 << z2 << std::endl;
	out2.close();

	filename = "C:\\Users\\jinge\\Downloads\\z.json";
	std::cout << "Writing data to " << filename << std::endl;
	std::ofstream out3(filename);
	out3 << z3 << std::endl;
	out3.close();

	std::cout << "Finished." << std::endl;

	return 0;
}

// Discritize an interval including its end points
std::vector<double> DiscritizeInterval(double start, double end, double delta) {
	size_t N = abs(start - end) / delta;
	std::vector<double> x(N, start);
	for (size_t i = 1; i < N; i++) {
		x[i] = x[i - 1] + delta;
	}
	x.push_back(end);
	return x;
}

// Search the sorted vector for the distribution of elements up to the specified value
double SearchVectorDist(QSMatrix<double> vec, double value) {
	size_t M = vec.get_rows();
	size_t count = 0;
	double divisor = 0.0;
	for (size_t i = 0; i < M; i++) {
		for (size_t j = 0; j < vec.get_cols(); j++) {
			if (vec(i, j) != 0) {
				if (vec(i, j) <= value) {
					count++;
				}
				divisor++;
			}
		}
	}
	return (double)count / divisor;
}

// Evaluate F(z) in Bosma and Smeets
double F(double z) {
	double a = 1.0 / sqrt(5);

	if (z >= 0.5) {
		return 1;
	}
	else if (z >= a) {
		double s = sqrt(1.0 - (4.0 * z * z));
		return (1.0 / log(StringParsing::phi)) * (s + log(StringParsing::phi * ((1.0 - s) / (2.0 * z))));
	}
	else if (z >= 0) {
		return z / log(StringParsing::phi);
	}
	else {
		return 0; //continuing the end point
	}

}

json convertJSONData(std::vector<double> z_vector, std::vector<double> vec) {
	double z[ITERATIONS] = { 0 };
	double v[ITERATIONS] = { 0 };
	for (size_t i = 0; i < z_vector.size(); i++) {
		z[i] = z_vector[i];
		v[i] = vec[i];
	}

	json j;
	j["z"] = z;
	j["v"] = v;
	return j;
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