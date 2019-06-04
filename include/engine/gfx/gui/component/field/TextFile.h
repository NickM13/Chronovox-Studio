#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CTextFile : public Component
{
private:
	std::vector<std::string> m_lines;
	std::vector<Vector2<Sint32>> m_variableInputLoc;
	Sint32 m_variableInputIndex;
	Alignment m_align;
	std::string m_color;
public:
	CTextFile(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Alignment p_align, std::string p_fontColor);

	void updateTitle();

	void addInputString(std::string p_str);

	std::string getTitle();

	void render();
};
