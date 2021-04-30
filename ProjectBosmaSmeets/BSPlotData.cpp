#include "BSPlotData.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <random>

std::vector<double> BSPlotDirichletData::DiscritizeInterval(const double& start, const double& end, const size_t& N) {
	double delta = std::abs(start - end) / (N - 1);
	std::vector<double> x(N, start);
	for (size_t i = 1; i < N - 1; i++) {
		x[i] = x[i - 1] + delta;
	}
	x[N - 1] = end;
	return x;
}

double SearchVectorDist(const std::vector<double>& vec, const double& value) {
	size_t total_dir = 0, non_zero_count = 0;
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i] != 0) {
			if (vec[i] <= value) {
				total_dir++;
			}
			non_zero_count++;
		}
	}
	return (double)total_dir / non_zero_count;
}

double F(const double& z) {
	double a = 1.0 / std::sqrt(5);

	if (z >= 0.5) {
		return 1;
	}
	else if (z >= a) {
		double s = std::sqrt(1.0 - (4.0 * z * z));
		return (1.0 / std::log(CPPMathLibrary::StringParsing::phi)) * (s + std::log(CPPMathLibrary::StringParsing::phi * ((1.0 - s) / (2.0 * z))));
	}
	else if (z >= 0) {
		return z / std::log(CPPMathLibrary::StringParsing::phi);
	}
	else {
		return 0; //continuing the end point
	}

}

std::vector<double> BSPlotDirichletData::GetOptimalCFData(const std::vector<double>& Z_seq) {
	std::vector<double> f_vector(Z_seq.size());
	for (size_t k = 0; k < Z_seq.size(); k++) {
		f_vector[k] = F(Z_seq[k]);
	}
	return f_vector;
}

//assumes the seed has already been set for the rng
double GetILLLRandomNumber() {
	// Use random_device to generate a seed for Mersenne twister engine.
	std::random_device rd{};

	// Use Mersenne twister engine to generate pseudo-random numbers.
	std::mt19937 engine{ rd() };

	// "Filter" MT engine's output to generate pseudo-random double values,
	// **uniformly distributed** on the closed interval [0, 1].
	// (Note that the range is [inclusive, inclusive].)
	std::uniform_real_distribution<double> dist{ 0, 1 };

	double x;
	{ x = dist(engine);  }while (x == 0 || x == 1);

	return x;
}

//assumes the seed has already been set for the rng
QSMatrix<double> GetILLLRandomizedMatrix(const size_t& m, const size_t& n, const size_t& format_precision) {
	QSMatrix<double> result(m, n, format_precision, 0.0);
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++) {
			result(i, j) = GetILLLRandomNumber();
		}
	}
	return result;
}

QSMatrix<double> BSPlotDirichletData::GetSinglePlotDirichletData(const size_t& m, const size_t& n, const double& d, const size_t& iterations) {
	size_t N = iterations;
	size_t nm = n + m; //for convenience
	QSMatrix<double> preValues(m, n, 0);

	double epsilon = 1.0 / d;
	double alpha = 0.75;

	//values taken from constraints in the Bosma Smeets paper
	double val = (std::pow(nm, 2) / m) - ((double)nm / m * std::log(epsilon));
	size_t M = (size_t)std::ceil(val) + 30;
	size_t qmax = std::pow(2, M) - 1;
	size_t k_prime = (size_t)std::ceil((-1.0 * (nm - 1) * nm) / (4.0 * n) + (m * std::log(qmax) / (std::log(2) * n)));

	std::vector<double> dir_coefs; //all dirichlet coefficients
	std::vector<double> dir_coefs_nonrepeated; //this name is misleading as it refers to only non repeated dir coeffs for a fixed i
	std::vector<double> current_dir_coeffs; //holds the dir coeffs for the current approximation matrix

	for (size_t i = 0; i < N; i++) {
		preValues = GetILLLRandomizedMatrix(m, n, 10);

		std::vector< QSMatrix<double> > result = CPPMathLibrary::SimultaneousDiophantine::IteratedLLL_Dyadic(preValues, alpha, epsilon, qmax, M);
		current_dir_coeffs.clear(); current_dir_coeffs.resize(k_prime, 0.0);

		for (size_t k = 0; k < result.size(); k++) {
			double dir_coef = CPPMathLibrary::SimultaneousDiophantine::DirichletCoefficient(result[k], preValues);
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

	//return a matrix representation of the data
	QSMatrix<double> Q(2, k_prime * N, 0);
	Q.setRowVector(dir_coefs, 0);
	Q.setRowVector(dir_coefs_nonrepeated, 1);
	return Q;
}

std::vector< QSMatrix<double> > BSPlotDirichletData::RunBSPlotConfiguration(const std::vector<int>& M, const std::vector<int>& N, const std::vector<int>& D, const size_t& iterations) {
	if (M.size() != N.size()) {
		throw new std::invalid_argument("M and N are not the same size.");
	}

	std::vector<int> D_seq = D;
	if (D.size() == 1) {
		D_seq.resize(M.size(), D[0]);
	}
	else if (D.size() != M.size()) {
		throw new std::invalid_argument("D should be either size 1 or the same as M and N.");
	}

	std::vector< QSMatrix<double> > all_data;
	for (size_t i = 0; i < M.size(); i++) {
		std::cout << "Calculating Dirichlet data for: m = " << M[i] << ", n = " << N[i] << ", d = " << D_seq[i] << std::endl;
		QSMatrix<double> singlePlotData = BSPlotDirichletData::GetSinglePlotDirichletData(M[i], N[i], (double)D_seq[i], iterations);
		all_data.push_back(singlePlotData);
	}

	return all_data;
}

std::vector<double> BSPlotDirichletData::ProcessRow_DirichletData(const std::vector<double>& z_vector, const std::vector<double>& dirichlet_vector) {
	size_t z_length = z_vector.size();
	std::vector<double> theta(z_length, 0);
	for (size_t i = 0; i < z_length; i++) {
		theta[i] = SearchVectorDist(dirichlet_vector, z_vector[i]);
	}
	return theta;
}