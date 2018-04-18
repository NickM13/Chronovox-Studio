#pragma once
#include "engine\utils\Utilities.h"
#include <stdlib.h>
#include <vector>
#include <filesystem>

class LDirectory {
public:
	static void getFilesInDirectory(std::vector<std::string>& p_files, std::string p_root, std::string p_fileExtension);
	static std::vector<std::string> getFilesInDirectory(std::string p_path, std::string p_fileExtension);
	static std::string getProjectPath();
};
