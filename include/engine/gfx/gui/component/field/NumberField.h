#pragma once

#include "engine\utils\global\GScreen.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include <iostream>

struct NumberField : public Component {
public:
	enum NumType {
		INT = 0,
		FLOAT = 1
	};
private:
	std::string m_numText;
	Vector2<GLfloat> m_numBounds;
	NumType m_numType;
	GLfloat m_prevValue;
	bool m_positive;
public:
	NumberField(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Vector2<GLfloat> p_numBounds, NumType p_numType);

	void setSize(Vector2<Sint32> p_size) {}
	void setValue(GLfloat p_value);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
};
