#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CText : public Component
{
private:
	Alignment m_align;
	std::string m_color;
	std::string m_font;
public:
	CText(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Alignment p_align, std::string p_fontColor, std::string p_font = "Body");

	void render();
};
