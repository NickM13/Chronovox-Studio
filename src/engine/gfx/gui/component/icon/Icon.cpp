#include "engine\gfx\gui\component\icon\Icon.h"

CIcon::CIcon(std::string p_compName, Texture* p_icon, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Component(p_compName, "", p_pos, p_size, Theme::PRIMARY) {
	m_texture = p_icon;
}

void CIcon::render() {
	Shader::pushMatrixModel();

	GBuffer::setTexture(m_texture->getGlId());
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	GBuffer::setColor(Color(1.f, 1.f, 1.f));

	GBuffer::setUV(0.f, 1.f);	GBuffer::addVertexQuad(0, 0);
	GBuffer::setUV(1.f, 1.f);	GBuffer::addVertexQuad(m_size.x, 0);
	GBuffer::setUV(1.f, 0.f);	GBuffer::addVertexQuad(m_size.x, m_size.y);
	GBuffer::setUV(0.f, 0.f);	GBuffer::addVertexQuad(0, m_size.y);

	Shader::popMatrixModel();
}
