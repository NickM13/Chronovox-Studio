#pragma once

#include "engine\utils\global\GScreen.h"
#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CList : public Component
{
private:
	Uint16 m_itemHeight;
	Uint16 m_maxVisible; // Max items visible, more efficient rendering but not necessarily needed?
	Sint16 m_scroll, m_maxScroll;
	Sint16 m_hoveredItem, m_selectedItem;
	Vector2<Uint16> m_mouseBuffer;
	bool m_hover, m_dragging;
	struct ListItem {
		std::string name;
		Sint8 state; // 0=Unselected, 1=Hovered, 2=Selected
		ListItem(std::string name = "", Sint8 state = 0) : name(name), state(state) {};
	};
	std::vector<ListItem> m_itemList;
public:
	CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight);

	Component* addItem(std::string p_itemName);
	Component* insertItem(Uint16 p_index, std::string p_itemName);
	void removeItem(Uint16 p_itemName);
	Uint16 getItemCount();
	ListItem &getItem(Uint16 p_index);
	std::vector<ListItem> getItemList() { return m_itemList; }
	void clear();

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();
};
