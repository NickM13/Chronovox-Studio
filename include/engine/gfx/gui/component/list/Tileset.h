#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CTileSet : public Component
{
private:
	Vector2<Sint16> m_selectedTile;
	Uint16 m_tileSize;

	Texture* m_tileSheet;
	Texture* m_transparentTex, *m_selectTex;
	Vector2<Uint16> m_tileCount;
	Vector2<Sint16> m_scroll;

	Vector2<Uint16> m_mouseBuffer;

	bool m_hover;
	bool m_dragging;
public:
	CTileSet(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_tileSize, Texture* p_tileSheet, Theme p_colorTheme = Theme::PRIMARY);
	void setTileSheet(Texture* p_tileSheet);
	Vector2<Sint32> getRealPosition();
	Vector2<Sint32> getRealSize();

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();

	void setSelectedTile(Uint16 p_index);
	Uint16 getSelectedTile();
	Sint8 isSelected();
};
