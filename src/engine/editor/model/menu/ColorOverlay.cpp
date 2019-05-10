#include "engine\editor\model\menu\ColorOverlay.h"

ColorOverlay::ColorOverlay(Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Component("OVERLAY_COLOR", "", p_pos, p_size) {
	m_pos = p_pos;
	m_size = p_size;

	m_background1 = MTexture::getTexture("gui\\palette\\WhiteSquare.png");
	m_background2 = MTexture::getTexture("gui\\palette\\BlackSquare.png");
	m_background3 = MTexture::getTexture("gui\\palette\\ColorSquare.png");
	m_selectTexFill = MTexture::getTexture("gui\\palette\\ColorSelectFill.png");
	m_selectTexOut = MTexture::getTexture("gui\\palette\\ColorSelectOut.png");

	color = Color(0, 0, 0);
	hue = sat = val = 0;
	setColorRGB(255, 0, 0);
	held = false;

	m_hueBar.hueTex = MTexture::getTexture("gui\\palette\\HueBar.png");
	m_hueBar.pickTex = MTexture::getTexture("gui\\palette\\HuePicker.png");
	m_hueBar.x = m_size.x + 16;
	m_hueBar.y = 0;
	m_hueBar.w = 16;
	m_hueBar.h = m_size.y;
	m_hueBar.held = false;

	m_savedColors.highlightTex = MTexture::getTexture("gui\\palette\\SavedColorHighlight.png");
	m_savedColors.nColors = 32;
	m_savedColors.columns = 2;
	m_savedColors.rows = m_savedColors.nColors / m_savedColors.columns;
	m_savedColors.colors = new ColorSquare*[m_savedColors.nColors];
	m_savedColors.offset = { -34, 0 };
	m_savedColors.size = { 20, m_size.y };
	m_savedColors.colorSize = { m_savedColors.size.x / m_savedColors.columns, m_savedColors.size.y / m_savedColors.rows };

	for (Sint32 i = 0; i < m_savedColors.nColors; i++) {
		m_savedColors.colors[i] = new ColorSquare(i, Color(), m_savedColors.offset, m_savedColors.colorSize, m_savedColors.columns);
	}

	m_savedColors.colors[0]->setColor(Color("0x000000"));
	m_savedColors.colors[1]->setColor(Color("0xFFFFFF"));
	m_savedColors.colors[2]->setColor(Color("0x404040"));
	m_savedColors.colors[3]->setColor(Color("0x808080"));
	for (Sint32 i = 0; i < 12; i++) {
		Sint32 r, g, b;
		Sint32 h = 360 / 12 * i;

		if (h <= 60 || h >= 300)	r = 255;
		else if (h <= 120)			r = ((120 - h) / 60.f) * 255;
		else if (h <= 240)			r = 0;
		else						r = ((h - 240) / 60.f) * 255;

		if (h <= 60)				b = ((h) / 60.f) * 255;
		else if (h <= 180)			b = 255;
		else if (h <= 240)			b = ((240 - h) / 60.f) * 255;
		else						b = 0;

		if (h <= 120)				g = 0;
		else if (h <= 180)			g = ((h - 120) / 60.f) * 255;
		else if (h <= 300)			g = 255;
		else						g = ((360 - h) / 60.f) * 255;

		m_savedColors.colors[i * 2 + 4]->setColor(Color(r / 255.f, g / 255.f, b / 255.f));
		m_savedColors.colors[i * 2 + 5]->setColor(Color(r / 512.f, g / 512.f, b / 512.f));
	}
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
	if (!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		held = false;
		m_hueBar.held = false;
	}
	if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			if (_mousePos.x >= 0 &&
				_mousePos.y >= 0 &&
				_mousePos.x <= m_size.x &&
				_mousePos.y <= m_size.y) {
				held = true;
			}
			if (_mousePos.x >= m_hueBar.x &&
				_mousePos.y >= m_hueBar.y &&
				_mousePos.x <= m_hueBar.x + m_hueBar.w &&
				_mousePos.y <= m_hueBar.y + m_hueBar.h) {
				m_hueBar.held = true;
			}
		}
		if (_mousePos.x >= m_savedColors.offset.x &&
			_mousePos.y >= m_savedColors.offset.y &&
			_mousePos.x < m_savedColors.offset.x + m_savedColors.size.x &&
			_mousePos.y < m_savedColors.offset.y + m_savedColors.size.y) {
			Sint32 cx = static_cast<Sint32>(std::floorf(static_cast<GLfloat>(_mousePos.x - m_savedColors.offset.x) / (m_savedColors.size.x / m_savedColors.columns)));
			Sint32 cy = static_cast<Sint32>(std::floorf(static_cast<GLfloat>(_mousePos.y - m_savedColors.offset.y) / (m_savedColors.size.y / m_savedColors.rows)) * m_savedColors.columns);
			Sint32 ci = cx + cy;
			m_savedColors.hovered = ci;
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				Color c = m_savedColors.colors[ci]->getColor();
				setColorRGB(c.r * 255, c.g * 255, c.b * 255);
			}
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
				m_savedColors.colors[ci]->setColor(color);
			}
			m_savedColors.colors[ci]->addTooltip();
		}
		else {
			m_savedColors.hovered = -1;
		}
	}

	if (held) {
		sat = _percent.x * 100;
		val = 100 - _percent.y * 100;
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		addTooltip();
	}
	if (m_hueBar.held) {
		hue = _percent.y * 360;
	}
}
void ColorOverlay::update(GLfloat p_deltaUpdate) {
	Sint32 r = color.r * 255, g = color.g * 255, b = color.b * 255;
	// Convert RGB to HSV
	if (r != pr || g != pg || b != pb) {
		GLfloat max = (std::fmaxf(std::fmaxf(r, g), b)) / 255.f, min = (std::fminf(std::fminf(r, g), b)) / 255.f;
		val = Sint32(max * 100);
		if (max != 0)
			sat = Sint32((max - min) / max * 100);
		else
			sat = 0;
		if (r != g || g != b || b != r) {
			if (r >= g && r >= b) hue = (g - b) / ((max - min) * 255) * -60;
			else if (g >= r && g >= b) hue = (2 + (b - r) / ((max - min) * 255)) * -60;
			else  hue = (4 + (r - g) / ((max - min) * 255)) * -60;
		}
		if (hue < 0) hue = hue % 360 + 360;
		if (hue > 360) hue = hue % 360;

		if (hue <= 60 || hue >= 300)	hr = 255;
		else if (hue <= 120)			hr = ((120 - hue) / 60.f) * 255;
		else if (hue <= 240)			hr = 0;
		else							hr = ((hue - 240) / 60.f) * 255;

		if (hue <= 60)					hb = ((hue) / 60.f) * 255;
		else if (hue <= 180)			hb = 255;
		else if (hue <= 240)			hb = ((240 - hue) / 60.f) * 255;
		else							hb = 0;

		if (hue <= 120)					hg = 0;
		else if (hue <= 180)			hg = ((hue - 120) / 60.f) * 255;
		else if (hue <= 300)			hg = 255;
		else							hg = ((360 - hue) / 60.f) * 255;

		callPressFunction();
	} else if (hue != phue || sat != psat || val != pval) { // Convert HSV to RGB
		if (hue <= 60 || hue >= 300)	hr = 255;
		else if (hue <= 120)			hr = ((120 - hue) / 60.f) * 255;
		else if (hue <= 240)			hr = 0;
		else							hr = ((hue - 240) / 60.f) * 255;

		if (hue <= 60)					hb = ((hue) / 60.f) * 255;
		else if (hue <= 180)			hb = 255;
		else if (hue <= 240)			hb = ((240 - hue) / 60.f) * 255;
		else							hb = 0;

		if (hue <= 120)					hg = 0;
		else if (hue <= 180)			hg = ((hue - 120) / 60.f) * 255;
		else if (hue <= 300)			hg = 255;
		else							hg = ((360 - hue) / 60.f) * 255;

		r = hr;
		g = hg;
		b = hb;

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
	} else return;
	phue = hue;
	psat = sat;
	pval = val;
	pr = r;
	pg = g;
	pb = b;

	std::stringstream ss;
	ss << "0x" << std::hex << std::setfill('0') << std::setw(6) << std::uppercase <<
		(r << 16) + (g << 8) + (b << 0);
	setTooltip(ss.str());
}
void ColorOverlay::render() {
	Shader::pushMatrixModel();
		Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0));

		GBuffer::pushScissor(Rect(0, 0, m_size.x, m_size.y));

		GBuffer::setTexture(m_background1->getTexId());
		GBuffer::setColor(Color(1.f, 1.f, 1.f, 1.f));
		GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, 0);
		GBuffer::setUV(1, 0); GBuffer::addVertexQuad(m_size.x, 0);
		GBuffer::setUV(1, 1); GBuffer::addVertexQuad(m_size.x, m_size.y);
		GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, m_size.y);

		GBuffer::setTexture(m_background3->getTexId());
		GBuffer::setColor(Color(hr / 255.f, hg / 255.f, hb / 255.f));
		GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, 0);
		GBuffer::setUV(1, 0); GBuffer::addVertexQuad(m_size.x, 0);
		GBuffer::setUV(1, 1); GBuffer::addVertexQuad(m_size.x, m_size.y);
		GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, m_size.y);

		GBuffer::setTexture(m_background2->getTexId());
		GBuffer::setColor(Color(1.f, 1.f, 1.f, 1.f));
		GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, 0);
		GBuffer::setUV(1, 0); GBuffer::addVertexQuad(m_size.x, 0);
		GBuffer::setUV(1, 1); GBuffer::addVertexQuad(m_size.x, m_size.y);
		GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, m_size.y);;

		Shader::pushMatrixModel();
			Shader::translate(glm::vec3(sat / 100.f * m_size.x, m_size.y - val / 100.f * m_size.y, 0));

			GBuffer::setTexture(m_selectTexFill->getTexId());
			GBuffer::setColor(color);
			GBuffer::setUV(0, 0);	GBuffer::addVertexQuad(m_selectTexFill->getSize().x / -2, m_selectTexFill->getSize().y / -2);
			GBuffer::setUV(1, 0);	GBuffer::addVertexQuad(m_selectTexFill->getSize().x / 2, m_selectTexFill->getSize().y / -2);
			GBuffer::setUV(1, 1);	GBuffer::addVertexQuad(m_selectTexFill->getSize().x / 2, m_selectTexFill->getSize().y / 2);
			GBuffer::setUV(0, 1);	GBuffer::addVertexQuad(m_selectTexFill->getSize().x / -2, m_selectTexFill->getSize().y / 2);

			GBuffer::setTexture(m_selectTexOut->getTexId());
			if (color.r > 0.5f && color.g > 0.5f && color.b > 0.5f) {
				GBuffer::setColor(Color(0.f, 0.f, 0.f));
			} else {
				GBuffer::setColor(Color(1.f, 1.f, 1.f));
			}
			GBuffer::setUV(0, 0);	GBuffer::addVertexQuad(m_selectTexOut->getSize().x / -2, m_selectTexOut->getSize().y / -2);
			GBuffer::setUV(1, 0);	GBuffer::addVertexQuad(m_selectTexOut->getSize().x / 2, m_selectTexOut->getSize().y / -2);
			GBuffer::setUV(1, 1);	GBuffer::addVertexQuad(m_selectTexOut->getSize().x / 2, m_selectTexOut->getSize().y / 2);
			GBuffer::setUV(0, 1);	GBuffer::addVertexQuad(m_selectTexOut->getSize().x / -2, m_selectTexOut->getSize().y / 2);
		Shader::popMatrixModel();

		GBuffer::popScissor();

		GBuffer::setTexture(m_hueBar.hueTex->getTexId());
		GBuffer::setColor(Color(1.f, 1.f, 1.f));
		GBuffer::setUV(1, 0);	GBuffer::addVertexQuad(m_hueBar.x, m_hueBar.y);
		GBuffer::setUV(1, 1);	GBuffer::addVertexQuad(m_hueBar.x + m_hueBar.w, m_hueBar.y);
		GBuffer::setUV(0, 1);	GBuffer::addVertexQuad(m_hueBar.x + m_hueBar.w, m_hueBar.y + m_hueBar.h);
		GBuffer::setUV(0, 0);	GBuffer::addVertexQuad(m_hueBar.x, m_hueBar.y + m_hueBar.h);

		GBuffer::renderTexture(m_hueBar.pickTex,
			Vector2<Sint32>(m_hueBar.x - m_hueBar.pickTex->getSize().x, m_hueBar.y + m_hueBar.h * (hue / 360.f) - m_hueBar.pickTex->getSize().y / 2),
			m_hueBar.pickTex->getSize());

		Shader::pushMatrixModel();
			Shader::translate(glm::vec3(m_savedColors.offset.x, m_savedColors.offset.y, 0));
			GBuffer::setTexture(0);
		
			for (Sint32 x = 0; x < m_savedColors.columns; x++) {
				for (Sint32 y = 0; y < m_savedColors.rows; y++) {
					Color c = m_savedColors.colors[x + y * m_savedColors.columns]->getColor();
					GBuffer::setColor(c);
					GBuffer::addQuadFilled(Vector2<Sint32>(x * m_savedColors.colorSize.x, y * m_savedColors.colorSize.y),
						Vector2<Sint32>(m_savedColors.colorSize.x, m_savedColors.colorSize.y));
					if (x + y * m_savedColors.columns == m_savedColors.hovered) {
						GBuffer::renderTexture(m_savedColors.highlightTex,
							Vector2<Sint32>(x * m_savedColors.colorSize.x, y * m_savedColors.colorSize.y),
							m_savedColors.highlightTex->getSize());
						GBuffer::setTexture(0);
					}
				}
			}

		Shader::popMatrixModel();
	Shader::popMatrixModel();
}
