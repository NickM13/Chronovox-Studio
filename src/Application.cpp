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

	if(!glfwInit()) return false;
	if(!glewInit()) return false;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_FLOATING, 0);

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_mainWindow = glfwCreateWindow(min(m_screenSize.x, mode->width), min(m_screenSize.y, mode->height), "Voxel Model Editor", 0, 0);
	glfwSetWindowPos(m_mainWindow, (mode->width - m_screenSize.x) / 2, (mode->height - m_screenSize.y) / 2);
	GScreen::initWindow(m_mainWindow);
	if(!m_mainWindow) {
		glfwTerminate();
		return false;
	}

	glfwSetKeyCallback(m_mainWindow, GKey::keyCallback);
	glfwSetMouseButtonCallback(m_mainWindow, GMouse::mousePressCallback);
	glfwSetCursorPosCallback(m_mainWindow, GMouse::mouseMovedCallback);
	glfwSetScrollCallback(m_mainWindow, GMouse::mouseScrollCallback);
	glfwSetCursorEnterCallback(m_mainWindow, mouseEnterCallback);
	glfwSetWindowSizeCallback(m_mainWindow, windowResizeCallback);
	glfwSetDropCallback(m_mainWindow, dropFileCallback);

	glfwMakeContextCurrent(m_mainWindow);
	glClearColor(0.35f, 0.25f, 0.95f, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_editor = new Editor();

	return true;
}

void Application::terminate() {
	glfwTerminate();
	delete m_editor;
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

void Application::init2d() {
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
void Application::init3d() {
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

void Application::run() {
	GLdouble i;
	while(GScreen::m_exitting != 2) {
		i = glfwGetTime();

		input();
		update();
		render();

		m_sleepTime = DWORD(max(1000 / m_maxFps - ((glfwGetTime() - i) * 1000), 0));
		if(m_sleepTime > 0)
			Sleep(m_sleepTime);
		GScreen::m_fps = 1.f / GLfloat(glfwGetTime() - i);

		if(GScreen::m_exitting == 1) {
			if(m_editor->attemptClose()) {
				GScreen::m_exitting = 2;
			} else {
				GScreen::m_exitting = 0;
				glfwSetWindowShouldClose(m_mainWindow, false);
			}
		}
	}

	glfwTerminate();
}

void Application::input() {
	if(glfwWindowShouldClose(m_mainWindow))
		GScreen::m_exitting = 1;
	GMouse::reset();
	GKey::reset();
	glfwPollEvents();
	m_editor->input();
	if(GScreen::isMaximized() && GScreen::isDraggingWindow() && GScreen::m_dragStart.y < GMouse::getMousePos().y)
		maximize(true);
	if(GScreen::finishedResize()) resize();
	GScreen::updateWindow();
}

GLfloat _last = 0;
void Application::update() {
	m_editor->update();

	GMouse::update(GScreen::m_deltaTime);

	if(GScreen::m_windowCommand == GScreen::MINIMIZE) glfwIconifyWindow(m_mainWindow);
	if(GScreen::m_windowCommand == GScreen::RESIZE) maximize(false);
	if(GScreen::m_windowCommand == GScreen::CLOSE) glfwSetWindowShouldClose(m_mainWindow, true);
	GScreen::m_windowCommand = GScreen::NONE;
}

void Application::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	init3d();
	m_editor->render3d();
	init2d();
	m_editor->render2d();

	glfwSwapBuffers(m_mainWindow);
}
