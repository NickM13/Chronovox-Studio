#include "engine\gfx\gui\component\icon\Icon.h"

CIcon::CIcon(std::string p_compName, Texture* p_icon, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Component(p_compName, "", p_pos, p_size) {
	m_texture = p_icon;
}

void CIcon::render() {
	GBuffer::renderTexture(m_texture, m_pos, m_size, GBuffer::TextureStyle::STRETCH);
}
