#include "engine\editor\Editor.h"

#include "engine\editor\menu\Overlay.h"

#include "engine\gfx\font\Font.h"
#include "engine\sfx\Sound.h"

#include <iostream>

Model* Editor::m_model = 0;

Editor::Editor()
{
	m_engineState = GAME;

	LTexture::init();
	Font::getInstance().loadFont("UI", "res\\segoeui.ttf", 10);
	Sound::getInstance().init();

	m_model = new Model();

	Overlay::init(m_model);
	m_mainGui.addComponent(Overlay::getContainer());
}
Editor::~Editor()
{
	Sound::getInstance().close();
	Font::getInstance().clean();
	delete m_model;
}

void Editor::resize()
{
	m_mainGui.removeComponent(Overlay::getContainer()->getName());
	Overlay::clear();
	Overlay::init(m_model);
	m_mainGui.addComponent(Overlay::getContainer());
}

void Editor::dropFile(const char* path)
{
	m_model->load(path);
}

void Editor::setEngineState(EngineState p_state)
{
	if(m_engineState != p_state)
	{
		m_engineState = p_state;
		switch(m_engineState)
		{
		case MENU:

			break;
		case GAME:

			break;
		}
	}
}

void Editor::renderMouse()
{
	if(Globals::m_tooltip != "")
	{
		Vector2<Sint32> _size = Font::getInstance().getMessageWidth(Globals::m_tooltip);
		Vector2<Sint32> _pos;
		_pos.x = min(Globals::m_screenSize.x - _size.x - 4, max(4, m_mouseBuffer.x - _size.x / 2));
		_pos.y = m_mouseBuffer.y - _size.y - 6;
		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glTranslatef(_pos.x, _pos.y, 0);
			glColor4f(0, 0, 0, 0.6f);
			glBegin(GL_QUADS);
			{
				glVertex2f(-4, -4);
				glVertex2f(_size.x + 5, -4);
				glVertex2f(_size.x + 5, _size.y + 5);
				glVertex2f(-4, _size.y + 5);
			}
			glEnd();
			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			{
				glVertex2f(-5, -4);
				glVertex2f(_size.x + 5, -4);
				glVertex2f(_size.x + 5, -5);
				glVertex2f(-5, -5);

				glVertex2f(_size.x + 5, -4);
				glVertex2f(_size.x + 5, _size.y + 5);
				glVertex2f(_size.x + 4, _size.y + 5);
				glVertex2f(_size.x + 4, -4);

				glVertex2f(_size.x + 5, _size.y + 5);
				glVertex2f(-4, _size.y + 5);
				glVertex2f(-4, _size.y + 4);
				glVertex2f(_size.x + 5, _size.y + 4);

				glVertex2f(-4, _size.y + 5);
				glVertex2f(-4, -4);
				glVertex2f(-5, -4);
				glVertex2f(-5, _size.y + 5);
			}
			glEnd();
			Font::getInstance().print(Globals::m_tooltip, 0, Font::getInstance().getHeight() / 2);
		}
		glPopMatrix();
	}
	Globals::m_tooltip = "";
}

void Editor::input()
{
	Vector2<Sint32> _mouseMoved = MouseStates::m_mousePos - m_mouseBuffer;
	m_mouseBuffer = MouseStates::m_mousePos;
	MouseStates::m_guiMousePos = MouseStates::m_mousePos;

	switch(m_engineState)
	{
	case MENU:

		break;
	case GAME:
		Sint8 _interact = Component::EventFlag::EVENT_ALL;

		m_mainGui.input(_interact, KeyStates::m_keyStates, MouseStates::m_mouseStates, MouseStates::m_mousePos);
		m_model->input(_interact);

		if(!Overlay::getContainer()->isPaused() && (_interact & Component::EventFlag::EVENT_MOUSEOVER))
		{
			if(MouseStates::m_mouseStates[GLFW_MOUSE_BUTTON_RIGHT] & MouseStates::MOUSE_DOWN)
			{
				if(KeyStates::m_keyStates[GLFW_KEY_LEFT_SHIFT])	m_model->pan(_mouseMoved);
				else m_model->turn(_mouseMoved);
			}

			m_model->zoom(MouseStates::m_mouseScroll);
		}
		if(!Overlay::getContainer()->isPaused())
		{
			if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_Z] & KeyStates::KEY_PRESS) m_model->undo();
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_C] & KeyStates::KEY_PRESS) m_model->copyMatrix();
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_V] & KeyStates::KEY_PRESS) m_model->pasteMatrix();
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_Y] & KeyStates::KEY_PRESS) m_model->redo();
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_S] & KeyStates::KEY_PRESS) m_model->save();
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_O] & KeyStates::KEY_PRESS) m_model->open();
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_G] & KeyStates::KEY_PRESS) m_model->toggleGrid();
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_H] & KeyStates::KEY_PRESS) m_model->toggleOutline();
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_T] & KeyStates::KEY_PRESS) Overlay::getContainer()->setPauseScreen("RESIZE");
			else if(KeyStates::m_keyStates[GLFW_KEY_LEFT_CONTROL] && KeyStates::m_keyStates[GLFW_KEY_N] & KeyStates::KEY_PRESS) Overlay::getContainer()->setPauseScreen("NEW");
			else if(KeyStates::m_keyStates[GLFW_KEY_SPACE] & KeyStates::KEY_PRESS) m_model->focus();
			else if(KeyStates::m_keyStates[GLFW_KEY_DELETE] & KeyStates::KEY_PRESS) m_model->removeMatrix();
			else if(KeyStates::m_keyStates[GLFW_KEY_B] & KeyStates::KEY_PRESS) m_model->setTool(ADD);
			else if(KeyStates::m_keyStates[GLFW_KEY_E] & KeyStates::KEY_PRESS) m_model->setTool(ERASE);
			else if(KeyStates::m_keyStates[GLFW_KEY_R] & KeyStates::KEY_PRESS) m_model->setTool(REPLACE);
			else if(KeyStates::m_keyStates[GLFW_KEY_K] & KeyStates::KEY_PRESS) m_model->setTool(EYEDROP);
			else if(KeyStates::m_keyStates[GLFW_KEY_S] & KeyStates::KEY_PRESS) m_model->setTool(SELECT);
			else if(KeyStates::m_keyStates[GLFW_KEY_M] & KeyStates::KEY_PRESS) m_model->setTool(MOVE);
			else if(KeyStates::m_keyStates[GLFW_KEY_T] & KeyStates::KEY_PRESS) m_model->setTool(RESIZE);
			else if(KeyStates::m_keyStates[GLFW_KEY_1] & KeyStates::KEY_PRESS) m_model->setToolMeta(0);
			else if(KeyStates::m_keyStates[GLFW_KEY_2] & KeyStates::KEY_PRESS) m_model->setToolMeta(1);
			else if(KeyStates::m_keyStates[GLFW_KEY_3] & KeyStates::KEY_PRESS) m_model->setToolMeta(2);
		}
		break;
	}
}

void Editor::update()
{
	m_deltaUpdate = min(0.5f, GLfloat(glfwGetTime() - m_lastUpdate));
	m_lastUpdate = GLfloat(glfwGetTime());

	Globals::m_deltaTime = m_deltaUpdate;

	switch(m_engineState)
	{
	case MENU:

		break;
	case GAME:
		m_model->update(m_deltaUpdate);
		m_mainGui.update(m_deltaUpdate);
		break;
	}
}

void Editor::render3d()
{
	switch(m_engineState)
	{
	case MENU:

		break;
	case GAME:
		m_model->renderSkyBox();
		m_model->render();
		break;
	}
}
void Editor::render2d()
{
	switch(m_engineState)
	{
	case MENU:

		break;
	case GAME:
		Font::getInstance().setAlignment(Alignment::ALIGN_LEFT);
		m_mainGui.render();
		renderMouse();
		break;
	}
}
