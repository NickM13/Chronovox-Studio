#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include "engine\sfx\Sound.h"

class CButtonToggle : public Component
{
public:
	CButtonToggle(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state = 0, function p_func = 0);
	CButtonToggle(std::string p_compName, Texture p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state = 0, function p_func = 0);
	CButtonToggle(std::string p_compName, Texture p_activeTex, Texture p_inactiveTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state = 0, function p_func = 0);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();
private:
	Texture m_buttonTex[2];
	Sint8 m_texType;

	bool m_hover;

	Source m_soundClick;
	Source m_soundHover;
};
