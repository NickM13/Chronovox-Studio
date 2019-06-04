#include "engine\gfx\gui\component\field\Text.h"

CText::CText(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Alignment p_align, std::string p_fontColor, std::string p_font)
	: Component(p_compName, p_title, p_pos, p_size) {
	m_align = p_align;
	m_color = p_fontColor;
	m_font = p_font;
	m_size = Font::getMessageWidth(p_title);
}

void CText::render() {
	GBuffer::setColor(getElementColor(m_color));
	Font::setFont(m_font);
	Font::setAlignment(m_align);
	switch (m_align) {
	case ALIGN_LEFT:
		Font::print(m_title, m_pos.x, m_pos.y + Font::getSpacingHeight() / 2);
		break;
	case ALIGN_CENTER:
		Font::print(m_title, m_pos.x + m_size.x / 2, m_pos.y + Font::getSpacingHeight() / 2);
		break;
	case ALIGN_RIGHT:
		Font::print(m_title, m_pos.x + m_size.x, m_pos.y + Font::getSpacingHeight() / 2);
		break;
	}
	Font::setFont("Body");
}
