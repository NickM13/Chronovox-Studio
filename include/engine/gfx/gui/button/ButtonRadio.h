#pragma once

#include "ButtonToggle.h"
#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include "engine\sfx\Sound.h"

class CButtonRadio : public Component
{
public:
	CButtonRadio(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_buttonSize, Vector2<Sint32> p_buttonStep);
	~CButtonRadio();

	Component* addButton(Texture* p_tex);
	void setSize(Vector2<Sint32> p_size) {}

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();

	void setSelectedItem(Uint16 p_selectedButton);

	Sint16 getSelectedItem()	 { return m_selectedButton; }
	Sint16 getPrevSelectedItem() { return m_prevSelectedButton; }
	Sint32 getButtonCount()		 { return m_buttonList.size(); }
private:
	Vector2<Sint32> m_buttonSize, m_buttonStep;
	Uint16 m_selectedButton, m_prevSelectedButton;

	std::vector<CButtonToggle*> m_buttonList;

	Source m_soundClick;
	Source m_soundHover;
};
