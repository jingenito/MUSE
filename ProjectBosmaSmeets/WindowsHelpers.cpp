#include "WindowsHelpers.h"
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

bool WindowsHelpers::FullPathFileExists(const std::string& full_path) {
	WIN32_FIND_DATAA fd = { 0 };
	HANDLE hFound = FindFirstFileA(full_path.c_str(), &fd);
	bool retval = hFound != INVALID_HANDLE_VALUE;
	FindClose(hFound);

	return retval;
}

std::string WindowsHelpers::GetFullPathFromRelativePath(const std::string& rel_path) {
	fs::path p = rel_path;
	return fs::absolute(p).string();
}