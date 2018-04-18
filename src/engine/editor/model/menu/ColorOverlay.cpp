#include "engine\editor\model\menu\ColorOverlay.h"

ColorOverlay::ColorOverlay(Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Component("OVERLAY_COLOR", "", p_pos, p_size, Theme::PRIMARY) {
	m_pos = p_pos;
	m_size = p_size;

	m_background1 = MTexture::getTexture("gui\\ColorSquare.png");
	m_background2 = MTexture::getTexture("gui\\GraySquare.png");
	m_selectTex = MTexture::getTexture("gui\\ColorSelect.png");

	color = Color(0, 0, 0);
	hue = sat = val = 0;
	held = false;
}

void ColorOverlay::setColorRGB(Sint32 r, Sint32 g, Sint32 b) {
	color.r = r / 255.f;
	color.g = g / 255.f;
	color.b = b / 255.f;
}
void ColorOverlay::setColorHSV(Sint32 hue, Sint32 sat, Sint32 val) {
	this->hue = hue;
	this->sat = sat;
	this->val = val;
}

void ColorOverlay::input(Sint8& p_interactFlags) {
	Vector2<GLfloat> _mousePos = GMouse::getMousePos() - m_pos;
	Vector2<GLfloat> _percent = Vector2<GLfloat>(_mousePos) / Vector2<GLfloat>(m_size);
	_percent = Vector2<GLfloat>(min(1, max(0, _percent.x)), min(1, max(0, _percent.y)));
	if(!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT))
		held = false;
	if((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && _mousePos.x >= 0 && _mousePos.y >= 0 && _mousePos.x <= m_size.x && _mousePos.y <= m_size.y)
		if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
			held = true;

	if(held) {
		hue = _percent.x * 360;
		val = _percent.y * 100;
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
}
void ColorOverlay::update(GLfloat p_deltaUpdate) {
	Sint32 r = color.r * 255, g = color.g * 255, b = color.b * 255;
	if(r != pr || g != pg || b != pb) {
		GLfloat max = (max(max(r, g), b)) / 255.f, min = (min(min(r, g), b)) / 255.f;
		val = Sint32(max * 100);
		if(max != 0)
			sat = Sint32((max - min) / max * 100);
		else
			sat = 100;
		if(r != g || g != b || b != r) {
			if(r >= g && r >= b) hue = (g - b) / ((max - min) * 255) * -60;
			else if(g >= r && g >= b) hue = (2 + (b - r) / ((max - min) * 255)) * -60;
			else  hue = (4 + (r - g) / ((max - min) * 255)) * -60;
		}
		if(hue < 0) hue += 360;
		callPressFunction();
	}
	else if(hue != phue || sat != psat || val != pval) {
		if(hue <= 60 || hue >= 300)	r = 255;
		else if(hue <= 120)			r = ((120 - hue) / 60.f) * 255;
		else if(hue <= 240)			r = 0;
		else						r = ((hue - 240) / 60.f) * 255;

		if(hue <= 60)				b = ((hue) / 60.f) * 255;
		else if(hue <= 180)			b = 255;
		else if(hue <= 240)			b = ((240 - hue) / 60.f) * 255;
		else						b = 0;

		if(hue <= 120)				g = 0;
		else if(hue <= 180)			g = ((hue - 120) / 60.f) * 255;
		else if(hue <= 300)			g = 255;
		else						g = ((360 - hue) / 60.f) * 255;

		r += (255 - r) * (1.f - (sat / 100.f));
		g += (255 - g) * (1.f - (sat / 100.f));
		b += (255 - b) * (1.f - (sat / 100.f));

		r *= val / 100.f;
		g *= val / 100.f;
		b *= val / 100.f;

		color.r = r / 255.f;
		color.g = g / 255.f;
		color.b = b / 255.f;

		callPressFunction();
	}
	else return;
	phue = hue;
	psat = sat;
	pval = val;
	pr = r;
	pg = g;
	pb = b;
}
void ColorOverlay::render() {
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

		glBindTexture(GL_TEXTURE_2D, m_background1->getId());
		glColor4f(1, 1, 1, (sat / 100.f));
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0); glVertex2f(0, 0);
			glTexCoord2f(1, 0); glVertex2f(m_size.x, 0);
			glTexCoord2f(1, 1); glVertex2f(m_size.x, m_size.y);
			glTexCoord2f(0, 1); glVertex2f(0, m_size.y);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, m_background2->getId());
		glColor4f(1, 1, 1, 1.f - (sat / 100.f));
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0); glVertex2f(0, 0);
			glTexCoord2f(1, 0); glVertex2f(m_size.x, 0);
			glTexCoord2f(1, 1); glVertex2f(m_size.x, m_size.y);
			glTexCoord2f(0, 1); glVertex2f(0, m_size.y);
		}
		glEnd();

		MScissor::getInstance().toggle();
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(hue / 360.f * m_size.x, val / 100.f * m_size.x, 0);
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_QUADS);
		{
			glVertex2f(-6, -6);
			glVertex2f(6, -6);
			glVertex2f(6, 6);
			glVertex2f(-6, 6);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, m_selectTex->getId());
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1); glVertex2f(-8, -8);
			glTexCoord2f(1, 1); glVertex2f(8, -8);
			glTexCoord2f(1, 0); glVertex2f(8, 8);
			glTexCoord2f(0, 0); glVertex2f(-8, 8);
		}
		glEnd();
		MScissor::getInstance().toggle();
	}
	glPopMatrix();
}
