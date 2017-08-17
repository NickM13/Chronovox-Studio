#pragma once

#include "engine\utils\global\GScreen.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include <iostream>

struct FileField : public Component
{
public:
	FileField(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_colorTheme = 0, bool p_limitField = true);

	void setScroll(Vector2<Sint32> p_scroll);
	void setScrolling(bool p_state);

	void setTitle(std::string p_title);
	void splitTitle();
	void mergeTitle();
	std::string getTitle();

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();
private:
	std::string m_blankField;
	Vector2<Sint32> m_cursorPos;

	Vector2<Sint32> m_scroll;
	bool m_scrolling;
	std::vector<std::string> m_text;

	bool m_limited;
	Vector2<Sint32> m_limitSize;
};
