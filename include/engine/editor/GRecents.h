#pragma once

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\logger\Logger.h"

#include <list>

class GRecents {
private:
	static std::string m_fileDirectory;
	static std::list<std::string> m_recentFiles;
public:
	static void init();
	static void save();

	static std::list<std::string>& getRecentFiles() { return m_recentFiles; }

	static void addFile(std::string&);
};
