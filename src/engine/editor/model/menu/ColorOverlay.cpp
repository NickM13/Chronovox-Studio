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
	_percent = Vector2<GLfloat>(std::fminf(1, std::fmaxf(0, _percent.x)), std::fminf(1, std::fmaxf(0, _percent.y)));
	if (!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT))
		held = false;
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && _mousePos.x >= 0 && _mousePos.y >= 0 && _mousePos.x <= m_size.x && _mousePos.y <= m_size.y)
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
			held = true;

	if (held) {
		hue = _percent.x * 360;
		val = _percent.y * 100;
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
}
void ColorOverlay::update(GLfloat p_deltaUpdate) {
	Sint32 r = color.r * 255, g = color.g * 255, b = color.b * 255;
	if (r != pr || g != pg || b != pb) {
		GLfloat max = (std::fmaxf(std::fmaxf(r, g), b)) / 255.f, min = (std::fminf(std::fminf(r, g), b)) / 255.f;
		val = Sint32(max * 100);
		if (std::fmaxf != 0)
			sat = Sint32((max - min) / max * 100);
		else
			sat = 100;
		if (r != g || g != b || b != r) {
			if (r >= g && r >= b) hue = (g - b) / ((max - min) * 255) * -60;
			else if (g >= r && g >= b) hue = (2 + (b - r) / ((max - min) * 255)) * -60;
			else  hue = (4 + (r - g) / ((max - min) * 255)) * -60;
		}
		if (hue < 0) hue += 360;
		callPressFunction();
	}
	else if (hue != phue || sat != psat || val != pval) {
		if (hue <= 60 || hue >= 300)	r = 255;
		else if (hue <= 120)			r = ((120 - hue) / 60.f) * 255;
		else if (hue <= 240)			r = 0;
		else						r = ((hue - 240) / 60.f) * 255;

		if (hue <= 60)				b = ((hue) / 60.f) * 255;
		else if (hue <= 180)			b = 255;
		else if (hue <= 240)			b = ((240 - hue) / 60.f) * 255;
		else						b = 0;

		if (hue <= 120)				g = 0;
		else if (hue <= 180)			g = ((hue - 120) / 60.f) * 255;
		else if (hue <= 300)			g = 255;
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
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0));

	GBuffer::setTexture(0);
	GBuffer::setColor(Color(0.f, 0.f, 0.f));
	GBuffer::addVertexQuad(-1, -1);
	GBuffer::addVertexQuad(m_size.x + 1, -1);
	GBuffer::addVertexQuad(m_size.x + 1, m_size.y + 1);
	GBuffer::addVertexQuad(-1, m_size.y + 1);

	GBuffer::setTexture(m_background1->getGlId());
	GBuffer::setColor(Color(1.f, 1.f, 1.f, (sat / 100.f)));
	GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, 0);
	GBuffer::setUV(1, 0); GBuffer::addVertexQuad(m_size.x, 0);
	GBuffer::setUV(1, 1); GBuffer::addVertexQuad(m_size.x, m_size.y);
	GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, m_size.y);

	GBuffer::setTexture(m_background2->getGlId());
	GBuffer::setColor(Color(1.f, 1.f, 1.f, 1.f - (sat / 100.f)));
	GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, 0);
	GBuffer::setUV(1, 0); GBuffer::addVertexQuad(m_size.x, 0);
	GBuffer::setUV(1, 1); GBuffer::addVertexQuad(m_size.x, m_size.y);
	GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, m_size.y);;

	GBuffer::setScissorActive(false);

	GBuffer::setTexture(0);
	Shader::translate(glm::vec3(hue / 360.f * m_size.x, val / 100.f * m_size.x, 0));
	GBuffer::setColor(color);
	GBuffer::addVertexQuad(-6, -6);
	GBuffer::addVertexQuad(6, -6);
	GBuffer::addVertexQuad(6, 6);
	GBuffer::addVertexQuad(-6, 6);

	GBuffer::setTexture(m_selectTex->getGlId());
	GBuffer::setColor(Color(1.f, 1.f, 1.f));
	GBuffer::setUV(0, 1); GBuffer::addVertexQuad(-8, -8);
	GBuffer::setUV(1, 1); GBuffer::addVertexQuad(8, -8);
	GBuffer::setUV(1, 0); GBuffer::addVertexQuad(8, 8);
	GBuffer::setUV(0, 0); GBuffer::addVertexQuad(-8, 8);

	GBuffer::setScissorActive(true);
	Shader::popMatrixModel();
}
