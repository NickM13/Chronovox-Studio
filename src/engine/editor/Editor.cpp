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
	Font::loadFont("LeelawUI", "res\\font\\leelawui.ttf", 9);
	Font::loadFont("SegoeUI", "res\\font\\segoeui.ttf", 9);
	Sound::getInstance().init();

	m_model = new Model();

	Overlay::init(m_model);
	m_mainGui.addComponent(Overlay::getContainer());
}
Editor::~Editor()
{
	Sound::getInstance().close();
	Font::clean();
	delete m_model;
}

void Editor::resize(bool p_maximizedByDrag)
{
	m_mainGui.removeComponent("GUI_OVERLAY");
	Overlay::clear();
	Overlay::init(m_model);
	m_mainGui.addComponent(Overlay::getContainer());

	if(p_maximizedByDrag)
		m_mainGui.findComponent("GUI_OVERLAY\\TITLE\\DRAGBAR")->setState(1);
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
	if(GScreen::m_tooltip != "")
	{
		Vector2<Sint32> _size = Font::getMessageWidth(GScreen::m_tooltip);
		Vector2<Sint32> _pos;
		_pos.x = min(GScreen::m_screenSize.x - _size.x - 4, max(4, m_mouseBuffer.x + 5));
		_pos.y = m_mouseBuffer.y + 26;
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
			Font::setAlignment(Alignment::ALIGN_LEFT);
			Font::print(GScreen::m_tooltip, 0, Font::getHeight() / 2);
		}
		glPopMatrix();
		GScreen::m_tooltip = "";
	}
}

void Editor::input()
{
	Vector2<Sint32> _mouseMoved = GMouse::m_mousePos - m_mouseBuffer;
	m_mouseBuffer = GMouse::m_mousePos;
	GMouse::m_guiMousePos = GMouse::m_mousePos;

	switch(m_engineState)
	{
	case MENU:

		break;
	case GAME:
		Sint8 _interact = Component::EventFlag::EVENT_ALL;

		m_mainGui.input(_interact, GKey::m_keyStates, GMouse::m_mouseStates, GMouse::m_mousePos);
		m_model->input(_interact);

		if(!Overlay::getContainer()->isPaused() && (_interact & Component::EventFlag::EVENT_MOUSEOVER))
		{
			if(GMouse::m_mouseStates[GLFW_MOUSE_BUTTON_RIGHT] & GMouse::MOUSE_DOWN)
			{
				if(GKey::m_keyStates[GLFW_KEY_LEFT_SHIFT])	m_model->pan(_mouseMoved);
				else m_model->turn(_mouseMoved);
			}

			m_model->zoom(GMouse::m_mouseScroll);
		}
		if(!Overlay::getContainer()->isPaused() && (_interact & Component::EventFlag::EVENT_KEYPRESS))
		{
			if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_Z] & GKey::KEY_PRESS) m_model->undo();
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_C] & GKey::KEY_PRESS) m_model->copyMatrix();
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_V] & GKey::KEY_PRESS) m_model->pasteMatrix();
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_Y] & GKey::KEY_PRESS) m_model->redo();
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_S] & GKey::KEY_PRESS) m_model->save();
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_O] & GKey::KEY_PRESS) m_model->open();
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_G] & GKey::KEY_PRESS) m_model->toggleGrid();
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_H] & GKey::KEY_PRESS) m_model->toggleOutline();
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_T] & GKey::KEY_PRESS) Overlay::getContainer()->setPauseScreen("RESIZE");
			else if(GKey::m_keyStates[GLFW_KEY_LEFT_CONTROL] && GKey::m_keyStates[GLFW_KEY_N] & GKey::KEY_PRESS) Overlay::getContainer()->setPauseScreen("NEW");
			else if(GKey::m_keyStates[GLFW_KEY_SPACE] & GKey::KEY_PRESS) m_model->focus();
			else if(GKey::m_keyStates[GLFW_KEY_DELETE] & GKey::KEY_PRESS) m_model->removeMatrix();
			else if(GKey::m_keyStates[GLFW_KEY_B] & GKey::KEY_PRESS) m_model->setTool(ADD);
			else if(GKey::m_keyStates[GLFW_KEY_E] & GKey::KEY_PRESS) m_model->setTool(ERASE);
			else if(GKey::m_keyStates[GLFW_KEY_R] & GKey::KEY_PRESS) m_model->setTool(REPLACE);
			else if(GKey::m_keyStates[GLFW_KEY_K] & GKey::KEY_PRESS) m_model->setTool(EYEDROP);
			else if(GKey::m_keyStates[GLFW_KEY_S] & GKey::KEY_PRESS) m_model->setTool(SELECT);
			else if(GKey::m_keyStates[GLFW_KEY_M] & GKey::KEY_PRESS) m_model->setTool(MOVE);
			else if(GKey::m_keyStates[GLFW_KEY_T] & GKey::KEY_PRESS) m_model->setTool(RESIZE);
			else if(GKey::m_keyStates[GLFW_KEY_1] & GKey::KEY_PRESS) m_model->setToolMeta(0);
			else if(GKey::m_keyStates[GLFW_KEY_2] & GKey::KEY_PRESS) m_model->setToolMeta(1);
			else if(GKey::m_keyStates[GLFW_KEY_3] & GKey::KEY_PRESS) m_model->setToolMeta(2);
		}
		break;
	}
}

void Editor::update()
{
	m_deltaUpdate = min(0.5f, GLfloat(glfwGetTime() - m_lastUpdate));
	m_lastUpdate = GLfloat(glfwGetTime());

	GScreen::m_deltaTime = m_deltaUpdate;

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
		Font::setAlignment(Alignment::ALIGN_LEFT);
		m_mainGui.render();
		renderMouse();
		break;
	}
}
