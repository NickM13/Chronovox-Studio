#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CButton : public Component {
public:
	enum RenderStyle {
		EMPTY = 0,
		FILL = 1,
		BORDER = 2,
		ALL = 3
	};
protected:
	RenderStyle m_renderStyle = {};

	bool m_stuck = false;
public:
	CButton(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, RenderStyle p_borderStyle, function p_func = 0);
	CButton(std::string p_compName, std::string p_title, Texture* p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, RenderStyle p_borderStyle, function p_func = 0);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	virtual void render();

	void setState(Sint8 p_selected);

	Sint8 isSelected(); // 0 = not selected, 1 = pressed, 2 = is held, 3 = released
	bool isHovered() { return m_hovered; }
	void setHover(bool p_state) { m_hovered = p_state; }
};
