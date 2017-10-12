#include "engine\gfx\gui\container\Panel.h"


Panel::Panel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme, Sint8 p_borderFlags)
	: Component(p_compName, p_title, p_pos, p_size, p_colorTheme)
{
	m_border = p_borderFlags;
	m_visible = true;
}

void Panel::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	if(p_interactFlags & EVENT_MOUSEOVER) {
		if(m_title != "") {
			if(p_mousePos.x - m_pos.x >= 0 && p_mousePos.x - m_pos.x < m_size.x &&
				p_mousePos.y - m_pos.y >= -24 && p_mousePos.y - m_pos.y < 0) {
				m_selected = 1;
				callPressFunction();
			}
		} else {
			if(p_mousePos.x - m_pos.x >= 0 && p_mousePos.x - m_pos.x < m_size.x &&
				p_mousePos.y - m_pos.y >= 0 && p_mousePos.y - m_pos.y < m_size.y) {
				p_interactFlags -= EVENT_MOUSEOVER;
				if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS) {
					m_selected = 1;
					callPressFunction();
				}
			}
		}
	}
	if(!(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_DOWN) && m_selected == 1)
	{
		m_selected = 0;
		callReleaseFunction();
	}
}
void Panel::update(GLfloat p_updateTime)
{

}
void Panel::render()
{
	if(m_visible)
	{
		if(m_title != "") {
			Sint8 _ignore = m_selected;
			m_selected = 0;
			Component::render();
			m_selected = _ignore;
		}
		else Component::render();
		glPushMatrix();
		{
			glTranslatef(0, -24, 0);
			if(m_title != "") {
				glBegin(GL_QUADS);
				{
					m_colorTheme.m_border.useColor();
					glVertex2f(-1, -1);
					glVertex2f((GLfloat)m_size.x + 1, -1);
					glVertex2f((GLfloat)m_size.x + 1, 25);
					glVertex2f(-1, 25);

					m_colorTheme.m_primary.useColor();
					glVertex2f(0, 0);
					glVertex2f((GLfloat)m_size.x, 0);
					glVertex2f((GLfloat)m_size.x, 24);
					glVertex2f(0, 24);
				}
				glEnd();

				m_colorTheme.m_text.useColor();
				Font::setAlignment(ALIGN_CENTER);
				Font::print(m_title, m_size.x / 2, 12);
			}
		}
		glPopMatrix();
	}
}
