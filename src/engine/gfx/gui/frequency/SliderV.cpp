#include "engine\gfx\gui\frequency\SliderV.h"

CSliderV::CSliderV(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_length, Sint32 p_maxValue)
	: Component(p_compName, p_title, p_pos, {}, Theme::ACTION) {
	m_length = p_length;
	m_maxValue = p_maxValue;
	m_numValue = 0;
	m_height = 4;
	m_width = 8;
	m_size = {m_width, m_length};
};

void CSliderV::setMaxValue(Sint16 p_value) {
	m_maxValue = p_value;
	if(m_numValue > m_maxValue)
		m_numValue = m_maxValue;
}

void CSliderV::setValue(Sint32 p_value) {
	if(m_numValue != p_value) {
		if(p_value < 0)
			m_numValue = 0;
		else if(p_value > m_maxValue)
			m_numValue = m_maxValue;
		else
			m_numValue = p_value;
		callPressFunction();
	}
}

void CSliderV::addValue(Sint16 p_value) {
	m_numValue += p_value;
}

void CSliderV::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	if((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) || m_held) {
		if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) && 
			_mousePos.x >= m_pos.x - m_width / 2 && _mousePos.x < m_pos.x + m_width / 2 &&
			_mousePos.y >= m_pos.y - m_height && _mousePos.y < m_pos.y + m_length + m_height)
			m_held = true;

		if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) && m_held) {
			m_slideValue = m_pos.y - _mousePos.y + m_length;
			setValue(Sint32(round((m_slideValue / GLfloat(m_length)) * (m_maxValue))));
		}
		if(GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT))
			m_held = false;

		if((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && (m_held || (_mousePos.x >= m_pos.x - m_width / 2 && _mousePos.x < m_pos.x + m_width / 2 &&
			_mousePos.y >= m_pos.y - m_height && _mousePos.y < m_pos.y + m_length + m_height))) {
			addTooltip();
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
	}
}
void CSliderV::update(Vector2<Sint32> p_pos) {

}
void CSliderV::render() {
	m_slideValue = Sint32((m_numValue / GLfloat(m_maxValue)) * m_length);
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		glBegin(GL_QUADS);
		{
			//Outline
			m_colorTheme.m_border.useColor(0.5f, 0.5f, 0.5f);
			glVertex2f(-m_width / 2.f - 1, -m_height - 1);
			glVertex2f(m_width / 2.f + 1, -m_height - 1);
			glVertex2f(m_width / 2.f + 1, m_length + m_height + 1);
			glVertex2f(-m_width / 2.f - 1, m_length + m_height + 1);

			//Background
			m_colorTheme.m_primary.useColor();
			glVertex2f(-m_width / 2.f, -m_height);
			glVertex2f(m_width / 2.f, -m_height);
			glVertex2f(m_width / 2.f, m_length + m_height);
			glVertex2f(-m_width / 2.f, m_length + m_height);

			m_colorTheme.m_border.useColor();
			glVertex2f(-m_width / 2.f, -m_height);
			glVertex2f(m_width / 2.f, -m_height);
			glVertex2f(m_width / 2.f, GLfloat(m_length - m_slideValue));
			glVertex2f(-m_width / 2.f, GLfloat(m_length - m_slideValue));

			//Slider
			m_colorTheme.m_hover.useColor();
			glVertex2f(-GLfloat(m_width / 2), GLfloat((m_length - m_slideValue) - m_height));
			glVertex2f(GLfloat(m_width / 2), GLfloat((m_length - m_slideValue) - m_height));
			glVertex2f(GLfloat(m_width / 2), GLfloat((m_length - m_slideValue) + m_height));
			glVertex2f(-GLfloat(m_width / 2), GLfloat((m_length - m_slideValue) + m_height));
		}
		glEnd();
	}
	glPopMatrix();
}

Sint32 CSliderV::getMaxValue()
{
	return m_maxValue;
}
Sint32 CSliderV::getLength()
{
	return m_length;
}