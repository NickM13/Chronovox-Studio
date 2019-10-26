#pragma once

#include "engine\utils\global\GScreen.h"
#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CListThick : public Component {
public:
	struct ListThickItem {
		std::string name;
		std::string path;
		std::string modified;
		GLfloat hoverTimer = 0;
		ListThickItem(std::string name = "");
	};
protected:
	Uint16 m_itemHeight = 0;
	GLfloat m_maxVisible = 0; // Max items visible, more efficient rendering but not necessarily needed?
	Sint16 m_scroll = 0, m_maxScroll = 0;
	Sint32 m_scrollBarHeight = 0;
	Sint16 m_hoveredItem = -1, m_selectedItem = -1;
	Vector2<Sint32> m_mouseBuffer = {};
	bool m_hover = false, m_dragging = false;
	std::vector<ListThickItem> m_itemList;
	Texture* m_listIcon;

	void renderItems();
public:
	CListThick(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight, Texture* p_listIcon);

	void resize();

	Component* addItem(std::string p_itemName);
	Component* insertItem(Uint16 p_index, std::string p_itemName);
	void removeItem(Uint16 p_itemName);
	Uint16 getItemCount();
	ListThickItem& getItem(Sint16 p_index);
	std::vector<ListThickItem> getItemList() { return m_itemList; }
	void clear();

	void selectItem(Sint16 p_index);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
};
