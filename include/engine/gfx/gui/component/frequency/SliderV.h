#pragma once

#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

struct CSliderV : public Component {
private:
	static Sint32 m_height;
	static Sint32 m_width;

	Sint32 m_length;
	Sint32 m_maxValue;
	Sint32 m_slideValue;

	bool m_held = false;
	Vector2<Sint32> m_holdPos;
public:
	CSliderV(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_length, Sint32 p_maxValue);

	void setMaxValue(Sint16 p_value);
	void setValue(Sint32 p_value);
	void addValue(Sint16 p_value);

	void input(Sint8& p_interactFlags);
	void update(Vector2<Sint32> p_pos);
	void render();

	Sint32 getMaxValue();
	Sint32 getLength();
};

