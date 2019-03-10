#pragma once

#include "Container.h"
#include "Panel.h"

// ContainerPanel is a [draggable] Container with scroll bars
class ContainerPanel : public Container {
private:
	Sint8 m_lHeld = 0, m_rHeld = 0;
	Vector2<Sint32> m_mousePos = {};
	Vector2<Sint32> m_scroll = {};
	Vector2<Sint32> m_minScroll = {};
	Vector2<Sint32> m_maxScroll = {};
	bool m_draggable = false;
	bool m_scrollX = false, m_scrollY = false;
	Panel* m_panel = 0;
public:
	ContainerPanel() {};
	ContainerPanel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_borderFlags);
	~ContainerPanel();

	Component* addComponent(Component* p_component, Anchor p_posAnchor = Anchor::NONE, Anchor p_sizeAnchor = Anchor::NONE);
	void resize();

	void calcSize(Vector2<Sint32> p_offset = {});
	//Set whether scroll bar across bottom exists
	void setScrollX(bool p_state);
	//Set whether scroll bar across right exists
	void setScrollY(bool p_state);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_updateTime);
	void render();
};
