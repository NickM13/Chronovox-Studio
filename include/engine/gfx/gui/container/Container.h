#pragma once

#define PANEL_ALIGN_NONE				0
#define PANEL_ALIGN_CENTER				1
#define PANEL_ALIGN_LEFT				2
#define PANEL_ALIGN_RIGHT				3
#define PANEL_ALIGN_TOP					4
#define PANEL_ALIGN_BOTTOM				5
#define PANEL_ALIGN_TOP_LEFT			6
#define PANEL_ALIGN_TOP_RIGHT			7
#define PANEL_ALIGN_BOTTOM_LEFT			8
#define PANEL_ALIGN_BOTTOM_RIGHT		9

#include "..\Component.h"
#include "engine\gfx\font\Font.h"

#include "engine\utils\global\GScreen.h"
#include <algorithm>


class Container : public Component
{
protected:
	struct Comp
	{
		Sint8 m_alignment;
		Component* m_component;
		Comp(Sint8 p_align, Component* p_comp) : m_alignment(p_align), m_component(p_comp)
		{};
	};
	std::vector<Comp> m_componentList;
	Vector4<Sint32> m_contentArea;

	std::vector<Component*> m_pauseScreens;
	Sint16 m_currentPause = -1;
public:
	Container() {};
	Container(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_visible);
	~Container();

	virtual Component* addComponent(Component* p_component, Sint8 p_alignment = PANEL_ALIGN_NONE);
	Component* findComponent(std::string p_compName);
	void removeComponent(std::string p_compName);

	Component* setVisible(bool p_visible);

	Vector2<Sint32> getRealPosition();
	Vector2<Sint32> getRealSize();

	Component* addPauseScreen(Component* p_comp, Sint8 p_alignment = PANEL_ALIGN_NONE)
	{
		m_pauseScreens.push_back(addComponent(p_comp, p_alignment));
		return p_comp;
	}
	Component* setPauseScreen(std::string p_compName)
	{
		if(m_currentPause != -1)
			m_pauseScreens[m_currentPause]->setVisible(false);
		if(p_compName != "")
		{
			for(Uint16 i = 0; i < m_pauseScreens.size(); i++)
			{
				if(m_pauseScreens[i]->getName() == p_compName)
				{
					m_currentPause = i;
					m_pauseScreens[i]->setVisible(true);
					return m_pauseScreens[i];
				}
			}
		}
		m_currentPause = -1;
		return 0;
	}
	bool isPaused()
	{
		return m_currentPause != -1;
	}

	void updateSize();

	void input();
	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_updateTime);
	void render();
};
