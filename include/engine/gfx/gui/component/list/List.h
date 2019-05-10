#pragma once

#include "engine\utils\global\GScreen.h"
#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CList : public Component {
protected:
	Uint16 m_itemHeight = 0;
	GLfloat m_maxVisible = 0; // Max items visible, more efficient rendering but not necessarily needed?
	Sint16 m_scroll = 0, m_maxScroll = 0;
	Sint32 m_scrollBarHeight = 0;
	Sint16 m_hoveredItem = -1, m_selectedItem = -1, m_selectedItemCtrl = -1;
	Texture* m_visibleTexture = 0, * m_invisibleTexture = 0;
	Vector2<Sint32> m_mouseBuffer = {};
	bool m_hover = false, m_dragging = false;
	struct ListItem {
		std::string name;
		bool visible = true;
		GLfloat hoverTimer = 0;
		Sint8 state; // 0=Unselected, 1=Hovered, 2=Selected
		ListItem(std::string name = "", Sint8 state = 0, bool visible = true) : name(name), state(state), visible(visible) {};
	};
	std::vector<ListItem> m_itemList;

	void renderItems();
public:
	CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight);

	void resize();

	Component* addItem(std::string p_itemName, bool p_isVisible);
	Component* insertItem(Uint16 p_index, std::string p_itemName);
	void removeItem(Uint16 p_itemName);
	Uint16 getItemCount();
	ListItem &getItem(Uint16 p_index);
	std::vector<ListItem> getItemList() { return m_itemList; }
	void clear();

	void selectItem(Sint16 id);
	void toggleVisiblity(Sint16 id);
	void setSelectedItem(Sint16 id) { m_selectedItem = id; }
	Sint16 getSelectedItem() { return m_selectedItem; }

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
};
