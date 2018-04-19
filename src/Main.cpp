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
	bool _success;
	if(argc > 1) _success = Application::getInstance().init(argv[1]);
	else _success = Application::getInstance().init();
	if(_success) {
		Application::getInstance().run();
		Application::getInstance().terminate();
		//_CrtDumpMemoryLeaks();
		return 0;
	}
	return -1;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
	if(Application::getInstance().init()) {
		Application::getInstance().run();
		Application::getInstance().terminate();
		return 0;
	}
	return -1;
}
