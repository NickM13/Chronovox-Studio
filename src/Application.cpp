#include "Application.h"
#include <glm\gtc\matrix_transform.hpp>

Editor* Application::m_editor = 0;
Vector2<Uint16> Application::m_screenSize = {};
GLFWwindow* Application::m_mainWindow = 0;

bool Application::init(char *p_filePath) {
	GScreen::m_fov = 70;
	m_focusFps = 60;
	m_unfocusFps = 60;

	Logger::logNormal("Initializing application...");

	GScreen::m_appName = "Chronovox Studio";
	GScreen::m_windowTitle = GScreen::m_appName;
	GScreen::m_appVersion = "1.2.4.1";
	GScreen::m_developer = true;
	GScreen::m_fps = 0;
	GScreen::m_exitting = 0;
	GScreen::m_shadows = false;

	if (!glfwInit()) {
		Logger::logError("glfw failed to initialize");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_FLOATING, 0);

	int monx, mony;
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	glfwGetMonitorPos(monitor, &monx, &mony);
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	m_screenSize = Vector2<Uint16>(std::fminf(1280, mode->width - 128), std::fminf(768, mode->height - 128));
	m_mainWindow = glfwCreateWindow(std::fminf(m_screenSize.x, mode->width), std::fminf(m_screenSize.y, mode->height), GScreen::m_windowTitle.c_str(), 0, 0);
	if (!m_mainWindow) {
		Logger::logError("glfwWindow failed to initialize");
		glfwTerminate();
		return false;
	}
	glfwSetWindowPos(m_mainWindow, (mode->width - m_screenSize.x) / 2, (mode->height - m_screenSize.y) / 2);

	GScreen::initWindow(m_mainWindow);

	glfwSetKeyCallback(m_mainWindow, GKey::keyCallback);
	glfwSetMouseButtonCallback(m_mainWindow, GMouse::mousePressCallback);
	glfwSetCursorPosCallback(m_mainWindow, GMouse::mouseMovedCallback);
	glfwSetScrollCallback(m_mainWindow, GMouse::mouseScrollCallback);
	glfwSetCursorEnterCallback(m_mainWindow, mouseEnterCallback);
	glfwSetWindowSizeCallback(m_mainWindow, windowResizeCallback);
	glfwSetDropCallback(m_mainWindow, dropFileCallback);
	glfwSetWindowIconifyCallback(m_mainWindow, GScreen::windowIconifyCallback);
	glfwSetWindowFocusCallback(m_mainWindow, GScreen::windowFocusCallback);

	glfwMakeContextCurrent(m_mainWindow);

	GLenum error;
	if ((error = glewInit()) != GLEW_OK) {
		Logger::logError(std::string("glew failed to initialize: ").append((const char*)glewGetErrorString(error)));
		return false;
	}

	Shader::init();
	Shader::getProgram("simple")
		->loadShader(GL_VERTEX_SHADER, "simple.vert")
		->loadShader(GL_FRAGMENT_SHADER, "simple.frag");
	Shader::getProgram("shadowmap")
		->loadShader(GL_VERTEX_SHADER, "shadowmap.vert")
		->loadShader(GL_FRAGMENT_SHADER, "shadowmap.frag");
	Shader::getProgram("depthRTT")
		->loadShader(GL_VERTEX_SHADER, "depthRTT.vert")
		->loadShader(GL_FRAGMENT_SHADER, "depthRTT.frag");
	Shader::getProgram("gui")
		->loadShader(GL_VERTEX_SHADER, "gui.vert")
		->loadShader(GL_FRAGMENT_SHADER, "gui.frag");

	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLua::init();
	//GLua::loadScriptFile("script.lua");

	m_editor = new Editor();

	return true;
}

void Application::terminate() {
	Logger::logNormal("Terminating application...");
	Shader::terminate();
	GLua::terminate();
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
	if (GScreen::m_maximized) {
		Vector2<Sint32> wpos, wsize;
		glfwGetWindowPos(GScreen::m_window, &wpos.x, &wpos.y);
		glfwGetWindowSize(GScreen::m_window, &wsize.x, &wsize.y);
		HMONITOR hMon = MonitorFromPoint({ wpos.x + wsize.x / 2, wpos.y + wsize.y / 2 }, MONITOR_DEFAULTTOPRIMARY);
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
	if (GScreen::m_maximized) GScreen::m_maximized = false;
	m_screenSize = GScreen::m_screenSize;
	glfwSetWindowSize(m_mainWindow, m_screenSize.x, m_screenSize.y);
	m_editor->resize();
}

void Application::init2d() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDisable(GL_MULTISAMPLE);
	glLoadIdentity();

	Shader::loadIdentityModel();
	Shader::loadIdentityView();
	Shader::loadIdentityProjection();

	glm::mat4 projection = glm::ortho(0.f, (GLfloat)m_screenSize.x, (GLfloat)m_screenSize.y, 0.f, -2000.f, 2000.f);
	Shader::transformProjection(projection);
	Shader::applyProjection();
	Shader::applyView();
	Shader::applyModel();
}
void Application::init3dPersp() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_BACK);
	glLoadIdentity();

	Shader::loadIdentityModel();
	Shader::loadIdentityView();
	Shader::loadIdentityProjection();

	glm::mat4 projection = glm::perspective((GLfloat)glm::radians(GScreen::m_fov), (GLfloat)GScreen::m_screenSize.x / GScreen::m_screenSize.y, 0.1f, 2000.0f);
	Shader::transformProjection(projection);
	Shader::applyProjection();
	Camera::setProjectionMatrix(projection);

	glUniform1i(4, 0);
}
void Application::init3dOrtho() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_FRONT);
	glLoadIdentity();

	Shader::loadIdentityModel();
	Shader::loadIdentityView();
	Shader::loadIdentityProjection();

	glm::mat4 depthProjectionMatrix = glm::ortho<GLfloat>(-64.0f, 64.0f, -64.0f, 64.0f, -64.0f, 64.0f);
	glm::mat4 depthViewMatrix = m_editor->getSunlightMatrix();

	Shader::transformProjection(depthProjectionMatrix);
	Shader::applyProjection();
	Shader::transformView(depthViewMatrix);
	Shader::applyView();
}

void Application::run() {
	GLdouble i;
	while (GScreen::m_exitting != 2) {
		i = glfwGetTime();

		input();
		update();
		render();

		if (GScreen::m_focused) {
			m_sleepTime = DWORD(std::fmaxf(1000 / m_focusFps - ((glfwGetTime() - i) * 1000), 0));
		}
		else {
			m_sleepTime = DWORD(std::fmaxf(1000 / m_unfocusFps - ((glfwGetTime() - i) * 1000), 0));
		}
		if (m_sleepTime > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(m_sleepTime));
		}
		GScreen::m_fps = 1.f / GLfloat(glfwGetTime() - i);

		if (GScreen::m_exitting == 1) {
			if (m_editor->attemptClose()) {
				GScreen::m_exitting = 2;
			}
			else {
				GScreen::m_exitting = 0;
				glfwSetWindowShouldClose(m_mainWindow, false);
			}
		}
	}

	glfwTerminate();
}

void Application::input() {
	if (glfwWindowShouldClose(m_mainWindow)) {
		GScreen::m_exitting = 1;
	}
	GMouse::reset();
	GKey::reset();
	glfwPollEvents();
	m_editor->input();
	if (GScreen::isMaximized() && GScreen::isDraggingWindow() && GScreen::m_dragStart.y < GMouse::getMousePos().y) {
		maximize(true);
	}
	if (GScreen::finishedResize()) resize();
	GScreen::updateWindow();
}

GLfloat _last = 0;
void Application::update() {
	m_editor->update();

	if (GScreen::m_windowCommand == GScreen::WindowCommand::MINIMIZE)	glfwIconifyWindow(m_mainWindow);
	if (GScreen::m_windowCommand == GScreen::WindowCommand::RESIZE)		maximize(false);
	if (GScreen::m_windowCommand == GScreen::WindowCommand::CLOSE)		glfwSetWindowShouldClose(m_mainWindow, true);
	GScreen::m_windowCommand = GScreen::WindowCommand::NONE;

	GMouse::update(GScreen::m_deltaTime);
}

void Application::render() {
	if (GScreen::m_iconified) return;

	if (GScreen::m_shadows) {
		m_editor->bindShadowBuffer();
		Shader::useProgram("depthRTT");
		m_editor->bindShadowTexture();
		init3dOrtho();
		m_editor->renderShadow();
		m_editor->unbindShadowBuffer();
	}

	glm::mat4 biasMatrix{
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};
	glm::mat4 depthBiasMVP = biasMatrix * Shader::getMVP();
	
	Shader::useProgram("shadowmap");
	Shader::setLightEnabled(false);
	init3dPersp();
	glUniformMatrix4fv(10, 1, GL_FALSE, &depthBiasMVP[0][0]);
	glUniform3f(11, m_editor->getSunlightDir().x, m_editor->getSunlightDir().y, m_editor->getSunlightDir().z);
	m_editor->render3d();

	Shader::useProgram("gui");
	init2d();
	m_editor->render2d();

	glfwSwapBuffers(m_mainWindow);
}
