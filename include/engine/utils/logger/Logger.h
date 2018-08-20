#pragma once

#include <stdlib.h>
#include <string>
#include <fstream>
#include <Windows.h>

// Logger class - prints to console (if available) and writes to log file (named by date)
class Logger {
public:
	enum class Verbosity {
		NONE = 0,
		QUIET = 1,
		MINIMAL = 2,
		NORMAL = 3,
		DETAILED = 4,
		DIAGNOSTIC = 5
	};
	enum class Type {
		NONE = 0,
		INFO = 1,
		WARN = 2,
		ERR = 3
	};

private:
	static Verbosity m_verbosity;
	static bool m_consoleOutput;
	static std::string m_logFile;
	static long long m_logTime;

	static std::string getTimeFormatted();

public:
	static void init(Verbosity p_verbosity);
	static void terminate();

	static void log(Verbosity p_verbosity, Type p_type, std::string p_msg);

	// Most common uses of log for ease of use
	// Log a missing file error
	static void logMissingFile(std::string p_msg);
	// Log a loaded file
	static void logLoadedFile(std::string p_msg);
	// Log a saved file
	static void logSavedFile(std::string p_msg);
	// Log as an error message
	static void logError(std::string p_msg);
	// Log as a warning message
	static void logWarning(std::string p_msg);
	// Log as an info quiet message
	static void logQuiet(std::string p_msg);
	// Log as an info minimal message
	static void logMinimal(std::string p_msg);
	// Log as an info normal message
	static void logNormal(std::string p_msg);
	// Log as an info detailed message
	static void logDetailed(std::string p_msg);
	// Log as an info diagnostic message
	static void logDiagnostic(std::string p_msg);
};
