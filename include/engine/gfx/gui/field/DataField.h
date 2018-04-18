#pragma once

#include "TextField.h"

struct DataField : public Component
{
public:
	DataField(std::string p_compName, std::string* p_dataString, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme)
		: Component(p_compName, "", p_pos, p_size, p_colorTheme)
	{ m_dataString = p_dataString; }

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
private:
	std::string* m_dataString;
};
