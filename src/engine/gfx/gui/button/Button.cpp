#include "engine\gfx\gui\button\Button.h"

CButton::CButton(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_showBorder, function p_func)
{
	m_selected = 0;
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_size = p_size;
	m_colorTheme = m_colorThemes[ACTION];
	m_texture = -1;
	m_showBorder = p_showBorder;
	m_pressFunction = p_func;

	m_soundClick.setSound(MBuffer::getInstance().getUnit("gui\\Click.wav"));
	m_soundHover.setSound(MBuffer::getInstance().getUnit("gui\\Hover.wav"));
}
CButton::CButton(std::string p_compName, std::string p_title, Texture p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_showBorder, function p_func)
{
	m_selected = 0;
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_size = p_size;
	m_colorTheme = m_colorThemes[ACTION];
	m_texture = -1;
	m_showBorder = p_showBorder;
	m_pressFunction = p_func;

	m_soundClick.setSound(MBuffer::getInstance().getUnit("gui\\Click.wav"));
	m_soundHover.setSound(MBuffer::getInstance().getUnit("gui\\Hover.wav"));
	m_buttonTex = p_buttonTex;
}
void CButton::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	if((p_interactFlags & EVENT_MOUSEOVER) &&
		p_mousePos.x >= m_pos.x && p_mousePos.x <= m_pos.x + m_size.x &&
		p_mousePos.y >= m_pos.y && p_mousePos.y <= m_pos.y + m_size.y)
	{
		if(!m_hover)
		{
			m_soundHover.play2d();
			m_hover = true;
		}
		p_interactFlags -= EVENT_MOUSEOVER;
	}
	else
		m_hover = false;

	if(m_hover)
	{
		addTooltip();
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_DOWN)
		{
			if((p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS) || m_selected != 0)
			{
				if(m_selected == 0) m_selected = 1;
				else m_selected = 2;
				return;
			}
		}
		else
		{
			if(m_selected == 1 || m_selected == 2)
			{
				m_selected = 3;
				return;
			}
			else if(m_selected == 3)
			{
				m_soundClick.play2d();
				m_selected = 0;
				if(m_pressFunction)
					m_pressFunction();
				return;
			}
		}
	}
	m_selected = 0;
}
void CButton::update(GLfloat p_deltaUpdate)
{
	if(m_stuck)
	{
		m_selected = 0;
		m_stuck = false;
	}
	else if(m_selected == 3)
		m_stuck = true;
	else
		m_stuck = false;
}
void CButton::render()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	if(m_showBorder)
		Component::renderBack();
	if(m_selected)
		m_colorTheme.m_select.useColor();
	else
	{
		if(m_hover)
			Color((m_colorTheme.m_select / 2 + m_colorTheme.m_primary / 2)).useColor();
		else
			m_colorTheme.m_primary.useColor();
	}
	Component::renderFill(false);

	glPushMatrix();
	{
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		glTranslatef(GLfloat(m_size.x / 2), GLfloat(m_size.y / 2), 0);

		if(m_buttonTex.getId() != 0)
		{
			glPushMatrix();
			{
				glColor3f(1, 1, 1);
				glBindTexture(GL_TEXTURE_2D, m_buttonTex.getId());
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0, 1);
					glVertex2f(-GLfloat(m_buttonTex.getSize().x) / 2, -GLfloat(m_buttonTex.getSize().y) / 2);
					glTexCoord2f(1, 1);
					glVertex2f(GLfloat(m_buttonTex.getSize().x) / 2, -GLfloat(m_buttonTex.getSize().y) / 2);
					glTexCoord2f(1, 0);
					glVertex2f(GLfloat(m_buttonTex.getSize().x) / 2, GLfloat(m_buttonTex.getSize().y) / 2);
					glTexCoord2f(0, 0);
					glVertex2f(-GLfloat(m_buttonTex.getSize().x) / 2, GLfloat(m_buttonTex.getSize().y) / 2);
				}
				glEnd();
			}
			glPopMatrix();
		}


		m_colorTheme.m_text.useColor();
		Font::getInstance().setAlignment(ALIGN_CENTER);
		Font::getInstance().print(m_title, 0, 0);
	}
	glPopMatrix();
}

void CButton::setState(Sint8 p_selected)
{
	m_selected = p_selected;
	if(p_selected == 1 && m_pressFunction != 0)
		m_pressFunction();
}

Sint8 CButton::isSelected() // 0 = not selected, 1 = JUST pressed, 2 = is held, 3 = JUST released
{
	if(!m_visible) return 0;
	return m_selected;
}
