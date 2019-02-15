#pragma once

#include "engine\utils\global\GScreen.h"
#include "engine\utils\LOpenGL.h"
#include "engine\utils\Singleton.h"
#include "engine\utils\Utilities.h"

#include "engine\editor\Editor.h"

class Application : public Singleton<Application> {
private:
	static Editor* m_editor;
	static GLFWwindow * m_mainWindow;
	static Vector2<Uint16> m_screenSize;
	GLuint m_shader;

	time_t m_focusFps;
	time_t m_unfocusFps;
	DWORD m_sleepTime;

	static void windowResizeCallback(GLFWwindow*, int, int);
	static void mouseEnterCallback(GLFWwindow*, int);
	static void dropFileCallback(GLFWwindow*, int count, const char** paths);

	static void maximize(bool p_maximizedByDrag);
	static void resize();

	static void init2d();
	static void init3dPersp();
	static void init3dOrtho();

	void input();
	void update();
	void render();
public:
	bool init(char *p_filePath = 0);
	void run();
	void terminate();
};
