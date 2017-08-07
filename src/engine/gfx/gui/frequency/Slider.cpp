#include "engine\gfx\gui\frequency\Slider.h"


CSlider::CSlider(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_length, Sint32 p_maxValue, Sint32& p_numValue)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_length = p_length;
	m_maxValue = p_maxValue;
	m_numValue = &p_numValue;
	m_size = {p_length, m_height};
	m_colorTheme = m_colorThemes[ACTION];

	m_height = 16;
	m_width = 4;
};

void CSlider::setMaxValue(Sint16 p_value)
{
	m_maxValue = p_value;
	if(*m_numValue > m_maxValue)
		*m_numValue = m_maxValue;
}

void CSlider::setValue(Sint32 p_value)
{
	if(*m_numValue != p_value)
	{
		if(p_value < 0)
			*m_numValue = 0;
		else if(p_value > m_maxValue)
			*m_numValue = m_maxValue;
		else
			*m_numValue = p_value;
		callFunction();
	}
}

void CSlider::addValue(Sint16 p_value)
{
	*m_numValue += p_value;
}

void CSlider::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	if((p_interactFlags & EVENT_MOUSEOVER) || m_held)
	{
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
		{
			if(p_mousePos.x >= m_pos.x - m_width && p_mousePos.x < m_pos.x + m_length + m_width &&
				p_mousePos.y >= m_pos.y - m_height / 2 && p_mousePos.y < m_pos.y + m_height / 2)
			{
				m_held = true;
			}
		}
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN)
		{
			if(m_held)
			{
				m_slideValue = p_mousePos.x - m_pos.x;
				setValue(Sint32(round((m_slideValue / GLfloat(m_length)) * (m_maxValue))));
			}
		}
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_RELEASE)
		{
			m_held = false;
		}
		if((p_interactFlags & EVENT_MOUSEOVER) && (m_held || (p_mousePos.x >= m_pos.x - m_width && p_mousePos.x < m_pos.x + m_length + m_width &&
													   p_mousePos.y >= m_pos.y - m_height / 2 && p_mousePos.y < m_pos.y + m_height / 2)))
		{
			addTooltip();
			p_interactFlags -= EVENT_MOUSEOVER;
		}
	}
}
void CSlider::update(Vector2<Sint32> p_pos)
{

}
void CSlider::render()
{
	m_slideValue = Sint32((*m_numValue / GLfloat(m_maxValue)) * m_length);
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		glBegin(GL_QUADS);
		{
			//Outline
			m_colorTheme.m_back.useColor(0.5f, 0.5f, 0.5f);
			glVertex2f(-m_width - 1, -GLfloat(m_height / 2) - 1);
			glVertex2f(GLfloat(m_length + m_width + 1), -GLfloat(m_height / 2) - 1);
			glVertex2f(GLfloat(m_length + m_width + 1), GLfloat(m_height / 2) + 1);
			glVertex2f(-m_width - 1, GLfloat(m_height / 2) + 1);

			//Background
			m_colorTheme.m_back.useColor();
			glVertex2f(-m_width, -GLfloat(m_height / 2));
			glVertex2f(GLfloat(m_length + m_width), -GLfloat(m_height / 2));
			glVertex2f(GLfloat(m_length + m_width), GLfloat(m_height / 2));
			glVertex2f(-m_width, GLfloat(m_height / 2));

			m_colorTheme.m_fore.useColor();
			glVertex2f(-m_width, -GLfloat(m_height / 2));
			glVertex2f(GLfloat(m_slideValue), -GLfloat(m_height / 2));
			glVertex2f(GLfloat(m_slideValue), GLfloat(m_height / 2));
			glVertex2f(-m_width, GLfloat(m_height / 2));

			//Slider
			m_colorTheme.m_active.useColor();
			glVertex2f(GLfloat(m_slideValue - m_width), -GLfloat(m_height / 2));
			glVertex2f(GLfloat(m_slideValue + m_width), -GLfloat(m_height / 2));
			glVertex2f(GLfloat(m_slideValue + m_width), GLfloat(m_height / 2));
			glVertex2f(GLfloat(m_slideValue - m_width), GLfloat(m_height / 2));
		}
		glEnd();
	}
	glPopMatrix();

	m_colorTheme.m_textInfo.useColor();
	Font::getInstance().setAlignment(ALIGN_CENTER);
	Font::getInstance().print(m_title, Sint32(m_pos.x + m_length / 2), Sint32(m_pos.y - m_height));

	m_colorTheme.m_text.useColor();
	Font::getInstance().setAlignment(ALIGN_CENTER);
	Font::getInstance().print(Util::numToString(*m_numValue), Sint32(m_pos.x + m_length / 2), Sint32(m_pos.y));
}

Sint32 CSlider::getMaxValue()
{
	return m_maxValue;
}
Sint32 CSlider::getLength()
{
	return m_length;
}