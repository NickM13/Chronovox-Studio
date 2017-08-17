#include "engine\utils\global\GScreen.h"
#include "engine\utils\global\event\GMouse.h"

std::string GScreen::m_tooltip = "";
float GScreen::m_fps = 0;
float GScreen::m_fov = 0;
float GScreen::m_deltaTime = 0;
Sint8 GScreen::m_exitting = 0;
bool GScreen::m_developer = false;

GLFWwindow *GScreen::m_window = 0;
Vector2<Sint32> GScreen::m_monitorSize = {};
Vector2<Sint32> GScreen::m_windowPos = {};
bool GScreen::m_maximized = false;
Vector2<Sint32> GScreen::m_screenSize = {};
Vector2<Sint32> GScreen::m_smallScreen = {};
bool GScreen::m_draggingWindow = false;
Vector2<Sint32> GScreen::m_dragStart = {};
Vector2<Sint32> GScreen::m_dragDistance = {};

GScreen::WindowCommand GScreen::m_windowCommand = NONE;

void GScreen::initWindow(GLFWwindow *p_window)
{
	m_window = p_window;

	int *x = new int(), *y = new int();
	glfwGetWindowPos(m_window, x, y);
	m_windowPos = Vector2<Sint32>(*x, *y);

	glfwGetWindowSize(m_window, x, y);
	m_smallScreen = m_screenSize = Vector2<Sint32>(*x, *y);
}

void GScreen::startWindowDrag()
{
	m_draggingWindow = true;
	m_dragStart = GMouse::m_guiMousePos;

}
bool GScreen::isDraggingWindow()
{
	return m_draggingWindow;
}
void GScreen::endWindowDrag()
{
	if(m_draggingWindow)
	{
		m_draggingWindow = false;
		m_dragDistance = {};
	}
}

void GScreen::updateWindow()
{
	if(isDraggingWindow() && !isMaximized())
	{
		m_dragDistance = m_dragStart - GMouse::m_guiMousePos;
		m_windowPos = m_windowPos - m_dragDistance;
		glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);
		m_dragStart = GMouse::m_guiMousePos + m_dragDistance;
	}
}
