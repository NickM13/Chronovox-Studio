#pragma once

#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

struct CSliderV : public Component
{
private:
	Sint32 m_length;
	Sint32 m_maxValue;
	Sint32 m_slideValue;
	Sint32 m_height;
	Sint8 m_width;

	bool m_held = false;
	Vector2<Sint32> m_holdPos;
public:
	CSliderV(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_length, Sint32 p_maxValue, Sint32& p_numValue);

	void setMaxValue(Sint16 p_value);
	void setValue(Sint32 p_value);
	void addValue(Sint16 p_value);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(Vector2<Sint32> p_pos);
	void render();

	Sint32 getMaxValue();
	Sint32 getLength();
};

