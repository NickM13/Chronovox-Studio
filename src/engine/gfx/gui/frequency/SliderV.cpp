#include "engine\gfx\gui\frequency\SliderV.h"


CSliderV::CSliderV(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_length, Sint32 p_maxValue, Sint32& p_numValue)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_length = p_length;
	m_maxValue = p_maxValue;
	m_numValue = &p_numValue;
	m_colorTheme = m_colorThemes[ACTION];

	m_height = 4;
	m_width = 12;
	m_size = {m_width, m_length};
};

void CSliderV::setMaxValue(Sint16 p_value)
{
	m_maxValue = p_value;
	if(*m_numValue > m_maxValue)
		*m_numValue = m_maxValue;
}

void CSliderV::setValue(Sint32 p_value)
{
	if(*m_numValue != p_value)
	{
		if(p_value < 0)
			*m_numValue = 0;
		else if(p_value > m_maxValue)
			*m_numValue = m_maxValue;
		else
			*m_numValue = p_value;
		callPressFunction();
	}
}

void CSliderV::addValue(Sint16 p_value)
{
	*m_numValue += p_value;
}

void CSliderV::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	if((p_interactFlags & EVENT_MOUSEOVER) || m_held)
	{
		if((p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS) && 
			p_mousePos.x >= m_pos.x - m_width / 2 && p_mousePos.x < m_pos.x + m_width / 2 &&
			p_mousePos.y >= m_pos.y - m_height && p_mousePos.y < m_pos.y + m_length + m_height)
			m_held = true;

		if((p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_DOWN) && m_held)
		{
			m_slideValue = m_pos.y - p_mousePos.y + m_length;
			setValue(Sint32(round((m_slideValue / GLfloat(m_length)) * (m_maxValue))));
		}
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_RELEASE)
			m_held = false;

		if((p_interactFlags & EVENT_MOUSEOVER) && (m_held || (p_mousePos.x >= m_pos.x - m_width / 2 && p_mousePos.x < m_pos.x + m_width / 2 &&
			p_mousePos.y >= m_pos.y - m_height && p_mousePos.y < m_pos.y + m_length + m_height)))
		{
			addTooltip();
			p_interactFlags -= EVENT_MOUSEOVER;
		}
	}
}
void CSliderV::update(Vector2<Sint32> p_pos)
{

}
void CSliderV::render()
{
	m_slideValue = Sint32((*m_numValue / GLfloat(m_maxValue)) * m_length);
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