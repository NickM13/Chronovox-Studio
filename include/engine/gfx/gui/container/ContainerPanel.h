#pragma once

#include "Container.h"
#include "Panel.h"

// ContainerPanel is a [draggable] Container with scroll bars
class ContainerPanel : public Container
{
private:
	Sint8 m_lHeld, m_rHeld;
	Vector2<Sint32> m_mousePos;
	Vector2<Sint32> m_scroll;
	Vector2<Sint32> m_minScroll;
	Vector2<Sint32> m_maxScroll;
	bool m_draggable;
	bool m_scrollX, m_scrollY;
	Panel* m_panel;
public:
	ContainerPanel() {};
	ContainerPanel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme, Sint8 p_borderFlags, bool p_visible = true);
	~ContainerPanel();

	Component* addComponent(Component* p_component, Anchor p_posAnchor = NOANCHOR, Anchor p_sizeAnchor = NOANCHOR);
	void resize();

	void calcSize(Vector2<Sint32> p_offset = {});
	//Set whether scroll bar across bottom exists
	void setScrollX(bool p_state);
	//Set whether scroll bar across right exists
	void setScrollY(bool p_state);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_updateTime);
	void render();
};
