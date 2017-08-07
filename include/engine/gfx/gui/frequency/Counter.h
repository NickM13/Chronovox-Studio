#pragma once

#include "..\button\Button.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

struct CCounter : public Component
{
private:
	CButton* m_buttonAdd, *m_buttonSubtract;
public:
	CCounter(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32& p_currValue);
	~CCounter();

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();

	void setPos(Vector2<Sint32> p_pos);
	Vector2<Sint32> getPos();
};
