#include "engine\utils\global\GScreen.h"
#include "engine\utils\global\event\GMouse.h"
#include "engine\utils\logger\Logger.h"

std::string GScreen::m_windowTitle = "";
std::string GScreen::m_appName = "";
std::string GScreen::m_appVersion = "";
float GScreen::m_fps = 0;
float GScreen::m_fov = 0;
float GScreen::m_deltaUpdate = 0;
Sint8 GScreen::m_exitting = 0;
bool GScreen::m_developer = false;

GLFWwindow *GScreen::m_window = 0;
Vector2<Sint32> GScreen::m_monitorSize = {};
Vector2<Sint32> GScreen::m_windowPos = {};
GLfloat GScreen::m_samples = 1;
bool GScreen::m_maximized = false;
bool GScreen::m_iconified = false;
bool GScreen::m_focused = true;
Vector2<Sint32> GScreen::m_screenSize = {};
Vector2<Sint32> GScreen::m_smallScreen = {};
Vector2<Sint32> GScreen::m_minScreenSize = {};
Vector2<Sint32> GScreen::m_dragScreenSize = {};
bool GScreen::m_draggingWindow = false;
Vector2<Sint32> GScreen::m_dragStart = {};
Vector2<Sint32> GScreen::m_dragDistance = {};
Vector2<Sint32> GScreen::m_initWindowSize = {};
Vector2<Sint32> GScreen::m_resizeMousePos = {};
bool GScreen::m_resizing = false;
bool GScreen::m_finishedResize = false;
bool GScreen::m_resizeUpdate = false;
bool GScreen::m_shadows = false;

GScreen::WindowCommand GScreen::m_windowCommand = GScreen::WindowCommand::NONE;
GScreen::ResizeType GScreen::m_resizeType = GScreen::ResizeType::BOTTOM;

void GScreen::initWindow(GLFWwindow *p_window) {
	m_window = p_window;

	int x, y;
	glfwGetWindowPos(m_window, &x, &y);
	m_windowPos = Vector2<Sint32>(x, y);

	glfwGetWindowSize(m_window, &x, &y);
	m_smallScreen = m_screenSize = Vector2<Sint32>(x, y);
}

void GScreen::windowIconifyCallback(GLFWwindow* p_window, int iconified) {
	m_iconified = (iconified != 0);
}

void GScreen::windowFocusCallback(GLFWwindow* p_window, int focus) {
	if (focus == GLFW_TRUE) {
		m_focused = true;
	}
	else {
		m_focused = false;
	}
}

void GScreen::startWindowDrag() {
	m_draggingWindow = true;
	m_dragStart = GMouse::getMousePos();
}
bool GScreen::isDraggingWindow() {
	return m_draggingWindow;
}
void GScreen::endWindowDrag() {
	if (m_draggingWindow) {
		Vector2<Sint32> wpos, wsize;
		glfwGetWindowPos(GScreen::m_window, &wpos.x, &wpos.y);
		glfwGetWindowSize(GScreen::m_window, &wsize.x, &wsize.y);
		HMONITOR hMon = MonitorFromPoint({ wpos.x + wsize.x / 2, wpos.y + wsize.y / 2 }, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMon, &info);

		glfwGetWindowPos(m_window, &wpos.x, &wpos.y);
		if (wpos.y + GMouse::getMousePos().y <= info.rcWork.top) {
			GScreen::m_windowCommand = WindowCommand::RESIZE;
		}
		m_draggingWindow = false;
		m_dragDistance = {};
	}
}

void GScreen::startResizing(ResizeType p_resizeType) {
	m_initWindowSize = m_screenSize;
	m_resizeMousePos = GMouse::getMousePos();
	m_resizing = true;
	m_resizeType = p_resizeType;
	m_dragStart = GMouse::getMousePos();
	m_dragScreenSize = m_screenSize;
}
void GScreen::stopResizing() {
	m_screenSize = m_dragScreenSize;
	m_resizing = false;
	m_finishedResize = true;
}

void GScreen::updateWindow() {
	if (isDraggingWindow() && !isMaximized()) {
		m_dragDistance = m_dragStart - GMouse::getMousePos();
		m_windowPos = m_windowPos - m_dragDistance / 2;
		glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);
		m_dragStart = GMouse::getMousePos() + m_dragDistance;
	}
	else if (m_finishedResize || m_resizeUpdate) {
		m_finishedResize = false;
		m_resizeUpdate = false;
	}
	else if (m_resizing) {
		if (!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			stopResizing();
		}
		Vector2<Sint32> _size = m_dragScreenSize;
		switch (m_resizeType) {
		case ResizeType::TOP:
			m_dragDistance.y = m_dragStart.y - GMouse::getMousePos().y;
			m_dragDistance.y = (Sint32)((std::fmaxl(m_minScreenSize.y, m_dragScreenSize.y + m_dragDistance.y) - m_dragScreenSize.y) * 2);
			m_dragScreenSize.y = (Sint32)std::fmaxl(m_minScreenSize.y, m_dragScreenSize.y + m_dragDistance.y / 2.f);
			m_windowPos.y = m_windowPos.y - m_dragDistance.y / 2;
			glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);
			break;
		case ResizeType::RIGHT:
			m_dragScreenSize.x = (Sint32)std::fmaxl(m_minScreenSize.x, (GLfloat)(m_initWindowSize.x - (m_resizeMousePos.x - GMouse::getMousePos().x)));
			break;
		case ResizeType::BOTTOM:
			m_dragScreenSize.y = (Sint32)std::fmaxl(m_minScreenSize.y, (GLfloat)(m_initWindowSize.y - (m_resizeMousePos.y - GMouse::getMousePos().y)));
			break;
		case ResizeType::LEFT:
			/*m_dragDistance.x = m_dragStart.x - GMouse::getMousePos().x;
			m_dragDistance.x = (Sint32)((std::fmaxl(m_minScreenSize.x, m_dragScreenSize.x + m_dragDistance.x) - m_dragScreenSize.x) * 2);
			m_windowPos.x = m_windowPos.x - m_dragDistance.x / 2;
			glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);
			m_dragStart.x = GMouse::getMousePos().x + m_dragDistance.x;
			m_dragScreenSize.x = (Sint32)std::fmaxl(m_minScreenSize.x, m_dragScreenSize.x + m_dragDistance.x / 2.f);*/

			m_dragDistance.x = m_dragStart.x - GMouse::getMousePos().x;
			m_dragDistance.x = (Sint32)((std::fmaxl(m_minScreenSize.x, m_dragScreenSize.x + m_dragDistance.x) - m_dragScreenSize.x) * 2);
			m_dragScreenSize.x = (Sint32)std::fmaxl(m_minScreenSize.x, m_dragScreenSize.x + m_dragDistance.x / 2.f);
			m_windowPos.x = m_windowPos.x - m_dragDistance.x / 2;
			glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);
			break;
		case ResizeType::TOP_RIGHT:
			m_dragScreenSize.x = (Sint32)std::fmaxl(m_minScreenSize.x, (GLfloat)(m_initWindowSize.x - (m_resizeMousePos.x - GMouse::getMousePos().x)));

			m_dragDistance.y = m_dragStart.y - GMouse::getMousePos().y;
			m_dragDistance.y = (Sint32)((std::fmaxl(m_minScreenSize.y, m_dragScreenSize.y + m_dragDistance.y) - m_dragScreenSize.y) * 2);
			m_dragScreenSize.y = (Sint32)std::fmaxl(m_minScreenSize.y, m_dragScreenSize.y + m_dragDistance.y / 2.f);
			m_windowPos.y = m_windowPos.y - m_dragDistance.y / 2;
			glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);
			break;
		case ResizeType::BOTTOM_RIGHT:
			m_dragScreenSize.x = (Sint32)std::fmaxl(m_minScreenSize.x, (GLfloat)(m_initWindowSize.x - (m_resizeMousePos.x - GMouse::getMousePos().x)));

			m_dragScreenSize.y = (Sint32)std::fmaxl(m_minScreenSize.y, (GLfloat)(m_initWindowSize.y - (m_resizeMousePos.y - GMouse::getMousePos().y)));
			break;
		case ResizeType::BOTTOM_LEFT:
			m_dragDistance.x = m_dragStart.x - GMouse::getMousePos().x;
			m_dragDistance.x = (Sint32)((std::fmaxl(m_minScreenSize.x, m_dragScreenSize.x + m_dragDistance.x) - m_dragScreenSize.x) * 2);
			m_dragScreenSize.x = (Sint32)std::fmaxl(m_minScreenSize.x, m_dragScreenSize.x + m_dragDistance.x / 2.f);
			m_windowPos.x = m_windowPos.x - m_dragDistance.x / 2;
			glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);

			m_dragScreenSize.y = (Sint32)std::fmaxl(m_minScreenSize.y, (GLfloat)(m_initWindowSize.y - (m_resizeMousePos.y - GMouse::getMousePos().y)));
			break;
		case ResizeType::TOP_LEFT:
			m_dragDistance.x = m_dragStart.x - GMouse::getMousePos().x;
			m_dragDistance.x = (Sint32)((std::fmaxl(m_minScreenSize.x, m_dragScreenSize.x + m_dragDistance.x) - m_dragScreenSize.x) * 2);
			m_dragScreenSize.x = (Sint32)std::fmaxl(m_minScreenSize.x, m_dragScreenSize.x + m_dragDistance.x / 2.f);
			m_windowPos.x = m_windowPos.x - m_dragDistance.x / 2;

			m_dragDistance.y = m_dragStart.y - GMouse::getMousePos().y;
			m_dragDistance.y = (Sint32)((std::fmaxl(m_minScreenSize.y, m_dragScreenSize.y + m_dragDistance.y) - m_dragScreenSize.y) * 2);
			m_dragScreenSize.y = (Sint32)std::fmaxl(m_minScreenSize.y, m_dragScreenSize.y + m_dragDistance.y / 2.f);
			m_windowPos.y = m_windowPos.y - m_dragDistance.y / 2;
			glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);
			break;
		default:
			break;
		}
		if (!(_size == m_dragScreenSize)) {
			m_resizeUpdate = true;
		}
	}
}
