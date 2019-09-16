#include "engine\editor\GPreferences.h"
#include "engine\utils\directory\LDirectory.h"

#include <fstream>

GPreferences::ViewMode GPreferences::m_viewMode = GPreferences::ViewMode::PERSPECTIVE;
bool GPreferences::m_vsync = false;
Sint32 GPreferences::m_focusFps = 60, GPreferences::m_unfocusFps = 20;
Logger::Verbosity GPreferences::m_logVerbosity = Logger::Verbosity::QUIET;
GPreferences::AntiAlias GPreferences::m_antiAlias = GPreferences::AntiAlias::NONE;
bool GPreferences::m_voxelAO = false;
Sint32 GPreferences::m_gridCount = 32;
Sint32 GPreferences::m_gridSpace = 2;

void GPreferences::init() {
	std::ifstream fs(LDirectory::getProjectPath() + "res\\config\\Preferences.ini");
	if (fs.good()) {
		std::string line = "";
		std::string first, second;
		std::stringstream ss;
		size_t equalPos;
		while (!fs.eof()) {
			std::getline(fs, line);
			if (line.empty() || line.at(0) == '#') continue;
			equalPos = line.find_first_of('=');
			first = line.substr(0, equalPos);
			second = line.substr(equalPos + 1);

			if (first == "VIEW") {
				m_viewMode = static_cast<ViewMode>(std::stoi(second));
			} else if (first == "FFPS") {
				m_focusFps = std::stoi(second);
			} else if (first == "UFPS") {
				m_unfocusFps = std::stoi(second);
			} else if (first == "ANTIALIAS") {
				m_antiAlias = static_cast<AntiAlias>(std::stoi(second));
			} else if (first == "GRIDCOUNT") {
				m_gridCount = std::stoi(second);
			} else if (first == "GRIDSPACE") {
				m_gridSpace = std::stoi(second);
			}

			line = "";
		}
	}
	fs.close();
}
void GPreferences::save() {
	std::ofstream fs(LDirectory::getProjectPath() + "res\\config\\Preferences.ini");
	if (fs.good()) {
		fs << "VIEW=" << static_cast<Sint32>(m_viewMode) << std::endl;
		fs << "FFPS=" << m_focusFps << std::endl;
		fs << "UFPS=" << m_unfocusFps << std::endl;
		fs << "ANTIALIAS=" << static_cast<Sint32>(m_antiAlias) << std::endl;
		fs << "GRIDCOUNT=" << m_gridCount << std::endl;
		fs << "GRIDSPACE=" << m_gridSpace << std::endl;
	}
	fs.close();
}

float GPreferences::getSamples() {
	switch (m_antiAlias) {
	case AntiAlias::LOW: return 2.f;
	case AntiAlias::MEDIUM: return 3.f;
	case AntiAlias::HIGH: return 4.f;
	case AntiAlias::NONE:
	default: return 1.f;
	}
}
