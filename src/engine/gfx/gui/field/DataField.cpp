#include "engine\gfx\gui\field\DataField.h"

void DataField::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	if((p_interactFlags & EVENT_MOUSEOVER) && p_mousePos.x >= 0 && p_mousePos.x < m_size.x
		&& p_mousePos.y >= 0 && p_mousePos.y < m_size.y)
	{
		addTooltip();
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
			m_selected = 1;
	}
	else if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
		m_selected = 0;

	if((p_interactFlags & EVENT_MOUSEOVER) &&
		p_mousePos.x >= 0 && p_mousePos.x < m_size.x
		&& p_mousePos.y >= 0 && p_mousePos.y < m_size.y)
		p_interactFlags -= EVENT_MOUSEOVER;
}

void DataField::update(GLfloat p_deltaUpdate)
{

}

void DataField::render()
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		m_colorTheme.m_text.useColor();
		Font::getInstance().setAlignment(ALIGN_LEFT);
		Font::getInstance().print(*m_dataString, 0, Sint32(Font::getInstance().getHeight()));
	}
	glPopMatrix();
}
