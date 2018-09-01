#pragma once

#include "engine\utils\Utilities.h"
#include "..\button\Button.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include <vector>

struct CTabBar : public Component
{
public:
	CTabBar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme);

	Uint16 getTabCount();
	void setSelected(Uint16 p_selected);
	Sint16 getSelectedItem();
	Uint8 isUpdated() {return m_updated;}

	Component* addItem(std::string p_title);
	Uint16 getItemCount() {return Uint16(m_tabList.size()); }
	std::string getItem(Uint16 p_index) {return m_tabList[p_index]; }
	void removeTab(Sint16 p_index);
	void clear();
	void setTab(Sint16 p_index, std::string p_title) {m_tabList[p_index] = p_title;};

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
private:
	void calcMaxScroll();

	Sint16 m_selected, m_hovered;
	Sint32 m_scroll, m_maxScroll;
	std::vector<std::string> m_tabList;

	CButton* m_buttonLShift, *m_buttonRShift;

	Sint8 m_updated;
};
