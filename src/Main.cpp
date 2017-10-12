#include "Application.h"

#include "engine\sfx\Sound.h"

int main(int argc, char* argv[])
{
	bool _success;
	if(argc > 1) _success = Application::getInstance().init(argv[1]);
	else _success = Application::getInstance().init();
	if(_success)
	{
		Application::getInstance().run();
		Application::getInstance().close();
		return 0;
	}
	return -1;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	if(Application::getInstance().init())
	{
		Application::getInstance().run();
		Application::getInstance().close();
		return 0;
	}
	return -1;
}
