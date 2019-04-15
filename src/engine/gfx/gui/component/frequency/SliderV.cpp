#include "engine\gfx\gui\component\frequency\SliderV.h"

Sint32 CSliderV::m_height = 0;
Sint32 CSliderV::m_width = 12;

CSliderV::CSliderV(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_length, Sint32 p_maxValue)
	: Component(p_compName, p_title, p_pos + Vector2<Sint32>(0, m_height), {}) {
	m_maxValue = p_maxValue;
	m_numValue = 0;
	m_length = p_length - m_height * 2;
	m_size = { m_width, m_length };
};

void CSliderV::setMaxValue(Sint16 p_value) {
	m_maxValue = p_value;
	if (m_numValue > m_maxValue) {
		m_numValue = m_maxValue;
	}
}

void CSliderV::setValue(Sint32 p_value) {
	if (m_numValue != p_value) {
		if (p_value < 0) {
			m_numValue = 0;
		}
		else if (p_value > m_maxValue) {
			m_numValue = m_maxValue;
		}
		else {
			m_numValue = p_value;
		}
		callPressFunction();
	}
}

void CSliderV::addValue(Sint16 p_value) {
	m_numValue += p_value;
	if (m_numValue > m_maxValue) {
		m_numValue = m_maxValue;
	}
	if (m_numValue < 0) {
		m_numValue = 0;
	}
}

void CSliderV::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) || m_held) {
		m_hovered = (_mousePos.x >= m_pos.x - m_width / 2 && _mousePos.x < m_pos.x + m_width / 2 &&
			_mousePos.y >= m_pos.y - m_height && _mousePos.y < m_pos.y + m_length + m_height);
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) && m_hovered) {
			m_held = true;
		}

		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) && m_held) {
			m_slideValue = m_pos.y - _mousePos.y + m_length;
			setValue(Sint32(round((m_slideValue / GLfloat(m_length)) * (m_maxValue))));
		}
		if (GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
			m_held = false;
		}

		if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && m_hovered) {
			addTooltip();
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
		else {
			resetTooltip();
		}
		if ((p_interactFlags & (Sint8)EventFlag::MOUSESCROLL) && m_hovered) {
			addValue(GMouse::getMouseScroll());
			p_interactFlags -= (Sint8)EventFlag::MOUSESCROLL;
		}
	}
	else {
		resetTooltip();
	}
}
void CSliderV::update(Vector2<Sint32> p_pos) {

}
void CSliderV::render() {
	m_slideValue = Sint32((m_numValue / GLfloat(m_maxValue)) * m_length);
	Shader::pushMatrixModel();

	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));

	//Outline
	GBuffer::setColor(m_colorThemeMap.at("borderElementUnfocused"));
	GBuffer::addQuadFilled(Vector2<Sint32>(-m_width / 2.f - 1, -m_height - 1), Vector2<Sint32>(m_width + 2, m_length + m_height * 2 + 2));

	//Background
	GBuffer::setColor(getPrimaryColor());
	GBuffer::addQuadFilled(Vector2<Sint32>(-m_width / 2.f, -m_height), Vector2<Sint32>(m_width, m_length + m_height * 2));

	GBuffer::setColor(m_colorThemeMap.at("actionHighlight"));
	GBuffer::addQuadFilled(Vector2<Sint32>(-m_width / 2.f, m_length - m_slideValue + m_height), Vector2<Sint32>(m_width, m_slideValue));

	//Slider
	GBuffer::setColor(m_colorThemeMap.at("actionHovered"));
	GBuffer::addQuadFilled(Vector2<Sint32>(-m_width / 2.f, (m_length - m_slideValue) - m_height), Vector2<Sint32>(m_width, m_height * 2));

	Shader::popMatrixModel();
}

Sint32 CSliderV::getMaxValue()
{
	return m_maxValue;
}
Sint32 CSliderV::getLength()
{
	return m_length;
}