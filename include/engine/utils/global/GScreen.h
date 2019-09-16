#pragma once

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\LOpenGL.h"
#include <vector>
#include <string>
#include <iostream>

class GScreen {
public:
	enum class WindowCommand {
		NONE = 0,
		CLOSE,
		RESIZE,
		MINIMIZE
	};

	enum class ResizeType {
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		LEFT,
		RIGHT,
		BOTTOM_LEFT,
		BOTTOM,
		BOTTOM_RIGHT
	};
private:
	static std::string m_windowTitle;
	static std::string m_appName;
	static std::string m_appVersion;
	static GLfloat m_fps;
	static GLfloat m_fov;
	static GLfloat m_deltaUpdate;
	static Sint8 m_exitting;
	static bool m_developer;

	static GLFWwindow *m_window;
	static Vector2<Sint32> m_monitorSize;
	static Vector2<Sint32> m_windowPos;
	static bool m_maximized;
	static bool m_iconified;
	static bool m_focused;
	static Vector2<Sint32> m_screenSize;
	static Vector2<Sint32> m_smallScreen;
	static Vector2<Sint32> m_minScreenSize;
	static Vector2<Sint32> m_dragScreenSize;
	static bool m_draggingWindow;
	static Vector2<Sint32> m_dragStart;
	static Vector2<Sint32> m_dragDistance;
	static Vector2<Sint32> m_initWindowSize;
	static Vector2<Sint32> m_resizeMousePos;
	static bool m_resizing;
	static bool m_finishedResize;
	static bool m_resizeUpdate;
	static bool m_shadows;

	static WindowCommand m_windowCommand;
	static ResizeType m_resizeType;

public:
	static GLFWwindow* getGLFWWindow() { return m_window; }

	static void setWindowTitle(std::string p_windowTitle) { m_windowTitle = p_windowTitle; }
	static std::string getWindowTitle() { return m_windowTitle; }

	static void setAppName(std::string p_appName) { m_appName = p_appName; }
	static std::string getAppName() { return m_appName; }

	static void setAppVersion(std::string p_appVersion) { m_appVersion = p_appVersion; }
	static std::string getAppVersion() { return m_appVersion; }

	static void setDeveloper(bool p_developer) { m_developer = p_developer; }
	static bool isDeveloper() { return m_developer; }

	static void setScreenSize(Vector2<Sint32> p_screenSize) { m_screenSize = p_screenSize; }
	static Vector2<Sint32> getScreenSize() { return m_screenSize; }
	static Vector2<GLfloat> getScreenSizeInverse() { return Vector2<GLfloat>(m_screenSize) / static_cast<GLfloat>(m_screenSize.y); }

	static void setSmallScreenSize(Vector2<Sint32> p_screenSize) { m_smallScreen = p_screenSize; }
	static Vector2<Sint32> getSmallScreenSize() { return m_smallScreen; }

	static void setMinScreenSize(Vector2<Sint32> p_screenSize) { m_minScreenSize = p_screenSize; }
	static Vector2<Sint32> getMinScreenSize() { return m_minScreenSize; }

	static void setDragScreenSize(Vector2<Sint32> p_dragScreenSize) { m_dragScreenSize = p_dragScreenSize; }
	static Vector2<Sint32> getDragScreenSize() { return m_dragScreenSize; }

	static bool isMaximized() { return m_maximized; }
	static void setMaximized(bool p_maximized) { m_maximized = p_maximized; }

	static void setFov(GLfloat p_fov) { m_fov = p_fov; }
	static GLfloat getFov() { return m_fov; }

	static void setFps(GLfloat p_fps) { m_fps = p_fps; }
	static GLfloat getFps() { return m_fps; }

	static void enableShadows(bool p_shadows) { m_shadows = p_shadows; }
	static bool hasShadows() { return m_shadows; }

	static void setFocused(bool p_focused) { m_focused = p_focused; }
	static bool isFocused() { return m_focused; }

	static void setIconified(bool p_iconified) { m_iconified = p_iconified; }
	static bool isIconified() { return m_iconified; }

	static void setExitting(Sint8 p_exitting) { m_exitting = p_exitting; }
	static Sint8 isExitting() { return m_exitting; }

	static void setDeltaUpdate(GLfloat p_deltaUpdate) { m_deltaUpdate = p_deltaUpdate; }
	static GLfloat getDeltaUpdate() { return m_deltaUpdate; }

	static void setWindowCommand(WindowCommand p_wincmd) { m_windowCommand = p_wincmd; }
	static WindowCommand getWindowCommand() { return m_windowCommand; }

	static void setResizeType(ResizeType p_restype) { m_resizeType = p_restype; }
	static ResizeType getResizeType() { return m_resizeType; }

	static void initWindow(GLFWwindow *p_window);

	static void windowIconifyCallback(GLFWwindow* window, int iconified);
	static void windowFocusCallback(GLFWwindow* window, int focused);

	static Vector2<Sint32> getDragStart() { return m_dragStart; }
	static void startWindowDrag();
	static bool isDraggingWindow();
	static void endWindowDrag();

	static void startResizing(ResizeType p_resizeType);
	static bool isFinishedResizing() { return m_finishedResize; }
	static bool hasResizeUpdate() { return m_resizeUpdate; }
	static void stopResizing();

	static void updateWindow();
};
