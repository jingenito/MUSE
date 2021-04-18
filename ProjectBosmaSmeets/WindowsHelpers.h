#pragma once

#include <string>

namespace WindowsHelpers {
	
	// Determine if the specified full path is an existing file.
	bool FullPathFileExists(const std::string& full_path);

	// Retrieve the full path from the specified relative path within the current working directory. Uses the std::experimental::filesystem library.
	std::string GetFullPathFromRelativePath(const std::string& rel_path);

}