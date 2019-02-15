#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include <vector>

struct CTabBar : public Component {
private:
	struct Tab {
		std::string title;
		std::string desc;
		bool change;
	};

	const Sint32 m_tabWidthAddition = 48;

	// Selected, Hovered, Previous hovered, Close hovered
	Sint16 m_selected, m_hovered, m_pHovered, m_cHovered;
	Sint16 m_closedItem;
	Sint32 m_visibleItems;
	std::vector<Tab> m_tabList;

	Texture* m_texClose;

	bool m_elVisible;
	Texture* m_elTexArrow;
	glm::vec2 m_elPos;
	glm::vec2 m_elSize;

	void refreshSize();
public:
	CTabBar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme);

	Uint16 getTabCount();
	void setSelected(Uint16 p_selected);
	void setSelectedState(Sint32 p_index, bool p_change) { if (p_index >= 0 && p_index < (Sint32)m_tabList.size()) m_tabList[p_index].change = p_change; }
	Sint16 getSelectedItem();
	Sint16 getClosedItem();

	Component* addItem(std::string p_name, std::string p_desc);
	void renameItem(Sint32 p_index, std::string p_name, std::string p_desc);
	void setSelectedItem(Uint16 p_selectedItem) { m_selected = p_selectedItem; }
	Uint16 getItemCount() { return Uint16(m_tabList.size()); }
	Tab getItem(Sint32 p_index) { return m_tabList[p_index]; }
	void removeTab(Sint32 p_index);
	void clear();
	void setTab(Sint16 p_index, std::string p_title, std::string p_desc) { m_tabList[p_index] = { p_title, p_desc }; };

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
};
