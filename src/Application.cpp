#include "Application.h"
#include <glm\gtc\matrix_transform.hpp>
#include "engine\editor\GPreferences.h"

Editor* Application::m_editor = 0;
Vector2<Uint16> Application::m_screenSize = {};
GLFWwindow* Application::m_mainWindow = 0;

bool Application::init(char *p_filePath) {
	GScreen::setFov(70);

	Logger::logNormal("Initializing application...");

	GScreen::setAppName("Chronovox Studio");
	GScreen::setAppVersion("1.3.0.1");
	GScreen::setWindowTitle("Chronovox Studio");
	GScreen::setDeveloper(false);
	GScreen::enableShadows(false);
	GScreen::setMinScreenSize(Vector2<Sint32>(600.f, 600.f));

	Logger::logQuiet("Setting GScreen values");

	if (!glfwInit()) {
		Logger::logError("glfw failed to initialize");
		return false;
	}

	Logger::logQuiet("GLFW Initialized");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_FLOATING, 0);

	Logger::logQuiet("Setting GLFW Hints");

	int monx, mony;
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	Logger::logQuiet("Found primary monitor");
	glfwGetMonitorPos(monitor, &monx, &mony);
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	m_screenSize = Vector2<Uint16>(std::fminf(1280, mode->width - 128), std::fminf(768, mode->height - 128));
	m_mainWindow = glfwCreateWindow(std::fminf(m_screenSize.x, mode->width), std::fminf(m_screenSize.y, mode->height), GScreen::getWindowTitle().c_str(), 0, 0);
	if (!m_mainWindow) {
		Logger::logError("glfwWindow failed to initialize");
		glfwTerminate();
		return false;
	}
	glfwSetWindowPos(m_mainWindow, (mode->width - m_screenSize.x) / 2, (mode->height - m_screenSize.y) / 2);

	GScreen::initWindow(m_mainWindow);

	Logger::logQuiet("Window initialized");

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

	Logger::logQuiet("GLFW Window set up");

	GLenum error;
	if ((error = glewInit()) != GLEW_OK) {
		Logger::logError(std::string("glew failed to initialize: ").append((const char*)glewGetErrorString(error)));
		return false;
	}

	Logger::logQuiet("GLEW Set up");

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
	Shader::getProgram("gBuffer")
		->loadShader(GL_VERTEX_SHADER, "gbuffer.vert")
		->loadShader(GL_FRAGMENT_SHADER, "gbuffer.frag");
	Shader::getProgram("deferredShading")
		->loadShader(GL_VERTEX_SHADER, "deferredshading.vert")
		->loadShader(GL_FRAGMENT_SHADER, "deferredshading.frag");
	Shader::getProgram("ssaa")
		->loadShader(GL_VERTEX_SHADER, "ssaa.vert")
		->loadShader(GL_FRAGMENT_SHADER, "ssaa.frag");

	Logger::logQuiet("Shaders set up");

	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	GLua::init();

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
	GScreen::setScreenSize(Vector2< Uint16 >(p_x, p_y));
	m_screenSize = Vector2< Uint16 >(p_x, p_y);

	glViewport(0, 0, m_screenSize.x, m_screenSize.y);

	init2d();
}

void Application::mouseEnterCallback(GLFWwindow* p_window, int p_Action) {
	GMouse::setMouseActive(p_Action);
}

void Application::dropFileCallback(GLFWwindow* p_window, int count, const char** paths) {
	for (Sint32 i = 0; i < 1; i++) {
		m_editor->dropFile(paths[i]);
	}
	glfwFocusWindow(m_mainWindow);
}

void Application::maximize(bool p_maximizedByDrag) {
	GScreen::setMaximized(!GScreen::isMaximized());
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (GScreen::isMaximized()) {
		Vector2<Sint32> wpos, wsize;
		glfwGetWindowPos(GScreen::getGLFWWindow(), &wpos.x, &wpos.y);
		glfwGetWindowSize(GScreen::getGLFWWindow(), &wsize.x, &wsize.y);
		HMONITOR hMon = MonitorFromPoint({ wpos.x + wsize.x / 2, wpos.y + wsize.y / 2 }, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMon, &info);
		GScreen::setSmallScreenSize(m_screenSize);
		glfwSetWindowMonitor(m_mainWindow, 0, info.rcWork.left, info.rcWork.top, info.rcWork.right - info.rcWork.left, info.rcWork.bottom - info.rcWork.top, mode->refreshRate);
		GScreen::setScreenSize(m_screenSize = Vector2<Uint16>(Uint16(info.rcWork.right - info.rcWork.left), Uint16(info.rcWork.bottom - info.rcWork.top)));
	}
	else {
		m_screenSize = GScreen::getSmallScreenSize();
		GScreen::setScreenSize(m_screenSize);
		glfwSetWindowMonitor(m_mainWindow, 0, (mode->width - m_screenSize.x) / 2, (mode->height - m_screenSize.y) / 2, m_screenSize.x, m_screenSize.y, mode->refreshRate);
	}

	m_editor->resize();
}
void Application::resize() {
	if (GScreen::isMaximized()) GScreen::setMaximized(false);
	m_screenSize = GScreen::getScreenSize();
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

	glm::mat4 projection = glm::perspective((GLfloat)glm::radians(GScreen::getFov()), (GLfloat)GScreen::getScreenSize().x / GScreen::getScreenSize().y, 0.1f, 10000.0f);
	Shader::transformProjection(projection);
	Shader::applyProjection();
	Camera::setProjectionMatrix(projection);

	glUniform1i(4, 0);
	glLineWidth(GPreferences::getSamples());
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
	glCullFace(GL_BACK);
	glLoadIdentity();

	Shader::loadIdentityModel();
	Shader::loadIdentityView();
	Shader::loadIdentityProjection();

	Vector2<GLfloat> screenSize = GScreen::getScreenSizeInverse() * Camera::getZoom();
	glm::mat4 depthProjectionMatrix = glm::ortho<GLfloat>(-screenSize.x, screenSize.x, -screenSize.y, screenSize.y, -2000.f, 2000.0f);
	glm::mat4 depthViewMatrix = m_editor->getSunlightMatrix();

	Shader::transformProjection(depthProjectionMatrix);
	Shader::applyProjection();
	//Shader::transformView(depthViewMatrix);
	//Shader::applyView();
	Camera::setProjectionMatrix(depthProjectionMatrix);

	glUniform1i(4, 0);
	glLineWidth(GPreferences::getSamples());
}

void Application::run() {
	GLdouble i;
	while (GScreen::isExitting() != 2) {
		i = glfwGetTime();

		input();
		update();
		render();

		if (GScreen::isFocused()) {
			m_sleepTime = DWORD(std::fmaxf(1000 / GPreferences::getFocusFPS() - ((glfwGetTime() - i) * 1000), 0));
		}
		else {
			m_sleepTime = DWORD(std::fmaxf(1000 / GPreferences::getUnfocusFPS() - ((glfwGetTime() - i) * 1000), 0));
		}
		if (m_sleepTime > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(m_sleepTime));
		}
		GScreen::setFps(1.f / GLfloat(glfwGetTime() - i));

		if (GScreen::isExitting() == 1) {
			if (m_editor->attemptClose()) {
				GScreen::setExitting(2);
			}
			else {
				GScreen::setExitting(0);
				glfwSetWindowShouldClose(m_mainWindow, false);
			}
		}
	}

	glfwTerminate();
}

void Application::input() {
	if (glfwWindowShouldClose(m_mainWindow)) {
		m_editor->attemptClose();
		glfwSetWindowShouldClose(m_mainWindow, false);
	}
	GMouse::reset();
	GKey::reset();
	glfwPollEvents();
	m_editor->input();
	if (GScreen::isMaximized() && GScreen::isDraggingWindow() && GScreen::getDragStart().y < GMouse::getMousePos().y) {
		maximize(true);
	}
	if (GScreen::isFinishedResizing()) resize();
	else if (GScreen::hasResizeUpdate()) glfwSetWindowSize(m_mainWindow, GScreen::getDragScreenSize().x, GScreen::getDragScreenSize().y);
	GScreen::updateWindow();
}

GLfloat _last = 0;
void Application::update() {
	Gui::setLoading(GFormat::getLoadPercent());
	m_editor->update();

	if (GScreen::getWindowCommand() == GScreen::WindowCommand::MINIMIZE)	glfwIconifyWindow(m_mainWindow);
	if (GScreen::getWindowCommand() == GScreen::WindowCommand::RESIZE)		maximize(false);
	if (GScreen::getWindowCommand() == GScreen::WindowCommand::CLOSE)		m_editor->attemptClose();
	GScreen::setWindowCommand(GScreen::WindowCommand::NONE);

	GMouse::update(GScreen::getDeltaUpdate());
}

void Application::render() {
	if (GScreen::isIconified()) return;
	
	switch (GPreferences::getViewMode()) {
	case GPreferences::ViewMode::PERSPECTIVE:
		init3dPersp();
		break;
	default:
		init3dOrtho();
		break;
	}

	m_editor->renderGeometry();

	m_editor->renderLight();

	Shader::useProgram("gui");
	init2d();
	m_editor->render2d();

	glfwSwapBuffers(m_mainWindow);
}
