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
double SearchVectorDist(std::vector<double>, double value);
double F(double z);
json convertJSONData(std::vector<double> z_vector, std::vector<double> vec);
double GetILLLRandomNumber();
QSMatrix<double> GetILLLRandomizedMatrix(const size_t, const size_t);

const int ZVECTORLIMIT = 2000;
const int ITERATIONS = 1000;

int main(int argc, char** argv)
{
	std::srand(std::time(nullptr)); // use current time as seed for random generator

	size_t N = ITERATIONS;
	size_t m = 1, n = 1;
	size_t nm = n + m;
	double deltaX = 1.0 / 1000;

	QSMatrix<double> preValues(m, n, 0);

	double epsilon = 1.0 / 128;
	double alpha = 0.75;

	size_t M = 0;
	double val = (pow(nm, 2) / m) - ((double)nm / m * log(epsilon));
	M = (size_t)ceil(val) + 30;
	size_t qmax = pow(2, M) - 1;
	size_t k_prime = (size_t)ceil((-1.0 * (nm - 1) * nm) / (4.0 * n) + (m * log(qmax) / (log(2) * n)));

	std::vector<double> dir_coefs; //all dirichlet coefficients
	std::vector<double> dir_coefs_nonrepeated; //this name is misleading as it refers to only non repeated dir coeffs for a fixed i

	std::cout << "Starting " << N << " ILLL approximations..." << std::endl;

	try {
		for (size_t i = 0; i < N; i++) {
			preValues = GetILLLRandomizedMatrix(m, n);
			
			std::vector< QSMatrix<int> > result = SimultaneousDiophantine::IteratedLLL_Dyadic(preValues, alpha, epsilon, qmax, M);
			std::vector<double> current_dir_coeffs(result.size(), 0);

			for (size_t k = 0; k < result.size(); k++) {
				double dir_coef = SimultaneousDiophantine::DirichletCoefficient(result[k], preValues);
				current_dir_coeffs[k] = dir_coef;
			}

			dir_coefs.insert(
				dir_coefs.end(),
				std::make_move_iterator(current_dir_coeffs.begin()),
				std::make_move_iterator(current_dir_coeffs.end())
			); //store all the current dirichlet coefficients to the global list

			//remove the duplicated dir coeffs from this list only - If I only consider one set of approximations (holding i fixed), then a repeated dirichlet coefficient 
			//would correspond to a repeated approx; however, if we are considering dir coeffs from all N approx's then it is not enough to only check dir coeffs - JI
			std::sort(current_dir_coeffs.begin(), current_dir_coeffs.end());
			current_dir_coeffs.erase(std::unique(current_dir_coeffs.begin(), current_dir_coeffs.end()), current_dir_coeffs.end());

			dir_coefs_nonrepeated.insert(
				dir_coefs_nonrepeated.end(),
				std::make_move_iterator(current_dir_coeffs.begin()),
				std::make_move_iterator(current_dir_coeffs.end())
			); //store the non repeated dir coeffs to the global list
		}
	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << "Errors occured during the ILLL approximations." << std::endl;
		std::cout << idEx->getMessage() << std::endl;
		return 0;
	}

	double delta = 0.001;
	std::cout << "Finished ILLL approximations." << std::endl;
	std::cout << "Discritizing the interval [0,1] by delta = " << delta << std::endl;
	std::vector<double> z_vector = DiscritizeInterval(0, 0.5, delta);
	std::cout << "Processing dirichlet coefficients..." << std::endl;

	//remove 0 dir coeffs before searching
	dir_coefs.erase(remove(dir_coefs.begin(), dir_coefs.end(), 0), dir_coefs.end());
	dir_coefs_nonrepeated.erase(remove(dir_coefs_nonrepeated.begin(), dir_coefs_nonrepeated.end(), 0), dir_coefs_nonrepeated.end());
	//sort both vectors before searching
	//std::sort(dir_coefs.begin(), dir_coefs.end());
	//std::sort(dir_coefs_nonrepeated.begin(), dir_coefs_nonrepeated.end());

	//storing vector sizes for calculations
	double zSize = z_vector.size();
	//initialize the data to be stored in json format
	std::vector<double> freqs(zSize, 0);
	std::vector<double> freqs_nonrepeated(zSize, 0);
	std::vector<double> f_vector(zSize, 0);

	for (size_t k = 0; k < zSize; k++) {
		double z = z_vector[k];
		f_vector[k] = F(z);
		freqs[k] = SearchVectorDist(dir_coefs, z);
		freqs_nonrepeated[k] = SearchVectorDist(dir_coefs_nonrepeated, z);
	}

	std::cout << "Finished processing." << std::endl;
	std::cout << "Serializing data..." << std::endl;

	json z1 = convertJSONData(z_vector, freqs);
	json z2 = convertJSONData(z_vector, freqs_nonrepeated);
	json z3 = convertJSONData(z_vector, f_vector);

	std::string filename = "freqs.json";
	std::cout << "Writing data to " << filename << std::endl;
	std::ofstream out1(filename);
	out1 << z1 << std::endl;
	out1.close();

	filename = "freqs_nonrepeated.json";
	std::cout << "Writing data to " << filename << std::endl;
	std::ofstream out2(filename);
	out2 << z2 << std::endl;
	out2.close();

	filename = "F.json";
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

// Search the vector for the distribution of elements up to the specified value
double SearchVectorDist(std::vector<double> vec, double value) {
	size_t count = 0;
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i] <= value) {
			count++;
		}
	}
	return (double)count / vec.size();
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
	double z[ZVECTORLIMIT] = { 0 };
	double v[ZVECTORLIMIT] = { 0 };
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