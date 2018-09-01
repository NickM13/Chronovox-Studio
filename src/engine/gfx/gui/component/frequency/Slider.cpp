#include "engine\gfx\gui\component\frequency\Slider.h"


CSlider::CSlider(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_length, Sint32 p_maxValue, Sint32& p_numValue)
	: Component(p_compName, p_title, p_pos, {}, Theme::ACTION) {
	m_length = p_length;
	m_maxValue = p_maxValue;
	m_numValue = p_numValue;
	m_height = 16;
	m_width = 4;
	m_size = { p_length, m_height };
};

void CSlider::setMaxValue(Sint16 p_value) {
	m_maxValue = p_value;
	if (m_numValue > m_maxValue) {
		setValue(m_maxValue);
	}
}

void CSlider::setValue(Sint32 p_value) {
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

void CSlider::addValue(Sint16 p_value)
{
	m_numValue += p_value;
}

void CSlider::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) || m_held) {
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			if (_mousePos.x >= m_pos.x - m_width && _mousePos.x < m_pos.x + m_length + m_width &&
				_mousePos.y >= m_pos.y - m_height / 2 && _mousePos.y < m_pos.y + m_height / 2) {
				m_held = true;
			}
		}
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			if (m_held) {
				m_slideValue = _mousePos.x - m_pos.x;
				setValue(Sint32(round((m_slideValue / GLfloat(m_length)) * (m_maxValue))));
			}
		}
		if (GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
			m_held = false;
		}
		if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && (m_held || (_mousePos.x >= m_pos.x - m_width && _mousePos.x < m_pos.x + m_length + m_width &&
			_mousePos.y >= m_pos.y - m_height / 2 && _mousePos.y < m_pos.y + m_height / 2))) {
			addTooltip();
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
	}
}
void CSlider::update(Vector2<Sint32> p_pos) {

}
void CSlider::render() {
	m_slideValue = Sint32((m_numValue / GLfloat(m_maxValue)) * m_length);
	Shader::pushMatrixModel();

	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));

	//Outline
	GBuffer::setColor(m_colorTheme.m_border.applyScale(Color(0.5f, 0.5f, 0.5f)));
	GBuffer::addVertexQuad(-m_width - 1, -GLfloat(m_height / 2) - 1);
	GBuffer::addVertexQuad(GLfloat(m_length + m_width + 1), -GLfloat(m_height / 2) - 1);
	GBuffer::addVertexQuad(GLfloat(m_length + m_width + 1), GLfloat(m_height / 2) + 1);
	GBuffer::addVertexQuad(-m_width - 1, GLfloat(m_height / 2) + 1);

	//Background
	GBuffer::setColor(m_colorTheme.m_border);
	GBuffer::addVertexQuad(-m_width, -GLfloat(m_height / 2));
	GBuffer::addVertexQuad(GLfloat(m_length + m_width), -GLfloat(m_height / 2));
	GBuffer::addVertexQuad(GLfloat(m_length + m_width), GLfloat(m_height / 2));
	GBuffer::addVertexQuad(-m_width, GLfloat(m_height / 2));

	GBuffer::setColor(m_colorTheme.m_primary);
	GBuffer::addVertexQuad(-m_width, -GLfloat(m_height / 2));
	GBuffer::addVertexQuad(GLfloat(m_slideValue), -GLfloat(m_height / 2));
	GBuffer::addVertexQuad(GLfloat(m_slideValue), GLfloat(m_height / 2));
	GBuffer::addVertexQuad(-m_width, GLfloat(m_height / 2));

	//Slider
	GBuffer::setColor(m_colorTheme.m_select);
	GBuffer::addVertexQuad(GLfloat(m_slideValue - m_width), -GLfloat(m_height / 2));
	GBuffer::addVertexQuad(GLfloat(m_slideValue + m_width), -GLfloat(m_height / 2));
	GBuffer::addVertexQuad(GLfloat(m_slideValue + m_width), GLfloat(m_height / 2));
	GBuffer::addVertexQuad(GLfloat(m_slideValue - m_width), GLfloat(m_height / 2));
	Shader::popMatrixModel();

	GBuffer::setColor(m_colorTheme.m_textLight);
	Font::setAlignment(ALIGN_CENTER);
	Font::print(m_title, Sint32(m_pos.x + m_length / 2), Sint32(m_pos.y - m_height));

	GBuffer::setColor(m_colorTheme.m_text);
	Font::setAlignment(ALIGN_CENTER);
	Font::print(Util::numToStringInt(m_numValue), Sint32(m_pos.x + m_length / 2), Sint32(m_pos.y));
}

Sint32 CSlider::getMaxValue() {
	return m_maxValue;
}
Sint32 CSlider::getLength() {
	return m_length;
}