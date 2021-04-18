#pragma once

#include <nlohmann/json.hpp>

namespace JSONHelpers {

	// Write a config file according to the expected format with the specified values. Can throw std::invalid_argument.
	nlohmann::json WriteBSPlotsConfig(const std::vector<int>& M, const std::vector<int>& N, const std::vector<int>& D, const size_t& iterations, const size_t& Z_length, const bool& show_optimalCF, const bool& allow_duplicates);

	// Read the Bosma Smeets Plots configuration represented by a json object.
	void ReadBSPlotsConfig(const nlohmann::json& config, std::vector<int>& M, std::vector<int>& N, std::vector<int>& D, size_t& iterations, size_t& Z_length, bool& show_optimalCF, bool& allow_duplicates);

	// Serialize the json data to the specified filename.
	void SerializeJSON(const nlohmann::json& json, const std::string& filename);

	// Desearialize the specified json file.
	nlohmann::json DeserializeJSON(const std::string& filename);

} // JSONHelpers