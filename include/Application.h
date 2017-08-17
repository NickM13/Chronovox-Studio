#pragma once

#include "engine\utils\global\GScreen.h"
#include "engine\utils\OpenGL.h"
#include "engine\utils\Singleton.h"
#include "engine\utils\Utilities.h"

#include "engine\editor\Editor.h"

class Application : public Singleton<Application>
{
public:
	bool init();
	void run();
	void close();
private:
	static Editor* m_editor;
	static GLFWwindow * m_mainWindow;
	static Vector2<Uint16> m_screenSize;

	time_t m_maxFps = 60;
	DWORD m_sleepTime;

	static void keyCallback(GLFWwindow*, int, int, int, int);
	static void mousePressCallback(GLFWwindow*, int, int, int);
	static void mouseMovedCallback(GLFWwindow*, double, double);
	static void mouseScrollCallback(GLFWwindow*, double, double);
	static void windowResizeCallback(GLFWwindow*, int, int);
	static void mouseEnterCallback(GLFWwindow*, int);
	static void dropFileCallback(GLFWwindow*, int count, const char** paths);

	static void maximize(bool p_maximizedByDrag);

	static void init2d();
	static void init3d();

	void input();
	void update();
	void render();
};
