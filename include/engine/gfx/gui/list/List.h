#pragma once

#include "engine\utils\Globals.h"
#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CList : public Component
{
private:
	Sint16 m_selectedItem;
	Uint16 m_itemHeight;

	Sint16 m_scroll, m_maxScroll;

	Vector2<Uint16> m_mouseBuffer;

	bool m_hover, m_dragging;
	Uint8 m_update;

	Texture m_baseTileSheet;

	struct ListItem
	{
		std::string m_name;
		Texture m_texture;
		Uint16 m_texId;

		ListItem(std::string p_name, Texture p_tex, Uint16 p_texId)
		{
			m_name = p_name;
			Texture m_texture;
			m_texId = p_texId;
		}
	};
	std::vector<ListItem> m_itemList;
public:
	CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight, Texture p_baseTileSheet, Sint8 p_colorTheme = 0);

	Component* addItem(std::string p_item, Uint16 p_texId, Texture p_texture = Texture());
	void removeItem(Uint16 p_index);
	void setItem(Uint16 p_index, std::string p_item);
	void setItemTexId(Uint16 p_index, Uint16 p_texId);
	void setItemTexture(Uint16 p_index, Texture p_texture);
	Uint16 getItemCount();
	std::string getItem(Uint16 p_index);
	Texture getItemTexture(Uint16 p_index);
	Uint16 getItemTexId(Uint16 p_index);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();

	Sint16 getSelectedItem();
	Uint8 isUpdated();

	Sint8 isSelected();

	void clear();
};
