#include "engine\gfx\gui\container\Panel.h"


Panel::Panel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme, Sint8 p_borderFlags)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_size = p_size;
	if(m_title != "")
	{
		m_pos.y -= 24;
		m_size.y += 24;
	}
	m_colorTheme = m_colorThemes[p_colorTheme];
	m_border = p_borderFlags;
	m_visible = true;
}

void Panel::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	if((p_interactFlags & EVENT_MOUSEOVER) &&
		p_mousePos.x - m_pos.x >= 0 && p_mousePos.x - m_pos.x < m_size.x && 
		p_mousePos.y - m_pos.y >= 0 && p_mousePos.y - m_pos.y < m_size.y)
		p_interactFlags -= EVENT_MOUSEOVER;
}
void Panel::update(GLfloat p_updateTime)
{

}
void Panel::render()
{
	if(m_visible)
	{
		Component::render();
		if(m_title != "")
		{
			glPushMatrix();
			{
				glTranslatef(m_pos.x, m_pos.y, 0);
				m_colorTheme.m_active.useColor();
				glBegin(GL_QUADS);
				{
					glVertex2f(0, 0);
					glVertex2f(GLfloat(m_size.x), 0);
					glVertex2f(GLfloat(m_size.x), 24);
					glVertex2f(0, 24);
				}
				glEnd();
				glColor3f(0, 0, 0);
				glBegin(GL_QUADS);
				{
					glVertex2f(GLfloat(0), 24);
					glVertex2f(GLfloat(m_size.x), 24);
					glVertex2f(GLfloat(m_size.x), 25);
					glVertex2f(GLfloat(0), 25);
				}
				glEnd();

				m_colorTheme.m_text.useColor();
				Font::getInstance().setAlignment(ALIGN_CENTER);
				Font::getInstance().print(m_title, m_size.x / 2, 12);
			}
			glPopMatrix();
		}
	}
}
