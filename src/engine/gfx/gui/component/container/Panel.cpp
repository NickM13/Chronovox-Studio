#include "engine\gfx\gui\component\container\Panel.h"

Panel::Panel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_borderFlags)
	: Component(p_compName, p_title, p_pos, p_size) {
	m_border = p_borderFlags;
	m_visible = true;
}

void Panel::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
		if (m_title != "") {
			if (_mousePos.x - m_pos.x >= 0 && _mousePos.x - m_pos.x < m_size.x &&
				_mousePos.y - m_pos.y >= -24 && _mousePos.y - m_pos.y < 0) {
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
				m_selected = 1;
				callPressFunction();
			}
		}
		if (_mousePos.x - m_pos.x >= 0 && _mousePos.x - m_pos.x < m_size.x &&
			_mousePos.y - m_pos.y >= 0 && _mousePos.y - m_pos.y < m_size.y) {
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				m_selected = 1;
				callPressFunction();
			}
		}
	}
	if (!(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) && m_selected == 1) {
		m_selected = 0;
		callReleaseFunction();
	}
}
void Panel::update(GLfloat p_updateTime) {

}
void Panel::render() {
	if (m_visible) {
		Shader::pushMatrixModel();
		if (m_title != "") {
			Sint8 _ignore = m_selected;
			m_selected = 0;
			Component::render();
			m_selected = _ignore;
			Shader::translate(glm::vec3(m_pos.x, m_pos.y - 24, 0));
			GBuffer::renderShadow(Vector2<Sint32>(0, 0), Vector2<Sint32>(m_size.x, m_size.y + 24));
			GBuffer::setTexture(0);
		}
		else {
			Sint8 _ignore = m_selected;
			m_selected = 0;
			Component::render();
			m_selected = _ignore;
			Shader::translate(glm::vec3(m_pos.x, m_pos.y, 0));
			GBuffer::renderShadow(Vector2<Sint32>(0, 0), Vector2<Sint32>(m_size.x, m_size.y));
			GBuffer::setTexture(0);
		}
		if (m_title != "") {
			GBuffer::setColor(getPrimaryColor());
			GBuffer::addQuadFilled(Vector2<Sint32>(0, 0), Vector2<Sint32>(m_size.x, 25));
			GBuffer::setColor(m_colorThemeMap.at("borderElementUnfocused"));
			GBuffer::addQuadOutlined(Vector2<Sint32>(0, 0), Vector2<Sint32>(m_size.x, 25));

			GBuffer::setColor(m_colorThemeMap.at("textLight"));
			Font::setAlignment(ALIGN_CENTER);
			Font::print(m_title, m_size.x / 2, 12);
			GBuffer::setTexture(0);
		}
		Shader::popMatrixModel();
	}
}
