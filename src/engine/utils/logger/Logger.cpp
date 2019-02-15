#include "engine\utils\logger\Logger.h"

#include "engine\utils\directory\LDirectory.h"
#include <iostream>
#include <iomanip>
#include <chrono>

Logger::Verbosity Logger::m_verbosity = Logger::Verbosity::DIAGNOSTIC;
bool Logger::m_consoleOutput = false;
std::string Logger::m_logFile = "";
long long Logger::m_logTime = 0;

void Logger::init(Verbosity p_verbosity) {
	m_logTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	m_consoleOutput = GetConsoleWindow() != NULL;
	m_verbosity = p_verbosity;

	LPSYSTEMTIME systime = new _SYSTEMTIME();
	GetSystemTime(systime);
	int i = 0;
	std::string logFolder = LDirectory::getProjectPath() + "logs\\";
	std::string filePrefix = logFolder
		+ std::to_string(systime->wYear) + "-"
		+ std::to_string(systime->wMonth) + "-"
		+ std::to_string(systime->wDay) + "-";
	std::string fileExt = ".log";
	bool exists = true;
	while (exists) {
		exists = false;
		CreateDirectory(logFolder.c_str(), NULL);
		for (std::string file : LDirectory::getFilesInDirectory(logFolder, fileExt)) {
			if (file == filePrefix + std::to_string(i) + fileExt) {
				i++;
				exists = true;
				break;
			}
		}
	}
	m_logFile = filePrefix + std::to_string(i) + fileExt;
	logQuiet("Logger initialized");
	switch (p_verbosity) {
	case Verbosity::QUIET:		logQuiet("Logger verbosity set to quiet"); break;
	case Verbosity::MINIMAL:	logQuiet("Logger verbosity set to minimal"); break;
	case Verbosity::NORMAL:		logQuiet("Logger verbosity set to normal"); break;
	case Verbosity::DETAILED:	logQuiet("Logger verbosity set to detailed"); break;
	case Verbosity::DIAGNOSTIC: logQuiet("Logger verbosity set to diagnostic"); break;
	}

}

void Logger::terminate() {
	m_logFile = "";
}

std::string Logger::getTimeFormatted() {
	Sint64 time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - m_logTime;
	Sint32 s = ((Sint32)std::floorf(time)) % 60;
	Sint32 m = (Sint32)std::floorf(time / 60.f) % 60;
	Sint32 h = (Sint32)std::floorf(time / 60.f / 60.f);

	std::ostringstream oss;
	oss << h;
	oss << ':' << std::setfill('0') << std::setw(2) << m;
	oss << ':' << std::setfill('0') << std::setw(2) << s;

	return oss.str();
}

void Logger::log(Verbosity p_verbosity, Type p_type, std::string p_msg) {
	if (static_cast<Sint32>(p_verbosity) <= static_cast<Sint32>(m_verbosity)) {
		std::string msg = std::string("[")
			+ getTimeFormatted() + " ";
		switch (p_type) {
		case Type::NONE: break;
		case Type::INFO: msg += "INFO"; break;
		case Type::WARN: msg += "WARN"; break;
		case Type::ERR: msg += "ERROR"; break;
		default: break;
		}
		msg += "]: " + p_msg + "\n";
		if (m_consoleOutput) {
			std::cout << msg;
		}
		if (m_logFile != "") {
			std::ofstream logFile(m_logFile, std::ios::app);
			logFile << msg;
			logFile.close();
		}
	}
}

void Logger::log(Verbosity p_verbosity, Type p_type, std::string p_msg, va_list p_list) {
	if (static_cast<Sint32>(p_verbosity) <= static_cast<Sint32>(m_verbosity)) {
		std::string msg = "";
		char c0, c1;
		for (Sint32 i = 0; i < p_msg.length(); i++) {
			c0 = p_msg[i];
			if (c0 == '%') {
				c1 = p_msg[i + 1];
				switch (c1) {
				case 'i':
					msg += std::to_string(va_arg(p_list, int));
					break;
				case 'f':
					msg += std::to_string(va_arg(p_list, float));
					break;
				case 'd':
					msg += std::to_string(va_arg(p_list, double));
					break;
				case 'b':
					msg += std::to_string(va_arg(p_list, bool));
					break;
				case 's':
					msg += va_arg(p_list, std::string);
					break;
				default:
					msg += c0;
					msg += c1;
					break;
				}
				i++;
			}
			else {
				msg += c0;
			}
		}
		log(p_verbosity, p_type, msg);
	}
}

void Logger::logMissingFile(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::QUIET, Logger::Type::ERR, "Could not find file: \"" + p_msg + "\"", list);
	va_end(list);
}

void Logger::logLoadedFile(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::NORMAL, Logger::Type::INFO, "Loaded file: \"" + p_msg + "\"", list);
	va_end(list);
}

void Logger::logSavedFile(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::NORMAL, Logger::Type::INFO, "Saved file: \"" + p_msg + "\"", list);
	va_end(list);
}

void Logger::logError(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::QUIET, Logger::Type::ERR, p_msg, list);
	va_end(list);
}

void Logger::logWarning(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::MINIMAL, Logger::Type::WARN, p_msg, list);
	va_end(list);
}

void Logger::logQuiet(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::QUIET, Logger::Type::INFO, p_msg, list);
	va_end(list);
}

void Logger::logMinimal(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::MINIMAL, Logger::Type::INFO, p_msg, list);
	va_end(list);
}

void Logger::logNormal(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::NORMAL, Logger::Type::INFO, p_msg, list);
	va_end(list);
}

void Logger::logDetailed(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::DETAILED, Logger::Type::INFO, p_msg, list);
	va_end(list);
}

void Logger::logDiagnostic(std::string p_msg, ...) {
	va_list list;
	va_start(list, p_msg);
	log(Logger::Verbosity::DIAGNOSTIC, Logger::Type::INFO, p_msg, list);
	va_end(list);
}
