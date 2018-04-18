#include "engine\gfx\gui\container\Panel.h"

Panel::Panel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme, Sint8 p_borderFlags)
	: Component(p_compName, p_title, p_pos, p_size, p_colorTheme) {
	m_border = p_borderFlags;
	m_visible = true;
}

void Panel::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	if(p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
		if(m_title != "") {
			if(_mousePos.x - m_pos.x >= 0 && _mousePos.x - m_pos.x < m_size.x &&
				_mousePos.y - m_pos.y >= -24 && _mousePos.y - m_pos.y < 0) {
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
				m_selected = 1;
				callPressFunction();
			}
		}
		if(_mousePos.x - m_pos.x >= 0 && _mousePos.x - m_pos.x < m_size.x &&
			_mousePos.y - m_pos.y >= 0 && _mousePos.y - m_pos.y < m_size.y) {
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
			if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				m_selected = 1;
				callPressFunction();
			}
		}
	}
	if(!(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) && m_selected == 1) {
		m_selected = 0;
		callReleaseFunction();
	}
}
void Panel::update(GLfloat p_updateTime) {

}
void Panel::render() {
	if(m_visible) {
		if(m_title != "") {
			Sint8 _ignore = m_selected;
			m_selected = 0;
			Component::render();
			m_selected = _ignore;
		}
		else Component::render();
		if(m_title != "") {
			glPushMatrix();
			{
				glTranslatef(m_pos.x, m_pos.y - 24, 0);
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
			glPopMatrix();
		}
	}
}
