#include "engine\gfx\gui\field\NumberField.h"

NumberField::NumberField(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Vector2<Sint32> p_numBounds, Sint32* p_numValue)
	: Component(p_compName, p_title, p_pos, {}, ACTION)
{
	m_sizeInit = p_size;
	m_size = Vector2<Sint32>(p_size.x, Sint32(p_size.y * Font::getSpacingHeight()));
	m_selected = 0;
	m_numValue = p_numValue;
	m_minNum = p_numBounds.x;
	m_maxNum = p_numBounds.y;
}

void NumberField::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	if((p_interactFlags & EVENT_MOUSEOVER) && p_mousePos.x >= 0 && p_mousePos.x < m_size.x
		&& p_mousePos.y >= 0 && p_mousePos.y < m_size.y)
	{
		addTooltip();
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS)
			m_selected = 1;
	}
	else if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS)
		m_selected = 0;
	if((p_interactFlags & EVENT_KEYPRESS) && m_selected != 0)
	{
		p_interactFlags -= EVENT_KEYPRESS;
		std::vector<GKey::keyPress> _keyEvents = GKey::m_keyEvents;
		for(Uint16 i = 0; i < _keyEvents.size(); i++)
		{
			if(_keyEvents[i].m_action != 0)
			{
				if(_keyEvents[i].m_keyCode == GLFW_KEY_ENTER || _keyEvents[i].m_keyCode == GLFW_KEY_TAB)
				{
					p_interactFlags += EVENT_KEYPRESS;
					m_selected = 0;
					callPressFunction();
				}
				else if(_keyEvents[i].m_keyCode == GLFW_KEY_ESCAPE)
				{
					m_selected = 0;
				}
				else if(_keyEvents[i].m_keyCode == GLFW_KEY_BACKSPACE)
				{
					*m_numValue = Sint32(floor(*m_numValue / 10.f));
				}
				else if(_keyEvents[i].m_keyCode >= GLFW_KEY_0 && _keyEvents[i].m_keyCode <= GLFW_KEY_9)
				{
					if((_keyEvents[i].m_mods & 1) == 0)
					{
						*m_numValue = *m_numValue * 10 + (_keyEvents[i].m_keyCode - GLFW_KEY_0);
						if(*m_numValue > m_maxNum)
							*m_numValue = m_maxNum;
						if(*m_numValue < m_minNum)
							*m_numValue = m_minNum;
					}
				}
				else if(_keyEvents[i].m_keyCode == GLFW_KEY_UP)
				{
					*m_numValue += (_keyEvents[i].m_mods & 1) ? 5 : 1;
					if(*m_numValue > m_maxNum)
						*m_numValue = m_maxNum;
					if(*m_numValue < m_minNum)
						*m_numValue = m_minNum;
				}
				else if(_keyEvents[i].m_keyCode == GLFW_KEY_DOWN)
				{
					*m_numValue -= (_keyEvents[i].m_mods & 1) ? 5 : 1;
					if(*m_numValue > m_maxNum)
						*m_numValue = m_maxNum;
					if(*m_numValue < m_minNum)
						*m_numValue = m_minNum;
				}
			}
		}
	}

	if((p_interactFlags & EVENT_MOUSEOVER) &&
	   p_mousePos.x >= 0 && p_mousePos.x < m_size.x
	   && p_mousePos.y >= 0 && p_mousePos.y < m_size.y)
		p_interactFlags -= EVENT_MOUSEOVER;
}

void NumberField::update(GLfloat p_deltaUpdate)
{

}

void NumberField::render()
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		glBegin(GL_QUADS);
		{
			m_colorTheme.m_border.useColor();
			glVertex2f(-1, -1);
			glVertex2f(GLfloat(m_size.x + 1), -1);
			glVertex2f(GLfloat(m_size.x + 1), GLfloat(m_size.y + 1));
			glVertex2f(-1, GLfloat(m_size.y + 1));

			if(m_selected) m_colorTheme.m_select.useColor();
			else m_colorTheme.m_primary.useColor();
			glVertex2f(0, 0);
			glVertex2f(GLfloat(m_size.x), 0);
			glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
			glVertex2f(0, GLfloat(m_size.y));
		}
		glEnd();
		m_colorTheme.m_textLight.useColor();
		Font::setAlignment(ALIGN_RIGHT);
		Font::print(m_title, -2, Sint32(0.5f * Font::getSpacingHeight()));
		m_colorTheme.m_text.useColor();
		Font::setAlignment(ALIGN_LEFT);
		Font::print(Util::numToString(*m_numValue), 2, Sint32(0.5f * Font::getSpacingHeight()));
		if(m_selected != 0 && (fmod(glfwGetTime(), 0.5) < 0.25))
			Font::print("|", Font::getMessageWidth(Util::numToString(*m_numValue)).x, Sint32(0.5f * Font::getSpacingHeight()));
	}
	glPopMatrix();
}