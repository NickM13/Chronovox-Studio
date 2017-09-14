#pragma once

#include "engine\utils\global\GScreen.h"
#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CList : public Component
{
private:
	Uint16 m_itemHeight;
	Uint16 m_maxVisible; // Max items visible

	Sint16 m_scroll, m_maxScroll;

	Vector2<Uint16> m_mouseBuffer;

	bool m_hover, m_dragging;

	std::vector<Sint8> *m_selectionStates;
	std::vector<std::string> *m_itemList;
public:
	CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight, std::vector<Sint8> *p_selectionStates, std::vector<std::string> *p_itemList);

	Component* addItem(std::string p_itemName);
	void removeItem(Uint16 p_index);
	void setItem(Uint16 p_index, std::string p_itemName);
	Uint16 getItemCount();
	std::string getItem(Uint16 p_index);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();
};
