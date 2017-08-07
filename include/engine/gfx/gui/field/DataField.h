#pragma once

#include "TextField.h"

struct DataField : public Component
{
public:
	DataField(std::string p_compName, std::string* p_dataString, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme)
	{
		m_compName = p_compName;
		m_dataString = p_dataString;
		m_colorTheme = m_colorThemes[p_colorTheme];
		m_pos = p_pos;
		m_size = p_size;
	}

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();
private:
	std::string* m_dataString;
};
