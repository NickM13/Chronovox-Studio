#include "engine\gfx\gui\component\container\ColorPanel.h"

ColorPanel::ColorPanel(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32& p_r, Sint32& p_g, Sint32& p_b, Texture p_texture)
	: Component("", "", p_pos, p_size, Theme::PRIMARY) {
	r = &p_r;
	g = &p_g;
	b = &p_b;
	m_texture = p_texture;
}

void ColorPanel::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && _mousePos.x >= 0 && _mousePos.y >= 0 && _mousePos.x < m_size.x && _mousePos.y < m_size.y) {
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
}
void ColorPanel::render() {
	Shader::pushMatrixModel();
	GBuffer::setTexture(m_texture.getGlId());
	GBuffer::setColor(Color(*r / 255.f, *g / 255.f, *b / 255.f));
	Shader::translate(glm::vec3(GLfloat(m_pos.x), GLfloat(m_pos.y), 0.f));
	GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, 0);
	GBuffer::setUV(1, 1); GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
	GBuffer::setUV(1, 0); GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
	GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, GLfloat(m_size.y));
	Shader::popMatrixModel();
}


SColorPanel::SColorPanel(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Color color, Texture p_texture) {
	m_compName = p_compName;
	m_pos = p_pos;
	m_size = p_size;
	m_color = color;
	m_texture = p_texture;
}
SColorPanel::SColorPanel(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Color color, Texture p_texture) {
	m_pos = p_pos;
	m_size = p_size;
	m_color = color;
	m_texture = p_texture;
}

void SColorPanel::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && _mousePos.x >= 0 && _mousePos.y >= 0 && _mousePos.x < m_size.x && _mousePos.y < m_size.y) {
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			callPressFunction();
		}
	}
}
void SColorPanel::render() {
	Shader::pushMatrixModel();
	GBuffer::setTexture(m_texture.getGlId());
	GBuffer::setColor(m_color);
	Shader::translate(glm::vec3(GLfloat(m_pos.x), GLfloat(m_pos.y), 0));
	GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, 0);
	GBuffer::setUV(1, 1); GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
	GBuffer::setUV(1, 0); GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
	GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, GLfloat(m_size.y));
	Shader::popMatrixModel();
}

Color SColorPanel::getColor() {
	return m_color;
}
