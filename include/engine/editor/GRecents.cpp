#include "engine\editor\GRecents.h"
#include "engine\utils\directory\LDirectory.h"

#include <fstream>

std::string GRecents::m_fileDirectory;
std::list<std::string> GRecents::m_recentFiles;

void GRecents::init() {
	m_fileDirectory = LDirectory::getProjectPath() + "res\\config\\Recents.ini";
	std::ifstream ifs(m_fileDirectory);
	if (ifs.good()) {
		std::string line;
		while (!ifs.eof()) {
			std::getline(ifs, line);
			if (line.length() > 1)
				m_recentFiles.insert(line, 0);
		}
	}
	ifs.close();
}
void GRecents::save() {
	std::ofstream ofs(m_fileDirectory);
	for (std::string file : m_recentFiles) {
		ofs << file << "\n";
	}
	ofs.close();
}

void GRecents::addFile(std::string& p_file) {
	m_recentFiles.push_back(p_file);
}
