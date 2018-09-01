#pragma once
#include "..\container\Container.h"

class Dialog : public Container {
public:
	Dialog() {};
	Dialog(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_updateTime);
	void render();
};
