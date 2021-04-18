#include <iostream>
#include <cstdlib>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <string>

#include "BSPlotData.h"
#include "JSONHelpers.h"
#include "WindowsHelpers.h"

int main(int argc, char** argv)
{
	nlohmann::json config;
	std::string config_filename = "bsplotsconfig.json";
	std::string config_full_path = WindowsHelpers::GetFullPathFromRelativePath(config_filename);
	if (WindowsHelpers::FullPathFileExists(config_full_path)) {
		// config file found - loading saved configuration
		config = JSONHelpers::DeserializeJSON(config_full_path);
	}
	else {
		std::vector<int> M, N, D;
		size_t iterations, Z_length, include_optimalCF, allow_duplicates;

		// config file not found - writing a new one
		std::cout << "No config file was found, creating a new one..." << std::endl;
		config.clear();

		std::string input;
		std::cout << "Input M sequence" << std::endl;
		std::getline(std::cin, input);
		M = CPPMathLibrary::StringParsing::SplitNumericString(input);

		std::cout << "Input N sequence" << std::endl;
		std::getline(std::cin, input);
		N = CPPMathLibrary::StringParsing::SplitNumericString(input);

		std::cout << "Input D sequence" << std::endl;
		std::getline(std::cin, input);
		D = CPPMathLibrary::StringParsing::SplitNumericString(input);

		std::cout << "Input iterations: ";
		std::cin >> iterations;
		std::cout << "Input Z_length: ";
		std::cin >> Z_length;
		std::cout << "Include optimal CF (0 or 1): ";
		std::cin >> include_optimalCF;
		std::cout << "Allow duplicates (0 or 1): ";
		std::cin >> allow_duplicates;

		try {
			config = JSONHelpers::WriteBSPlotsConfig(M, N, D, iterations, Z_length, include_optimalCF == 1, allow_duplicates == 1);
		}
		catch (std::invalid_argument* argEx) {
			std::cout << "Errors occured when saving a new config file - " << argEx->what() << " - Exit failure." << std::endl;
			return EXIT_FAILURE;
		}

		JSONHelpers::SerializeJSON(config, config_filename);
		std::cout << "Saved config to " << config_filename << " in the same directory as the current executable." << std::endl;
	}

	size_t iterations, Z_length;
	bool show_optimalCF, allow_duplicates;
	std::vector<int> M, N, D;
	JSONHelpers::ReadBSPlotsConfig(config, M, N, D, iterations, Z_length, show_optimalCF, allow_duplicates);

	std::cout << "Configuration began..." << std::endl;
	std::vector< QSMatrix<double> > full_data_set;
	try {
		full_data_set = BSPlotDirichletData::RunBSPlotConfiguration(M, N, D, iterations);
	}
	catch (std::invalid_argument* argEx) {
		std::cout << "Invalid argument - " << argEx->what() << " - Exit failure." << std::endl;
		return EXIT_FAILURE;
	}
	catch (IncorrectDimensionException* idEx) {
		std::cout << "Errors occured while running ILLL - " << idEx->getMessage() << " - Exit failure." << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Finished calculating Dirichlet coefficients.\nBegin processing " << (allow_duplicates ? "with" : "without") << " duplicates... " << (show_optimalCF ? "Including" : "Excluding") << " Optimal CF..." << std::endl;
	std::vector<double> z_vector = BSPlotDirichletData::DiscritizeInterval(0, 0.5, Z_length);

	// need to use std::vector at this point for json serialization
	std::vector< std::vector<double> > optimal_cf_data_to_serialize(2);
	std::vector< std::vector<double> > dup_data_to_serialize(full_data_set.size() + 1);
	std::vector< std::vector<double> > nondup_data_to_serialize(full_data_set.size() + 1);

	// first row is the z vector
	dup_data_to_serialize[0] = z_vector;
	nondup_data_to_serialize[0] = z_vector;
	optimal_cf_data_to_serialize[0] = z_vector;

	if (show_optimalCF) 
		optimal_cf_data_to_serialize[1] = BSPlotDirichletData::GetOptimalCFData(z_vector);

	for (size_t i = 0; i < full_data_set.size(); i++) {
		QSMatrix<double> this_result = full_data_set[i];

		if (allow_duplicates) 
			dup_data_to_serialize[i + 1] = BSPlotDirichletData::ProcessRow_DirichletData(z_vector, this_result.getRowVector<double>(0));

		nondup_data_to_serialize[i + 1] = BSPlotDirichletData::ProcessRow_DirichletData(z_vector, this_result.getRowVector<double>(1));
	}

	std::cout << "Finished processing.\nSerializing data..." << std::endl;

	nlohmann::json json_to_serialize;
	if (show_optimalCF) {
		json_to_serialize["Optimal_CF"] = optimal_cf_data_to_serialize;
	}
	if (allow_duplicates) {
		json_to_serialize["Dup_Data"] = dup_data_to_serialize;
	}
	json_to_serialize["Nondup_Data"] = nondup_data_to_serialize;

	std::string out_file_name = "bsplotsdata.json";
	JSONHelpers::SerializeJSON(json_to_serialize, out_file_name);

	std::cout << "Serialized data to " << out_file_name << " in the same directory as the current executable.\nExit success." << std::endl;

	return EXIT_SUCCESS;
}