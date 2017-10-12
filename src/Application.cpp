#include "Application.h"

Editor* Application::m_editor = 0;
Vector2<Uint16> Application::m_screenSize = {};
GLFWwindow* Application::m_mainWindow = 0;

bool Application::init(char *p_filePath) {
	GScreen::m_fov = 70;
	m_maxFps = 60;
	m_screenSize = Vector2<Uint16>(1280, 768);

	GScreen::m_developer = true;
	GScreen::m_fps = 0;
	GScreen::m_exitting = 0;

	for(Uint16 i = 0; i < 1024; i++)
		GKey::m_keyStates[i] = 0;

	if(!glfwInit()) return false;
	if(!glewInit()) return false;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_FLOATING, 0);

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_mainWindow = glfwCreateWindow(min(m_screenSize.x, mode->width), min(m_screenSize.y, mode->height), "Voxel Model Editor v1.0", 0, 0);
	glfwSetWindowPos(m_mainWindow, (mode->width - m_screenSize.x) / 2, (mode->height - m_screenSize.y) / 2);
	GScreen::initWindow(m_mainWindow);
	if(!m_mainWindow) {
		glfwTerminate();
		return false;
	}

	glfwSetKeyCallback(m_mainWindow, keyCallback);
	glfwSetMouseButtonCallback(m_mainWindow, mousePressCallback);
	glfwSetCursorEnterCallback(m_mainWindow, mouseEnterCallback);
	glfwSetCursorPosCallback(m_mainWindow, mouseMovedCallback);
	glfwSetScrollCallback(m_mainWindow, mouseScrollCallback);
	glfwSetWindowSizeCallback(m_mainWindow, windowResizeCallback);
	glfwSetDropCallback(m_mainWindow, dropFileCallback);

	glfwMakeContextCurrent(m_mainWindow);
	glClearColor(0.35f, 0.25f, 0.95f, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_editor = new Editor();

	return true;
}

void Application::close() {
	glfwDestroyWindow(m_mainWindow);
	glfwTerminate();
	delete m_editor;
}

void Application::keyCallback(GLFWwindow* p_window, int p_keycode, int p_scancode, int p_action, int p_mods) {
	//std::cout << p_keycode << " " << p_scancode << " " << p_action << " " << p_mods << "\n";
	GKey::m_keyEvents.push_back({p_keycode, p_scancode, p_action, p_mods});
	if(GScreen::m_developer && p_keycode == '`') {
		GScreen::m_exitting = 2;
		return;
	}
	if(p_action) {
		GKey::m_keyStates[p_keycode] = GKey::KEY_PRESS;
		GKey::m_keyStates[p_keycode] += GKey::KEY_HOLD;
	}
	else {
		GKey::m_keyStates[p_keycode] = GKey::KEY_RELEASE;
	}
}

void Application::mousePressCallback(GLFWwindow* p_window, int p_button, int p_state, int p_mods) {
	if(!(GMouse::m_mouseStates[p_button]) != !(p_state)) {
		if(GMouse::m_mouseStates[p_button] == 0) {
			if(GMouse::m_mouseDelay[p_button] > 0) {
				GMouse::m_mouseStates[p_button] = GMouse::MOUSE_PRESS | GMouse::MOUSE_DOWN | GMouse::MOUSE_DOUBLECLICK;
				GMouse::m_mouseDelay[p_button] = 0;
			}
			else {
				GMouse::m_mouseStates[p_button] = GMouse::MOUSE_PRESS | GMouse::MOUSE_DOWN;
				GMouse::m_mouseDelay[p_button] = 0.25f;
			}
		}
		else
			GMouse::m_mouseStates[p_button] = GMouse::MOUSE_RELEASE;
	}
}

void Application::mouseMovedCallback(GLFWwindow* p_window, double p_x, double p_y) {
	Sint32 x = p_x, y = p_y;
	if(GMouse::m_mousePos.x != x && GMouse::m_mousePos.y != y)
		GMouse::m_mouseMoved = true;
	else
		GMouse::m_mouseMoved = false;
	GMouse::m_mousePos = Vector2<Sint32>(x, y);
}

void Application::mouseScrollCallback(GLFWwindow* p_window, double p_x, double p_y) {
	GMouse::m_mouseScroll += Sint8(p_y);
}

void Application::windowResizeCallback(GLFWwindow* p_window, int p_x, int p_y) {
	GScreen::m_screenSize = Vector2< Uint16 >(p_x, p_y);
	m_screenSize = Vector2< Uint16 >(p_x, p_y);

	glViewport(0, 0, m_screenSize.x, m_screenSize.y);

	init2d();
}

void Application::mouseEnterCallback(GLFWwindow* p_window, int p_action) {}

void Application::dropFileCallback(GLFWwindow* p_window, int count, const char** paths) {
	m_editor->dropFile(paths[0]);
	glfwFocusWindow(m_mainWindow);
}

void Application::maximize(bool p_maximizedByDrag) {
	GScreen::m_maximized = !GScreen::m_maximized;
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if(GScreen::m_maximized) {
		HMONITOR hMon = MonitorFromPoint({0, 0}, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMon, &info);
		GScreen::m_smallScreen = m_screenSize;
		glfwSetWindowMonitor(m_mainWindow, 0, info.rcWork.left, info.rcWork.top, info.rcWork.right - info.rcWork.left, info.rcWork.bottom - info.rcWork.top, mode->refreshRate);
		GScreen::m_screenSize = m_screenSize = Vector2<Uint16>(Uint16(info.rcWork.right - info.rcWork.left), Uint16(info.rcWork.bottom - info.rcWork.top));
	}
	else {
		GScreen::m_screenSize = m_screenSize = GScreen::m_smallScreen;
		glfwSetWindowMonitor(m_mainWindow, 0, (mode->width - m_screenSize.x) / 2, (mode->height - m_screenSize.y) / 2, m_screenSize.x, m_screenSize.y, mode->refreshRate);
	}

	m_editor->resize();
}
void Application::resize() {
	if(GScreen::m_maximized) GScreen::m_maximized = false;
	m_screenSize = GScreen::m_screenSize;
	glfwSetWindowSize(m_mainWindow, m_screenSize.x, m_screenSize.y);
	m_editor->resize();
}

void Application::init2d()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_screenSize.x, m_screenSize.y, 0, -2000.f, 2000.f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDisable(GL_MULTISAMPLE);
	glLoadIdentity();
}
void Application::init3d()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(GScreen::m_fov, GLfloat(m_screenSize.x) / m_screenSize.y, 0.01f, 2000.f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_BACK);
	glLoadIdentity();
}

void Application::run()
{
	GLdouble i;
	while(GScreen::m_exitting != 2) {
		i = glfwGetTime();

		/*
	#ifdef _DEBUG
		if(GScreen::m_keyStates[GLFW_KEY_1] & GKey::KEY_PRESS)
			DebugBreak();
	#endif
		*/

		input();
		update();
		render();

		m_sleepTime = DWORD(max(1000 / m_maxFps - ((glfwGetTime() - i) * 1000), 0));
		if(m_sleepTime > 0)
			Sleep(m_sleepTime);
		GScreen::m_fps = 1.f / GLfloat(glfwGetTime() - i);

		if(GScreen::m_exitting == 1) {
			GScreen::m_exitting = 2;
			/*
			if(m_editor->attemptClose()) {
				GScreen::m_exitting = 2;
			}
			else {
				GScreen::m_exitting = 0;
				glfwSetWindowShouldClose(m_mainWindow, false);
			}
			*/
		}
	}

	glfwTerminate();
}

void Application::input()
{
	if(glfwWindowShouldClose(m_mainWindow))
		GScreen::m_exitting = 1;
	GKey::m_keyEvents.clear();
	GMouse::m_mouseScroll = 0;
	for(Uint16 i = 0; i < 1024; i++) {
		if(GKey::m_keyStates[i] & GKey::KEY_PRESS)
			GKey::m_keyStates[i] -= GKey::KEY_PRESS;
		if(GKey::m_keyStates[i] & GKey::KEY_RELEASE)
			GKey::m_keyStates[i] -= GKey::KEY_RELEASE;
	}
	glfwPollEvents();
	m_editor->input();
	if(GScreen::isMaximized() && GScreen::isDraggingWindow() && GScreen::m_dragStart.y < GMouse::m_mousePos.y)
		maximize(true);
	if(GScreen::finishedResize()) resize();
	GScreen::updateWindow();
}

GLfloat _last = 0;
void Application::update()
{
	m_editor->update();

	GLfloat _curr = glfwGetTime();
	for(Uint16 i = 0; i < 64; i++)
	{
		if(GMouse::m_mouseStates[i] & GMouse::MOUSE_PRESS)
			GMouse::m_mouseStates[i] -= GMouse::MOUSE_PRESS;
		if(GMouse::m_mouseStates[i] & GMouse::MOUSE_RELEASE)
			GMouse::m_mouseStates[i] = 0;
		if(GMouse::m_mouseDelay[i] > 0)
			GMouse::m_mouseDelay[i] -= (_curr - _last);
	}
	_last = glfwGetTime();

	if(GScreen::m_windowCommand == GScreen::MINIMIZE) glfwIconifyWindow(m_mainWindow);
	if(GScreen::m_windowCommand == GScreen::RESIZE) maximize(false);
	if(GScreen::m_windowCommand == GScreen::CLOSE) glfwSetWindowShouldClose(m_mainWindow, true);
	GScreen::m_windowCommand = GScreen::NONE;

	//For appending model name
	//glfwSetWindowTitle(m_mainWindow, std::string("Voxel Engine Indev 0.1").c_str());
}

void Application::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	init3d();
	m_editor->render3d();
	init2d();
	m_editor->render2d();

	glfwSwapBuffers(m_mainWindow);
}
