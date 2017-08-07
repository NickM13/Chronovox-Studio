#pragma once

#include "ButtonToggle.h"
#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"
#include "engine\sfx\Sound.h"

class CButtonRadio : public Component
{
public:
	CButtonRadio(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16* p_selectedButton);

	Component* addButton(CButtonToggle* p_button);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();

	void setSelectedButton(Uint16 p_selectedButton);

	Sint16 getSelectedItem()
	{ return *m_selectedButton; }
	Sint16 getPrevSelectedItem()
	{ return m_prevSelectedButton; }
private:
	Uint16* m_selectedButton, m_prevSelectedButton;

	std::vector<CButtonToggle*> m_buttonList;
};
