#include "engine\utils\directory\LDirectory.h"

#include <direct.h>
#include <shlobj.h>
#include <iostream>
#include <condition_variable>
#include <iostream>

void LDirectory::getFilesInDirectory(std::vector<std::string>& p_files, std::string p_root, std::string p_fileExtension) {
	std::string path;
	for(std::filesystem::directory_entry p : std::filesystem::directory_iterator(p_root)) {
		path = p.path().string();
		if(std::filesystem::is_directory(p.status())) {
			getFilesInDirectory(p_files, path, p_fileExtension);
		}
		else if(path.substr(path.length() - p_fileExtension.length()) == p_fileExtension) {
			p_files.push_back(path);
		}
	}
}

std::vector<std::string> LDirectory::getFilesInDirectory(std::string p_path, std::string p_fileExtension) {
	std::vector<std::string> files;
	getFilesInDirectory(files, p_path, p_fileExtension);
	return files;
}

std::string LDirectory::getProjectPath() {
	std::string path;
	char result[MAX_PATH];
	memset(result, 0, MAX_PATH);
	GetModuleFileName(NULL, result, MAX_PATH);
	path = std::string(result, MAX_PATH);
	return path.substr(0, path.find_last_of('\\') + 1);
}

std::string LDirectory::getPreferencePath() {
	char filepath[MAX_PATH];
	HRESULT res = SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, filepath);

	strcat_s(filepath, "\\Chronovox Studio\\Settings\\");
	if (_mkdir(filepath)) {
		Logger::logDiagnostic("Directory made");
	}

	return filepath;
}
