#include "engine\gfx\gui\component\field\Text.h"

CText::CText(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Alignment p_align, Color p_fontColor)
	: Component(p_compName, p_title, p_pos, p_size, Theme::PRIMARY) {
	m_align = p_align;
	m_color = p_fontColor;
}

void CText::render() {
	GBuffer::setColor(m_color);
	Font::setAlignment(m_align);
	switch (m_align) {
	case ALIGN_LEFT:
		Font::print(m_title, m_pos.x, m_pos.y);
		break;
	case ALIGN_CENTER:
		Font::print(m_title, m_pos.x + m_size.x / 2, m_pos.y);
		break;
	case ALIGN_RIGHT:
		Font::print(m_title, m_pos.x + m_size.x, m_pos.y);
		break;
	}
}
