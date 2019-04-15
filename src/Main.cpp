/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
*/
#include "Application.h"

#include "engine\sfx\Sound.h"

int main(int argc, char* argv[]) {
	//_crtBreakAlloc;
	Logger::init(static_cast<Sint8>(Logger::Verbosity::QUIET));
	Logger::logQuiet("Program launched using main");
	Logger::logQuiet("Initializing...");
	bool _success;
	if (argc > 1) {
		_success = Application::getInstance().init(argv[1]);
	}
	else {
		_success = Application::getInstance().init();
	}
	if (_success) {
		Logger::logQuiet("Program initialized");
		Application::getInstance().run();
		Application::getInstance().terminate();
		Logger::logQuiet("Program closing...");
		//_CrtDumpMemoryLeaks();
		return 0;
	}
	Logger::logError("Failed to start program, exitting...");
	Logger::terminate();
	return -1;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
	Logger::init(static_cast<Sint8>(Logger::Verbosity::QUIET));
	Logger::logQuiet("Program launched using WinMain");
	Logger::logQuiet("Initializing...");
	if (Application::getInstance().init()) {
		Logger::logQuiet("Program initialized");
		Application::getInstance().run();
		Application::getInstance().terminate();
		Logger::logQuiet("Program closing...");
		return 0;
	}
	Logger::logError("Failed to start program, exitting...");
	Logger::terminate();
	return -1;
}
