#include "engine\utils\directory\LDirectory.h"

void LDirectory::getFilesInDirectory(std::vector<std::string>& p_files, std::string p_root, std::string p_fileExtension) {
	std::string path;
	for(std::experimental::filesystem::directory_entry p : std::experimental::filesystem::directory_iterator(p_root)) {
		path = p.path().string();
		if(std::experimental::filesystem::is_directory(p.status())) {
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
	return "C:\\Users\\NickM\\Documents\\GitHub\\Voxel Projects\\Voxel Model Editor\\";
	//return path.substr(0, path.find_last_of('\\') + 1);
}
