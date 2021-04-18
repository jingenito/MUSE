#include "JSONHelpers.h"
#include <fstream>
#include <string>

nlohmann::json JSONHelpers::WriteBSPlotsConfig(const std::vector<int>& M, const std::vector<int>& N, const std::vector<int>& D, const size_t& iterations, const size_t& Z_length, const bool& show_optimalCF, const bool& allow_duplicates) {
	if (M.size() != N.size())
		throw new std::invalid_argument("M and N are not the same size.");

	if (D.size() != M.size()) {
		if (D.size() != 1)
			throw new std::invalid_argument("The size of D can only be 1 or equal to the size of M and N");
	}

	nlohmann::json j;
	j["M"] = M;
	j["N"] = N;
	j["D"] = D;
	j["iterations"] = iterations;
	j["Z_length"] = Z_length;
	j["show_optimalCF"] = show_optimalCF;
	j["allow_duplicates"] = allow_duplicates;

	return j;
}

void JSONHelpers::ReadBSPlotsConfig(const nlohmann::json& config, std::vector<int>& M, std::vector<int>& N, std::vector<int>& D, size_t& iterations, size_t& Z_length, bool& show_optimalCF, bool& allow_duplicates) {
	M = config["M"].get<std::vector<int>>();
	N = config["N"].get<std::vector<int>>();
	D = config["D"].get<std::vector<int>>();
	iterations = config["iterations"];
	Z_length = config["Z_length"];
	show_optimalCF = config["show_optimalCF"];
	allow_duplicates = config["allow_duplicates"];
}

void JSONHelpers::SerializeJSON(const nlohmann::json& json, const std::string& filename) {
	std::ofstream out(filename);
	out << json;
	out.close();
}

nlohmann::json JSONHelpers::DeserializeJSON(const std::string& filename) {
	std::ifstream infile(filename);
	nlohmann::json jobj;
	infile >> jobj;
	infile.close();
	return jobj;
}