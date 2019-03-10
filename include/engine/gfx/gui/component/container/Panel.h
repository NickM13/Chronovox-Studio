#pragma once

#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class Panel : public Component {
public:
	Panel() {};
	Panel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_borderFlags);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_updateTime);
	void render();
};
