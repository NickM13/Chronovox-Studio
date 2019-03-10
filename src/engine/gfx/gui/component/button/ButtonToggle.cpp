#include "engine\gfx\gui\component\button\ButtonToggle.h"

CButtonToggle::CButtonToggle(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
	: Component(p_compName, p_title, p_pos, p_size) {
	m_selected = p_state;
	m_texType = 0;
	m_pressFunction = p_func;

	m_buttonTex[0] = 0;
	m_buttonTex[1] = 0;
}
CButtonToggle::CButtonToggle(std::string p_compName, Texture* p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
	: Component(p_compName, "", p_pos, p_size) {
	m_selected = p_state;
	m_pressFunction = p_func;

	m_buttonTex[0] = p_buttonTex;
	m_buttonTex[1] = 0;
	m_texType = 1;
}
CButtonToggle::CButtonToggle(std::string p_compName, Texture* p_activeTex, Texture* p_inactiveTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
	: Component(p_compName, "", p_pos, p_size) {
	m_selected = p_state;
	m_pressFunction = p_func;

	m_buttonTex[0] = p_activeTex;
	m_buttonTex[1] = p_inactiveTex;
	m_texType = 2;
}

void CButtonToggle::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
	m_numValue = 0;
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x >= 0 && _mousePos.x <= m_size.x &&
		_mousePos.y >= 0 && _mousePos.y <= m_size.y) {
		addTooltip();
		setHovered(true);
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			m_numValue = 1;
			m_selected = !m_selected;
			if (m_pressFunction != 0) {
				m_pressFunction();
			}
		}
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
	else {
		resetTooltip();
		setHovered(false);
	}
}
void CButtonToggle::update(GLfloat p_deltaUpdate) {
	Component::update(p_deltaUpdate);
}
void CButtonToggle::render() {
	Shader::pushMatrixModel();
	Component::renderBack();
	Component::renderFill();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	Shader::translate(glm::vec3((GLfloat)(m_size.x / 2), (GLfloat)(m_size.y / 2), 0.f));

	if (m_texType != 0) {
		GBuffer::setColor(Color(1.f, 1.f, 1.f));
		if (m_selected != 0 || m_texType == 1) {
			GBuffer::setTexture(m_buttonTex[0]->getGlId());
		}
		else {
			GBuffer::setTexture(m_buttonTex[1]->getGlId());
		}
		GBuffer::setUV(0.f, 1.f);
		if (m_selected != 0 || m_texType == 1)	GBuffer::addVertexQuad(-GLfloat(m_buttonTex[0]->getSize().x) / 2, -GLfloat(m_buttonTex[0]->getSize().y) / 2);
		else									GBuffer::addVertexQuad(-GLfloat(m_buttonTex[1]->getSize().x) / 2, -GLfloat(m_buttonTex[1]->getSize().y) / 2);
		GBuffer::setUV(1, 1);
		if (m_selected != 0 || m_texType == 1)	GBuffer::addVertexQuad(GLfloat(m_buttonTex[0]->getSize().x) / 2, -GLfloat(m_buttonTex[0]->getSize().y) / 2);
		else									GBuffer::addVertexQuad(GLfloat(m_buttonTex[1]->getSize().x) / 2, -GLfloat(m_buttonTex[1]->getSize().y) / 2);
		GBuffer::setUV(1, 0);
		if (m_selected != 0 || m_texType == 1)	GBuffer::addVertexQuad(GLfloat(m_buttonTex[0]->getSize().x) / 2, GLfloat(m_buttonTex[0]->getSize().y) / 2);
		else									GBuffer::addVertexQuad(GLfloat(m_buttonTex[1]->getSize().x) / 2, GLfloat(m_buttonTex[1]->getSize().y) / 2);
		GBuffer::setUV(0, 0);
		if (m_selected != 0 || m_texType == 1)	GBuffer::addVertexQuad(-GLfloat(m_buttonTex[0]->getSize().x) / 2, GLfloat(m_buttonTex[0]->getSize().y) / 2);
		else									GBuffer::addVertexQuad(-GLfloat(m_buttonTex[1]->getSize().x) / 2, GLfloat(m_buttonTex[1]->getSize().y) / 2);
		GBuffer::setTexture(0);
	}
	GBuffer::setColor(m_colorThemeMap.at("textLight"));
	Font::setAlignment(ALIGN_CENTER);
	Font::print(m_title, 0, 0);
	Shader::popMatrixModel();
}
