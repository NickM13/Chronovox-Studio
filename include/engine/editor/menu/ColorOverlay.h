#pragma once

#include "engine\gfx\gui\Component.h"

class ColorOverlay : public Component
{
public:
	ColorOverlay(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32* p_r, Sint32* p_g, Sint32* p_b, Sint32* p_hue, Sint32* p_sat, Sint32* p_val)
	{
		m_pos = p_pos;
		m_size = p_size;
		r = p_r;
		g = p_g;
		b = p_b;
		hue = p_hue;
		sat = p_sat;
		val = p_val;

		m_background1 = LTexture::getTexture("gui\\ColorSquare.png");
		m_background2 = LTexture::getTexture("gui\\GraySquare.png");
		m_selectTex = LTexture::getTexture("gui\\ColorSelect.png");
	}

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
	{
		p_mousePos = p_mousePos - m_pos;
		Vector2<GLfloat> _percent = Vector2<GLfloat>(p_mousePos) / Vector2<GLfloat>(m_size);
		_percent = Vector2<GLfloat>(min(1, max(0, _percent.x)), min(1, max(0, _percent.y)));
		if(!(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN))
			held = false;
		if((p_interactFlags & EVENT_MOUSEOVER) && p_mousePos.x >= 0 && p_mousePos.y >= 0 && p_mousePos.x <= m_size.x && p_mousePos.y <= m_size.y)
			if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
				held = true;

		if(held)
		{
			*hue = _percent.x * 360;
			*val = _percent.y * 100;
			p_interactFlags -= EVENT_MOUSEOVER;
		}
	}
	void update(GLfloat p_deltaUpdate)
	{
		if(*r != pr || *g != pg || *b != pb)
		{
			GLfloat max = (max(max(*r, *g), *b)) / 255.f, min = (min(min(*r, *g), *b)) / 255.f;

			*val = Sint32(max * 100);
			if(max != 0)
				*sat = Sint32((max - min) / max * 100);
			else
				*sat = 100;
			if(*r != *g || *g != *b || *b != *r)
			{
				if(*r >= *g && *r >= *b) *hue = (*g - *b) / ((max - min) * 255) * -60;
				else if(*g >= *r && *g >= *b) *hue = (2 + (*b - *r) / ((max - min) * 255)) * -60;
				else  *hue = (4 + (*r - *g) / ((max - min) * 255)) * -60;
			}

			if(*hue < 0) *hue += 360;
		}
		else if(*hue != phue || *sat != psat || *val != pval)
		{
			if(*hue <= 60 || *hue >= 300)	*r = 255;
			else if(*hue <= 120)			*r = ((120 - *hue) / 60.f) * 255;
			else if(*hue <= 240)			*r = 0;
			else						*r = ((*hue - 240) / 60.f) * 255;

			if(*hue <= 60)				*b = ((*hue) / 60.f) * 255;
			else if(*hue <= 180)			*b = 255;
			else if(*hue <= 240)			*b = ((240 - *hue) / 60.f) * 255;
			else						*b = 0;

			if(*hue <= 120)				*g = 0;
			else if(*hue <= 180)			*g = ((*hue - 120) / 60.f) * 255;
			else if(*hue <= 300)			*g = 255;
			else						*g = ((360 - *hue) / 60.f) * 255;

			*r += (255 - *r) * (1.f - (*sat / 100.f));
			*g += (255 - *g) * (1.f - (*sat / 100.f));
			*b += (255 - *b) * (1.f - (*sat / 100.f));

			*r *= *val / 100.f;
			*g *= *val / 100.f;
			*b *= *val / 100.f;
		}
		else return;
		phue = *hue;
		psat = *sat;
		pval = *val;
		pr = *r;
		pg = *g;
		pb = *b;
	}
	void render()
	{
		glPushMatrix();
		{
			glTranslatef(m_pos.x, m_pos.y, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
			glColor3f(0, 0, 0);
			glBegin(GL_QUADS);
			{
				glVertex2f(-1, -1);
				glVertex2f(m_size.x + 1, -1);
				glVertex2f(m_size.x + 1, m_size.y + 1);
				glVertex2f(-1, m_size.y + 1);
			}
			glEnd();

			glBindTexture(GL_TEXTURE_2D, m_background1.getId());
			glColor4f(1, 1, 1, (*sat / 100.f));
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0); glVertex2f(0, 0);
				glTexCoord2f(1, 0); glVertex2f(m_size.x, 0);
				glTexCoord2f(1, 1); glVertex2f(m_size.x, m_size.y);
				glTexCoord2f(0, 1); glVertex2f(0, m_size.y);
			}
			glEnd();

			glBindTexture(GL_TEXTURE_2D, m_background2.getId());
			glColor4f(1, 1, 1, 1.f - (*sat / 100.f));
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0); glVertex2f(0, 0);
				glTexCoord2f(1, 0); glVertex2f(m_size.x, 0);
				glTexCoord2f(1, 1); glVertex2f(m_size.x, m_size.y);
				glTexCoord2f(0, 1); glVertex2f(0, m_size.y);
			}
			glEnd();

			glBindTexture(GL_TEXTURE_2D, 0);
			glTranslatef(*hue / 360.f * m_size.x, *val / 100.f * m_size.x, 0);
			glColor3f(*r / 255.f, *g / 255.f, *b / 255.f);
			glBegin(GL_QUADS);
			{
				glVertex2f(-6, -6);
				glVertex2f(6, -6);
				glVertex2f(6, 6);
				glVertex2f(-6, 6);
			}
			glEnd();

			glBindTexture(GL_TEXTURE_2D, m_selectTex.getId());
			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 1); glVertex2f(-8, -8);
				glTexCoord2f(1, 1); glVertex2f(8, -8);
				glTexCoord2f(1, 0); glVertex2f(8, 8);
				glTexCoord2f(0, 0); glVertex2f(-8, 8);
			}
			glEnd();
		}
		glPopMatrix();
	}
private:
	Texture m_background1, m_background2, m_selectTex;

	Sint32 *hue, *sat, *val;
	Sint32 phue, psat, pval;
	Sint32 *r, *g, *b;
	Sint32 pr, pg, pb;

	bool held;
};
