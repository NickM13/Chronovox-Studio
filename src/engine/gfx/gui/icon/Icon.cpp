#include "engine\gfx\gui\icon\Icon.h"

CIcon::CIcon(std::string p_compName, Texture* p_icon, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Component(p_compName, "", p_pos, p_size, Theme::PRIMARY) {
	m_texture = p_icon;
}

void CIcon::render() {
	glPushMatrix();
	{
		m_texture->bind();
		glTranslatef(m_pos.x, m_pos.y, 0);
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);	glVertex2f(0, 0);
			glTexCoord2f(1, 1); glVertex2f(m_size.x, 0);
			glTexCoord2f(1, 0); glVertex2f(m_size.x, m_size.y);
			glTexCoord2f(0, 0); glVertex2f(0, m_size.y);
		}
		glEnd();
	}
	glPopMatrix();
}
