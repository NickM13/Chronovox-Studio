#pragma once

#include "..\Component.h"
#include <vector>

class CDropDown : public Component
{
private:
	Sint16 m_selectedItem, m_prevSelectedItem;
	Sint32 m_hoverItem;

	bool m_update;

	std::vector<std::string> m_itemList;

	Texture* m_arrow;
public:
	CDropDown(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_width, Theme p_colorTheme = Theme::PRIMARY);

	void setList(std::vector<std::string> p_items);
	std::string getItem(Uint16 p_index);
	Component* addItem(std::string p_item);
	
	void setSelectedItem(Uint16 p_selectedItem);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();

	Uint8 isUpdated();
	Sint16 getSelectedItem();
	Sint16 getPrevSelectedItem();

	Vector2<Sint32> getRealPosition();
	Vector2<Sint32> getRealSize();

	bool hasList()
	{
		return !m_itemList.empty();
	}
};
