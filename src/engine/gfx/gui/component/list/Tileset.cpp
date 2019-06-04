#include "engine\gfx\gui\component\list\Tileset.h"

CTileSet::CTileSet(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_tileSize, Texture* p_tileSheet)
	: Component(p_compName, p_title, p_pos, p_size) {
	m_tileSize = p_tileSize;
	m_tileSheet = p_tileSheet;
	m_transparentTex = MTexture::getTexture("gui\\Transparent.png");
	m_selectTex = MTexture::getTexture("gui\\TileSelect.png");
	m_tileCount = m_tileSheet->getSize() / m_tileSize;
	m_priority = -1;
}
void CTileSet::setTileSheet(Texture* p_tileSheet) {
	m_tileSheet = p_tileSheet;
	m_tileCount = m_tileSheet->getSize() / m_tileSize;
}
Vector2<Sint32> CTileSet::getRealPosition() {
	return Vector2<Sint32>(m_pos - Vector2<Sint32>(0, 20));
}
Vector2<Sint32> CTileSet::getRealSize() {
	return Vector2<Sint32>(m_size + Vector2<Sint32>(0, 20));
}
void CTileSet::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	Vector2<Sint32> _size;
	if (m_selected == 0) {
		_size = m_size;
	}
	else {
		_size = Vector2<Sint32>(std::fmaxf(m_size.x, m_tileSheet->getSize().x), std::fmaxf(m_size.y, m_tileSheet->getSize().y));
	}
	if (_mousePos.x >= m_pos.x - (1) && _mousePos.x <= m_pos.x + _size.x + (1) &&
		_mousePos.y >= m_pos.y - 20 - (1) && _mousePos.y <= m_pos.y + _size.y + (1)) {
		m_hover = true;
	}
	else {
		m_hover = false;
	}
	if ((((p_interactFlags & 1) == 0) || m_dragging) &&
		_mousePos.x >= m_pos.x && _mousePos.x <= m_pos.x + _size.x &&
		_mousePos.y >= m_pos.y && _mousePos.y <= m_pos.y + _size.y) {
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			if (Sint16(GLfloat(_mousePos.x - m_pos.x + (GLfloat(m_scroll.x) / m_tileSize) * m_tileSize) < m_tileSheet->getSize().x) &&
				Sint16(GLfloat(_mousePos.y - m_pos.y + (GLfloat(m_scroll.y) / m_tileSize) * m_tileSize) < m_tileSheet->getSize().y))
				m_selectedTile = { Sint16(GLfloat(_mousePos.x - m_pos.x + m_scroll.x) / m_tileSize),
				Sint16(GLfloat(_mousePos.y - m_pos.y + m_scroll.y) / m_tileSize) };
			m_dragging = false;
			if ((p_interactFlags & 1) == 0) {
				p_interactFlags += 1;
			}

		}
		else if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) || (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) && m_dragging) {
			m_dragging = true;
			if ((p_interactFlags & 1) == 0) {
				p_interactFlags += 1;
			}
		}
	}

	if ((p_interactFlags & 2) == 0) {
		if (GKey::keyPressed(GLFW_KEY_LEFT) || GKey::keyPressed(GLFW_KEY_A)) {
			m_selectedTile.x--;
			if (m_selectedTile.x < 0) {
				m_selectedTile.x = 0;
			}
			if (m_selectedTile.x * m_tileSize < m_scroll.x) {
				m_scroll.x = m_selectedTile.x * m_tileSize;
			}
			p_interactFlags += 2;
		}
		if (GKey::keyPressed(GLFW_KEY_RIGHT) || GKey::keyPressed(GLFW_KEY_D)) {
			m_selectedTile.x++;
			if (m_selectedTile.x > (m_tileSheet->getSize().x / m_tileSize) - 1) {
				m_selectedTile.x = (m_tileSheet->getSize().x / m_tileSize) - 1;
			}
			if (m_selectedTile.x * m_tileSize - m_size.x + m_tileSize > m_scroll.x) {
				m_scroll.x = m_selectedTile.x * m_tileSize - m_size.x + m_tileSize;
			}
			p_interactFlags += 2;
		}
		if (GKey::keyPressed(GLFW_KEY_UP) || GKey::keyPressed(GLFW_KEY_W)) {
			m_selectedTile.y--;
			if (m_selectedTile.y < 0) {
				m_selectedTile.y = 0;
			}
			if (m_selectedTile.y * m_tileSize < m_scroll.y) {
				m_scroll.y = m_selectedTile.y * m_tileSize;
			}
			p_interactFlags += 2;
		}
		if (GKey::keyPressed(GLFW_KEY_DOWN) || GKey::keyPressed(GLFW_KEY_S)) {
			m_selectedTile.y++;
			if (m_selectedTile.y > (m_tileSheet->getSize().y / m_tileSize) - 1) {
				m_selectedTile.y = (m_tileSheet->getSize().y / m_tileSize) - 1;
			}
			if (m_selectedTile.y * m_tileSize - m_size.y + m_tileSize > m_scroll.y) {
				m_scroll.y = m_selectedTile.y * m_tileSize - m_size.y + m_tileSize;
			}
			p_interactFlags += 2;
		}
	}

	if (m_dragging) {
		if (!GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT) || GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			m_dragging = false;
		}
		else {
			if (m_tileSheet->getSize().x > _size.x) {
				m_scroll.x -= (_mousePos.x - m_mouseBuffer.x);
			}
			if (m_tileSheet->getSize().y > _size.y) {
				m_scroll.y -= (_mousePos.y - m_mouseBuffer.y);
			}
			if (m_scroll.x <= 0) {
				m_scroll.x = 0;
			}
			else if (m_scroll.x > m_tileSheet->getSize().x - _size.x - 1) {
				m_scroll.x = m_tileSheet->getSize().x - _size.x - 1;
			}
			if (m_scroll.y <= 0) {
				m_scroll.y = 0;
			}
			else if (m_scroll.y > m_tileSheet->getSize().y - _size.y - 1) {
				m_scroll.y = m_tileSheet->getSize().y - _size.y - 1;
			}
			m_mouseBuffer = _mousePos;
			if ((p_interactFlags & 1) == 0) {
				p_interactFlags += 1;
			}
		}
	}

	m_mouseBuffer = _mousePos;
}
void CTileSet::update(GLfloat p_deltaUpdate) {

}
void CTileSet::render() {
	Vector2<Sint32> _size;
	if (m_selected == 0) {
		_size = m_size;
	}
	else {
		_size = Vector2<Sint32>(std::fmaxf(m_size.x, m_tileSheet->getSize().x), std::fmaxf(m_size.y, m_tileSheet->getSize().y));
	}
	Shader::pushMatrixModel();
	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	if (m_selected == 0) {
		GBuffer::pushScissor(Rect(-GLfloat(1), -GLfloat(1 + 20), GLfloat(_size.x + 1 * 2), GLfloat(_size.y + 1 * 2 + 20)));
	}
	else {
		GBuffer::setScissorActive(true);
	}

	GBuffer::setColor(getElementColor(getElementPos() + "BorderUnfocused"));
	GBuffer::addVertexQuad(-1, -1 - 20);
	GBuffer::addVertexQuad((_size.x + 1), -1 - 20);
	GBuffer::addVertexQuad((_size.x + 1), (_size.y + 1));
	GBuffer::addVertexQuad(-1, (_size.y + 1));

	GBuffer::setColor(getPrimaryColor());
	GBuffer::addVertexQuad(-1, (_size.y + 1));
	GBuffer::addVertexQuad((_size.x + 1), (_size.y + 1));
	GBuffer::addVertexQuad((_size.x + 1), -20 - 1);
	GBuffer::addVertexQuad(-1, -20 - 1);

	GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
	Font::setAlignment(ALIGN_CENTER);
	Font::print(m_title, _size.x / 2, -12);

	GBuffer::setColor(Color(1.f, 1.f, 1.f));

	if (m_selected == 0) {
		GBuffer::popScissor();
	}
	else {
		GBuffer::setScissorActive(false);
	}

	GBuffer::pushScissor(Rect(0, 0, GLfloat(_size.x), GLfloat(_size.y)));

	GBuffer::setTexture(m_transparentTex->getTexId());
	GBuffer::setUV(0, 0);
	GBuffer::addVertexQuad(0, 0);
	GBuffer::setUV(GLfloat(ceil(GLfloat(_size.x) / m_tileSize)), 0);
	GBuffer::addVertexQuad(_size.x, 0);
	GBuffer::setUV(GLfloat(ceil(GLfloat(_size.x) / m_tileSize)), GLfloat(ceil(GLfloat(_size.y) / m_tileSize)));
	GBuffer::addVertexQuad(_size.x, _size.y);
	GBuffer::setUV(0, GLfloat(ceil(GLfloat(_size.y) / m_tileSize)));
	GBuffer::addVertexQuad(0, _size.y);

	Vector2<GLfloat> _scroll = m_scroll;

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(-(GLfloat)m_scroll.x, -(GLfloat)m_scroll.y, 0.f));

	GBuffer::setTexture(m_tileSheet->getTexId());
	GBuffer::setUV(0, 0);
	GBuffer::addVertexQuad(0, GLfloat(m_tileSheet->getSize().y));
	GBuffer::setUV(1, 0);
	GBuffer::addVertexQuad(GLfloat(m_tileSheet->getSize().x), GLfloat(m_tileSheet->getSize().y));
	GBuffer::setUV(1, 1);
	GBuffer::addVertexQuad(GLfloat(m_tileSheet->getSize().x), 0);
	GBuffer::setUV(0, 1);
	GBuffer::addVertexQuad(0, 0);
	Shader::popMatrixModel();

	Shader::translate(glm::vec3(-(GLfloat)(m_scroll.x % m_tileSize), -(GLfloat)(m_scroll.y % m_tileSize), 0.f));
	GBuffer::setTexture(m_selectTex->getTexId());
	if (m_selectedTile.x >= floor(m_scroll.x / m_tileSize) && m_selectedTile.x <= ceil(m_scroll.x / m_tileSize) + ceil(GLfloat(_size.x) / m_tileSize)
		&& m_selectedTile.y >= floor(m_scroll.y / m_tileSize) && m_selectedTile.y <= ceil(m_scroll.y / m_tileSize) + ceil(GLfloat(_size.y) / m_tileSize)) {
		GBuffer::setUV(0, 0);
		GBuffer::addVertexQuad(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize);
		GBuffer::setUV(1, 0);
		GBuffer::addVertexQuad(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize + m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize);
		GBuffer::setUV(1, 1);
		GBuffer::addVertexQuad(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize + m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize + m_tileSize);
		GBuffer::setUV(0, 1);
		GBuffer::addVertexQuad(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize + m_tileSize);
	}

	GBuffer::setTexture(0);

	Shader::translate(glm::vec3((GLfloat)(m_scroll.x % m_tileSize), (GLfloat)(m_scroll.y % m_tileSize), 0.f));

	if (m_hover || m_dragging) {
		GBuffer::setColor(getElementColor(getElementPos() + "BorderUnfocused"));
		GBuffer::addVertexQuad((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 4, (_size.y - 12));
		GBuffer::addVertexQuad((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 60, (_size.y - 12));
		GBuffer::addVertexQuad((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 60, (_size.y - 4));
		GBuffer::addVertexQuad((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 4, (_size.y - 4));

		GBuffer::addVertexQuad((_size.x - 12), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 4);
		GBuffer::addVertexQuad((_size.x - 4), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 4);
		GBuffer::addVertexQuad((_size.x - 4), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 60);
		GBuffer::addVertexQuad((_size.x - 12), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 60);

		GBuffer::setColor(getPrimaryColor());
		GBuffer::addVertexQuad((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 5, (_size.y - 11));
		GBuffer::addVertexQuad((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 59, (_size.y - 11));
		GBuffer::addVertexQuad((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 59, (_size.y - 5));
		GBuffer::addVertexQuad((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 5, (_size.y - 5));

		GBuffer::addVertexQuad((_size.x - 11), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 5);
		GBuffer::addVertexQuad((_size.x - 5), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 5);
		GBuffer::addVertexQuad((_size.x - 5), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 59);
		GBuffer::addVertexQuad((_size.x - 11), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 59);
	}

	GBuffer::popScissor();
	Shader::popMatrixModel();
}

void CTileSet::setSelectedTile(Uint16 p_index) {
	if (m_tileSheet->getTexId() > 0 && m_tileCount.x > 0) {
		m_selectedTile.x = p_index % m_tileCount.x;
		m_selectedTile.y = Sint16(floor(GLfloat(p_index) / m_tileCount.x));
	}
	else {
		m_selectedTile = {};
	}
}
Uint16 CTileSet::getSelectedTile() {
	return Uint16(m_selectedTile.x + m_selectedTile.y * m_tileCount.x);
}

Sint8 CTileSet::isSelected() {
	return m_selected;
}
