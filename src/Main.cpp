#include "Application.h"

#include "engine\sfx\Sound.h"

int main(int argc, wchar_t* argv[])
{
	if(Application::getInstance().init())
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
