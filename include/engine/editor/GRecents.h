#pragma once

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\logger\Logger.h"

#include <vector>

class GRecents {
private:
	static std::string m_fileDirectory;
	static std::vector<std::string> m_recentFiles;
public:
	static void init();
	static void save();

	static std::vector<std::string>& getRecentFiles() { return m_recentFiles; }

	static void addFile(std::string&);
	static void renameFile(std::string&, std::string&);
};
