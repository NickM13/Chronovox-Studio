#include "engine\gfx\gui\field\Text.h"


CText::CText(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Alignment p_align, Color p_fontColor)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_size = p_size;
	m_align = p_align;
	m_color = p_fontColor;
}

void CText::render()
{
	m_color.useColor();
	Font::getInstance().setAlignment(m_align);
	switch(m_align)
	{
	case ALIGN_LEFT:
		Font::getInstance().print(m_title, m_pos.x, m_pos.y);
		break;
	case ALIGN_CENTER:
		Font::getInstance().print(m_title, m_pos.x + m_size.x / 2, m_pos.y);
		break;
	case ALIGN_RIGHT:
		Font::getInstance().print(m_title, m_pos.x + m_size.x, m_pos.y);
		break;
	}
}
