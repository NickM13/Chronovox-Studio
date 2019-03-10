#include "engine\gfx\gui\component\field\DataField.h"

void DataField::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && _mousePos.x >= 0 && _mousePos.x < m_size.x
		&& _mousePos.y >= 0 && _mousePos.y < m_size.y) {
		addTooltip();
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			m_selected = 1;
		}
		GGui::setCursorType(GGui::CursorType::IBEAM);
	}
	else if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		m_selected = 0;
	}

	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x >= 0 && _mousePos.x < m_size.x
		&& _mousePos.y >= 0 && _mousePos.y < m_size.y) {
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
}

void DataField::update(GLfloat p_deltaUpdate) {

}

void DataField::render() {
	Shader::pushMatrixModel();
	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	GBuffer::setColor(m_colorThemeMap.at("textLight"));
	Font::setAlignment(ALIGN_LEFT);
	Font::print(*m_dataString, 0, Sint32(Font::getHeight()));
	Shader::popMatrixModel();
}
