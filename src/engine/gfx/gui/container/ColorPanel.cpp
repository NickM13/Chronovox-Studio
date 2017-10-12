#include "engine\gfx\gui\container\ColorPanel.h"

ColorPanel::ColorPanel(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32& p_r, Sint32& p_g, Sint32& p_b, Texture p_texture)
	: Component("", "", p_pos, p_size, 0)
{
	r = &p_r;
	g = &p_g;
	b = &p_b;
	m_texture = p_texture;
}

void ColorPanel::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	if((p_interactFlags & EVENT_MOUSEOVER) && p_mousePos.x >= 0 && p_mousePos.y >= 0 && p_mousePos.x < m_size.x && p_mousePos.y < m_size.y)
		p_interactFlags -= EVENT_MOUSEOVER;
}
void ColorPanel::render()
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, m_texture.getId());

		glColor3f(*r / 255.f, *g / 255.f, *b / 255.f);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1); glVertex2f(0, 0);
			glTexCoord2f(1, 1); glVertex2f(GLfloat(m_size.x), 0);
			glTexCoord2f(1, 0); glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
			glTexCoord2f(0, 0); glVertex2f(0, GLfloat(m_size.y));
		}
		glEnd();
	}
	glPopMatrix();
}


SColorPanel::SColorPanel(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Color color, Texture p_texture)
{
	m_compName = p_compName;
	m_pos = p_pos;
	m_size = p_size;
	m_color = color;
	m_texture = p_texture;
}
SColorPanel::SColorPanel(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Color color, Texture p_texture)
{
	m_pos = p_pos;
	m_size = p_size;
	m_color = color;
	m_texture = p_texture;
}

void SColorPanel::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	if((p_interactFlags & EVENT_MOUSEOVER) && p_mousePos.x >= 0 && p_mousePos.y >= 0 && p_mousePos.x < m_size.x && p_mousePos.y < m_size.y)
	{
		p_interactFlags -= EVENT_MOUSEOVER;
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_DOWN)
			callPressFunction();
	}
}
void SColorPanel::render()
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, m_texture.getId());

		m_color.useColor();
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1); glVertex2f(0, 0);
			glTexCoord2f(1, 1); glVertex2f(GLfloat(m_size.x), 0);
			glTexCoord2f(1, 0); glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
			glTexCoord2f(0, 0); glVertex2f(0, GLfloat(m_size.y));
		}
		glEnd();
	}
	glPopMatrix();
}

Color SColorPanel::getColor()
{
	return m_color;
}
