#pragma once

#include "..\Component.h"
#include "engine\gfx\font\Font.h"

#include "engine\utils\global\GScreen.h"
#include <algorithm>
#include <map>
#include <thread>

class Container : public Component
{
protected:
	struct Comp {
		Anchor m_posAnchor;
		Anchor m_sizeAnchor;
		Component* m_component;
		Comp() { m_component = 0; }
		Comp(Anchor p_posAnchor, Anchor p_sizeAnchor, Component* p_comp) : m_posAnchor(p_posAnchor), m_sizeAnchor(p_sizeAnchor), m_component(p_comp) {};
	};
	std::map<std::string, Comp> m_componentMap;
	std::vector<std::string> m_componentOrder;
	std::map<std::string, Component*> m_pauseScreens;
	Vector4<Sint32> m_contentArea;
	std::string m_currentPause = "";
	void sortInComponent(Comp comp);
public:
	Container() {};
	Container(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_visible);
	virtual ~Container();

	virtual Component* addComponent(Component* p_component, Anchor p_posAnchor = NOANCHOR, Anchor p_sizeAnchor = NOANCHOR);
	Component* findComponent(std::string p_compName);
	void removeComponent(std::string p_compName);

	Component* setVisible(bool p_visible);

	void resize();
	Vector2<Sint32> getRealPosition();
	Vector2<Sint32> getRealSize();

	Component* addPauseScreen(Component* p_comp, Anchor p_posAnchor = NOANCHOR, Anchor p_sizeAnchor = NOANCHOR) {
		m_pauseScreens.insert({p_comp->getName(), addComponent(p_comp, p_posAnchor, p_sizeAnchor)});
		return p_comp;
	}
	Component* setPauseScreen(std::string p_compName)
	{
		if(m_currentPause != "")
			m_pauseScreens[m_currentPause]->setVisible(false);
		if(p_compName != "") {
			Component *c = m_pauseScreens[p_compName];
			if(c) {
				m_currentPause = p_compName;
				c->setVisible(true);
				return c;
			}
		}
		m_currentPause = "";
		return 0;
	}
	bool isPaused() { return m_currentPause != ""; }

	void updateSize();

	void input();
	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_updateTime);
	void render();
};
