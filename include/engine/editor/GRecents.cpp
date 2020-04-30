#include "engine\editor\GRecents.h"
#include "engine\utils\directory\LDirectory.h"

#include <fstream>

std::string GRecents::m_fileDirectory;
std::vector<std::string> GRecents::m_recentFiles;

void GRecents::init() {
	m_fileDirectory = LDirectory::getPreferencePath() + "Recents";
	std::ifstream ifs(m_fileDirectory);
	if (ifs.good()) {
		std::string line;
		while (!ifs.eof()) {
			std::getline(ifs, line);
			if (line.length() > 1)
				m_recentFiles.insert(m_recentFiles.begin(), line);
		}
	}
	ifs.close();
}
void GRecents::save() {
	// Remove duplicates
	for (Sint32 i = 0; i < m_recentFiles.size(); i++) {
		for (Sint32 j = m_recentFiles.size() - 1; j > i; j--) {
			if (m_recentFiles.at(i) == m_recentFiles.at(j)) {
				m_recentFiles.erase(m_recentFiles.begin() + j);
			}
		}
	}

	std::ofstream ofs(m_fileDirectory);
	ofs.clear();
	for (std::string file : m_recentFiles) {
		ofs << file << "\n";
	}
	ofs.close();
}

void GRecents::addFile(std::string& p_file) {
	m_recentFiles.insert(m_recentFiles.begin(), p_file);
	save();
}

void GRecents::renameFile(std::string& p_from, std::string& p_to) {
	for (Sint32 i = 0; i < static_cast<Sint32>(m_recentFiles.size()); i++) {
		if (m_recentFiles.at(i) == p_from) {
			m_recentFiles.erase(m_recentFiles.begin() + i);
			return;
		}
	}
	m_recentFiles.insert(m_recentFiles.begin(), p_to);
	save();
}
