#include "engine\gfx\gui\icon\Icon.h"

CIcon::CIcon(std::string p_compName, Texture p_icon, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
{
	m_compName = p_compName;
	m_texture = p_icon.getId();
	m_pos = p_pos;
	m_size = p_size;
}

void CIcon::render()
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
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
