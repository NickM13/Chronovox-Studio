#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include "engine\sfx\Sound.h"

class CButton : public Component
{
public:
	CButton(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_showBorder, function p_func = 0);
	CButton(std::string p_compName, std::string p_title, Texture p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_showBorder, function p_func = 0);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	virtual void render();

	void setState(Sint8 p_selected);

	Sint8 isSelected(); // 0 = not selected, 1 = JUST pressed, 2 = is held, 3 = JUST released
	bool isHovered() {return m_hover;};
	void setHover(bool p_state) {m_hover = p_state;};
protected:
	Texture m_buttonTex;

	bool m_hover;
	bool m_stuck;

	Source m_soundClick;
	Source m_soundHover;
};
