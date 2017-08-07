#include "Application.h"

Editor* Application::m_editor = 0;
Vector2<Uint16> Application::m_screenSize = {};
GLFWwindow* Application::m_mainWindow = 0;

bool Application::init()
{
	Globals::m_fov = 70;
	m_maxFps = 60;
	m_screenSize = Vector2<Uint16>(1280, 768);

	Globals::m_developer = true;
	Globals::m_fps = 0;

	Globals::m_gameState = Globals::GAME_PLAY;
	Globals::m_exitting = 0;

	for(Uint16 i = 0; i < 1024; i++)
		KeyStates::m_keyStates[i] = 0;

	if(!glfwInit())
		return false;

	if(!glewInit())
		return false;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, false);

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	m_mainWindow = glfwCreateWindow(min(m_screenSize.x, mode->width), min(m_screenSize.y, mode->height), "Voxel Engine Indev v0.1", 0, 0);

	glfwSetWindowPos(m_mainWindow, (mode->width - m_screenSize.x) / 2, (mode->height - m_screenSize.y) / 2);


	Globals::m_screenSize = m_screenSize;

	if(!m_mainWindow)
	{
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
	m_editor->resize();

	return true;
}

void Application::close()
{
	glfwDestroyWindow(m_mainWindow);
	glfwTerminate();
	delete m_editor;
}

void Application::keyCallback(GLFWwindow* p_window, int p_keycode, int p_scancode, int p_action, int p_mods)
{
	//std::cout << p_keycode << " " << p_scancode << " " << p_action << " " << p_mods << "\n";
	KeyStates::m_keyEvents.push_back({p_keycode, p_scancode, p_action, p_mods});

	if(Globals::m_developer && p_keycode == '`')
	{
		Globals::m_exitting = 2;
		return;
	}

	if(p_action)
	{
		KeyStates::m_keyStates[p_keycode] = KeyStates::KEY_PRESS;
		KeyStates::m_keyStates[p_keycode] += KeyStates::KEY_HOLD;
	}
	else
	{
		KeyStates::m_keyStates[p_keycode] = KeyStates::KEY_RELEASE;
	}
}

void Application::mousePressCallback(GLFWwindow* p_window, int p_button, int p_state, int p_mods)
{
	if(!(MouseStates::m_mouseStates[p_button]) != !(p_state))
	{
		if(MouseStates::m_mouseStates[p_button] == 0)
		{
			if(MouseStates::m_mouseDelay[p_button] > 0)
			{
				MouseStates::m_mouseStates[p_button] = MouseStates::MOUSE_PRESS | MouseStates::MOUSE_DOWN | MouseStates::MOUSE_DOUBLECLICK;
				MouseStates::m_mouseDelay[p_button] = 0;
			}
			else
			{
				MouseStates::m_mouseStates[p_button] = MouseStates::MOUSE_PRESS | MouseStates::MOUSE_DOWN;
				MouseStates::m_mouseDelay[p_button] = 0.25f;
			}
		}
		else
			MouseStates::m_mouseStates[p_button] = MouseStates::MOUSE_RELEASE;
	}
}

void Application::mouseMovedCallback(GLFWwindow* p_window, double p_x, double p_y)
{
	Sint32 x = p_x, y = p_y;
	if(MouseStates::m_mousePos.x != x && MouseStates::m_mousePos.y != y)
		MouseStates::m_mouseMoved = true;
	else
		MouseStates::m_mouseMoved = false;
	MouseStates::m_mousePos = Vector2<Sint32>(x, y);
}

void Application::mouseScrollCallback(GLFWwindow* p_window, double p_x, double p_y)
{
	MouseStates::m_mouseScroll += Sint8(p_y);
}

void Application::windowResizeCallback(GLFWwindow* p_window, int p_x, int p_y)
{
	Globals::m_screenSize = Vector2< Uint16 >(p_x, p_y);
	m_screenSize = Vector2< Uint16 >(p_x, p_y);

	glViewport(0, 0, m_screenSize.x, m_screenSize.y);

	init2d();

	m_editor->resize();
}

void Application::mouseEnterCallback(GLFWwindow* p_window, int p_action) {}

void Application::dropFileCallback(GLFWwindow* p_window, int count, const char** paths)
{
	m_editor->dropFile(paths[0]);
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
	gluPerspective(Globals::m_fov, GLfloat(m_screenSize.x) / m_screenSize.y, 0.01f, 2000.f);
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
	while(Globals::m_exitting != 2)
	{
		i = glfwGetTime();

		/*
	#ifdef _DEBUG
		if(Globals::m_keyStates[GLFW_KEY_1] & KeyStates::KEY_PRESS)
			DebugBreak();
	#endif
		*/

		input();
		update();
		render();

		m_sleepTime = DWORD(max(1000 / m_maxFps - ((glfwGetTime() - i) * 1000), 0));
		if(m_sleepTime > 0)
			Sleep(m_sleepTime);
		Globals::m_fps = 1.f / GLfloat(glfwGetTime() - i);
	}

	glfwTerminate();
}

void Application::input()
{
	if(glfwWindowShouldClose(m_mainWindow))
	{
		Globals::m_exitting = 2;
	}

	KeyStates::m_keyEvents.clear();
	MouseStates::m_mouseScroll = 0;

	for(Uint16 i = 0; i < 1024; i++)
	{
		if(KeyStates::m_keyStates[i] & KeyStates::KEY_PRESS)
			KeyStates::m_keyStates[i] -= KeyStates::KEY_PRESS;
		if(KeyStates::m_keyStates[i] & KeyStates::KEY_RELEASE)
			KeyStates::m_keyStates[i] -= KeyStates::KEY_RELEASE;
	}

	glfwPollEvents();

	m_editor->input();
}

GLfloat _last = 0;
void Application::update()
{
	m_editor->update();

	GLfloat _curr = glfwGetTime();
	for(Uint16 i = 0; i < 64; i++)
	{
		if(MouseStates::m_mouseStates[i] & MouseStates::MOUSE_PRESS)
			MouseStates::m_mouseStates[i] -= MouseStates::MOUSE_PRESS;
		if(MouseStates::m_mouseStates[i] & MouseStates::MOUSE_RELEASE)
			MouseStates::m_mouseStates[i] = 0;
		if(MouseStates::m_mouseDelay[i] > 0)
			MouseStates::m_mouseDelay[i] -= (_curr - _last);
	}
	_last = glfwGetTime();

	if(Globals::m_windowCommand == Globals::MINIMIZE) glfwIconifyWindow(m_mainWindow);
	//if(Globals::m_windowCommand == Globals::RESIZE) glfwSetWindowSize(m_mainWindow);
	if(Globals::m_windowCommand == Globals::CLOSE) glfwSetWindowShouldClose(m_mainWindow, true);
	Globals::m_windowCommand = Globals::NONE;

	glfwSetWindowTitle(m_mainWindow, std::string("Voxel Engine Indev 0.1").c_str());
}

void Application::render()
{
	glLoadIdentity();

	init3d();
	m_editor->render3d();
	init2d();
	m_editor->render2d();

	glfwSwapBuffers(m_mainWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
