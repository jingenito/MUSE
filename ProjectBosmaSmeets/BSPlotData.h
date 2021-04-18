#pragma once

#include "CPPLibrary.h"

namespace BSPlotDirichletData {

	// Discritize an interval including its end points.
	std::vector<double> DiscritizeInterval(const double& start, const double& end, const size_t& N);

	// Get a vector that represents the Dirichlet distribution for Optimal CF over a given sequence of z values.
	std::vector<double> GetOptimalCFData(std::vector<double> Z_seq);

	// Get the Dirichlet data for fixed m, n, and d. Returns a matrix where Row 0 is the nonfiltered data, and Row 1 filters duplicates. Can throw invalid_argument or incorrectdimensionexception.
	QSMatrix<double> GetSinglePlotDirichletData(const size_t& m, const size_t& n, const double& d, const size_t& iterations);

	// Get the Dirichlet data over the specified sequence for M, N and D. Returns a vector of matrices where Row 0 is the nonfiltered data, and Row 1 filters duplicates. Can throw invalid_argument or incorrectdimensionexception.
	std::vector< QSMatrix<double> > RunBSPlotConfiguration(const std::vector<int>& M, const std::vector<int>& N, const std::vector<int>& D, const size_t& iterations);

	// Create a probability density function over the specified z vector.
	std::vector<double> ProcessRow_DirichletData(const std::vector<double>& z_vector, const std::vector<double>& dirichlet_vector);

} // BSPlotDirichletData