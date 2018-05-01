#include "engine\gfx\gui\list\Tileset.h"

CTileSet::CTileSet(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_tileSize, Texture* p_tileSheet, Theme p_colorTheme)
	: Component(p_compName, p_title, p_pos, p_size, p_colorTheme)
{
	m_tileSize = p_tileSize;
	m_tileSheet = p_tileSheet;
	m_transparentTex = MTexture::getTexture("gui\\Transparent.png");
	m_selectTex = MTexture::getTexture("gui\\TileSelect.png");
	m_tileCount = m_tileSheet->getSize() / m_tileSize;
	m_priority = -1;
}
void CTileSet::setTileSheet(Texture* p_tileSheet)
{
	m_tileSheet = p_tileSheet;
	m_tileCount = m_tileSheet->getSize() / m_tileSize;
}
Vector2<Sint32> CTileSet::getRealPosition()
{
	return Vector2<Sint32>(m_pos - Vector2<Sint32>(0, 20));
}
Vector2<Sint32> CTileSet::getRealSize()
{
	return Vector2<Sint32>(m_size + Vector2<Sint32>(0, 20));
}
void CTileSet::input(Sint8& p_interactFlags)
{
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	Vector2<Sint32> _size;
	if(m_selected == 0)
		_size = m_size;
	else
		_size = Vector2<Sint32>(std::fmaxf(m_size.x, m_tileSheet->getSize().x), std::fmaxf(m_size.y, m_tileSheet->getSize().y));
	if(_mousePos.x >= m_pos.x - (1) && _mousePos.x <= m_pos.x + _size.x + (1) &&
		_mousePos.y >= m_pos.y - 20 - (1) && _mousePos.y <= m_pos.y + _size.y + (1))
		m_hover = true;
	else
		m_hover = false;
	if((((p_interactFlags & 1) == 0) || m_dragging) && 
		_mousePos.x >= m_pos.x && _mousePos.x <= m_pos.x + _size.x &&
		_mousePos.y >= m_pos.y && _mousePos.y <= m_pos.y + _size.y)
	{
		if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if(Sint16(GLfloat(_mousePos.x - m_pos.x + (GLfloat(m_scroll.x) / m_tileSize) * m_tileSize) < m_tileSheet->getSize().x) &&
				Sint16(GLfloat(_mousePos.y - m_pos.y + (GLfloat(m_scroll.y) / m_tileSize) * m_tileSize) < m_tileSheet->getSize().y))
				m_selectedTile = {Sint16(GLfloat(_mousePos.x - m_pos.x + m_scroll.x) / m_tileSize),
				Sint16(GLfloat(_mousePos.y - m_pos.y + m_scroll.y) / m_tileSize)};
			m_dragging = false;
			if((p_interactFlags & 1) == 0)
				p_interactFlags += 1;

		}
		else if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) || (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) && m_dragging)
		{
			m_dragging = true;
			if((p_interactFlags & 1) == 0)
				p_interactFlags += 1;
		}
	}

	if((p_interactFlags & 2) == 0)
	{
		if(GKey::keyPressed(GLFW_KEY_LEFT) || GKey::keyPressed(GLFW_KEY_A))
		{
			m_selectedTile.x--;
			if(m_selectedTile.x < 0)
				m_selectedTile.x = 0;
			if(m_selectedTile.x * m_tileSize < m_scroll.x)
				m_scroll.x = m_selectedTile.x * m_tileSize;
			p_interactFlags += 2;
		}
		if(GKey::keyPressed(GLFW_KEY_RIGHT) || GKey::keyPressed(GLFW_KEY_D))
		{
			m_selectedTile.x++;
			if(m_selectedTile.x > (m_tileSheet->getSize().x / m_tileSize) - 1)
				m_selectedTile.x = (m_tileSheet->getSize().x / m_tileSize) - 1;
			if(m_selectedTile.x * m_tileSize - m_size.x + m_tileSize > m_scroll.x)
				m_scroll.x = m_selectedTile.x * m_tileSize - m_size.x + m_tileSize;
			p_interactFlags += 2;
		}
		if(GKey::keyPressed(GLFW_KEY_UP) || GKey::keyPressed(GLFW_KEY_W))
		{
			m_selectedTile.y--;
			if(m_selectedTile.y < 0)
				m_selectedTile.y = 0;
			if(m_selectedTile.y * m_tileSize < m_scroll.y)
				m_scroll.y = m_selectedTile.y * m_tileSize;
			p_interactFlags += 2;
		}
		if(GKey::keyPressed(GLFW_KEY_DOWN) || GKey::keyPressed(GLFW_KEY_S))
		{
			m_selectedTile.y++;
			if(m_selectedTile.y > (m_tileSheet->getSize().y / m_tileSize) - 1)
				m_selectedTile.y = (m_tileSheet->getSize().y / m_tileSize) - 1;
			if(m_selectedTile.y * m_tileSize - m_size.y + m_tileSize > m_scroll.y)
				m_scroll.y = m_selectedTile.y * m_tileSize - m_size.y + m_tileSize;
			p_interactFlags += 2;
		}
	}

	if(m_dragging)
	{
		if(!GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT) || GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
			m_dragging = false;
		else
		{
			if(m_tileSheet->getSize().x > _size.x)
				m_scroll.x -= (_mousePos.x - m_mouseBuffer.x);
			if(m_tileSheet->getSize().y > _size.y)
				m_scroll.y -= (_mousePos.y - m_mouseBuffer.y);
			if(m_scroll.x <= 0)
				m_scroll.x = 0;
			else if(m_scroll.x > m_tileSheet->getSize().x - _size.x - 1)
				m_scroll.x = m_tileSheet->getSize().x - _size.x - 1;
			if(m_scroll.y <= 0)
				m_scroll.y = 0;
			else if(m_scroll.y > m_tileSheet->getSize().y - _size.y - 1)
				m_scroll.y = m_tileSheet->getSize().y - _size.y - 1;
			m_mouseBuffer = _mousePos;
			if((p_interactFlags & 1) == 0)
				p_interactFlags += 1;
		}
	}

	m_mouseBuffer = _mousePos;
}
void CTileSet::update(GLfloat p_deltaUpdate)
{

}
void CTileSet::render()
{
	Vector2<Sint32> _size;
	if(m_selected == 0)
		_size = m_size;
	else
		_size = Vector2<Sint32>(std::fmaxf(m_size.x, m_tileSheet->getSize().x), std::fmaxf(m_size.y, m_tileSheet->getSize().y));
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		if(m_selected == 0)
			MScissor::getInstance().push(Rect(-GLfloat(1), -GLfloat(1 + 20), GLfloat(_size.x + 1 * 2), GLfloat(_size.y + 1 * 2 + 20)));
		else
			MScissor::getInstance().toggle();

		m_colorTheme.m_border.useColor();
		glBegin(GL_QUADS);
		{
			glVertex2f(-GLfloat(1), -GLfloat(1) - 20);
			glVertex2f(GLfloat(_size.x + 1), -GLfloat(1) - 20);
			glVertex2f(GLfloat(_size.x + 1), GLfloat(_size.y + 1));
			glVertex2f(-GLfloat(1), GLfloat(_size.y + 1));
		}
		glEnd();

		m_colorTheme.m_primary.useColor();
		glBegin(GL_QUADS);
		{
			glVertex2f(-GLfloat(1), GLfloat(_size.y + (1)));
			glVertex2f(GLfloat(_size.x + (1)), GLfloat(_size.y + (1)));
			glVertex2f(GLfloat(_size.x + (1)), -20 - GLfloat(1));
			glVertex2f(-GLfloat(1), -20 - GLfloat(1));
		}
		glEnd();

		m_colorTheme.m_text.useColor();
		Font::setAlignment(ALIGN_CENTER);
		Font::print(m_title, _size.x / 2, -12);

		glColor3f(1, 1, 1);

		if(m_selected == 0)
			MScissor::getInstance().pop();
		else
			MScissor::getInstance().toggle();

		MScissor::getInstance().push(Rect(0, 0, GLfloat(_size.x), GLfloat(_size.y)), m_selected != 0);

		m_transparentTex->bind();
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(0, 0);
			glTexCoord2f(GLfloat(ceil(GLfloat(_size.x) / m_tileSize)), 0);
			glVertex2f(GLfloat(_size.x), 0);
			glTexCoord2f(GLfloat(ceil(GLfloat(_size.x) / m_tileSize)), GLfloat(ceil(GLfloat(_size.y) / m_tileSize)));
			glVertex2f(GLfloat(_size.x), GLfloat(_size.y));
			glTexCoord2f(0, GLfloat(ceil(GLfloat(_size.y) / m_tileSize)));
			glVertex2f(0, GLfloat(_size.y));
		}
		glEnd();

		Vector2<GLfloat> _scroll = m_scroll;

		glPushMatrix();
		{
			glTranslatef(-GLfloat(m_scroll.x), -GLfloat(m_scroll.y), 0);

			m_tileSheet->bind();
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex2f(0, GLfloat(m_tileSheet->getSize().y));
				glTexCoord2f(1, 0);
				glVertex2f(GLfloat(m_tileSheet->getSize().x), GLfloat(m_tileSheet->getSize().y));
				glTexCoord2f(1, 1);
				glVertex2f(GLfloat(m_tileSheet->getSize().x), 0);
				glTexCoord2f(0, 1);
				glVertex2f(0, 0);
			}
			glEnd();
		}
		glPopMatrix();

		glTranslatef(-GLfloat(m_scroll.x % m_tileSize), -GLfloat(m_scroll.y % m_tileSize), 0);
		m_selectTex->bind();
		if(m_selectedTile.x >= floor(m_scroll.x / m_tileSize) && m_selectedTile.x <= ceil(m_scroll.x / m_tileSize) + ceil(GLfloat(_size.x) / m_tileSize)
			&& m_selectedTile.y >= floor(m_scroll.y / m_tileSize) && m_selectedTile.y <= ceil(m_scroll.y / m_tileSize) + ceil(GLfloat(_size.y) / m_tileSize))
		{
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex2f(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize);
				glTexCoord2f(1, 0);
				glVertex2f(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize + m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize);
				glTexCoord2f(1, 1);
				glVertex2f(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize + m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize + m_tileSize);
				glTexCoord2f(0, 1);
				glVertex2f(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize + m_tileSize);
			}
			glEnd();
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		glTranslatef(GLfloat(m_scroll.x % m_tileSize), GLfloat(m_scroll.y % m_tileSize), 0);

		if(m_hover || m_dragging)
		{
			m_colorTheme.m_border.useColor();
			glBegin(GL_QUADS);
			{
				glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 4, GLfloat(_size.y - 12));
				glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 60, GLfloat(_size.y - 12));
				glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 60, GLfloat(_size.y - 4));
				glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 4, GLfloat(_size.y - 4));

				glVertex2f(GLfloat(_size.x - 12), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 4);
				glVertex2f(GLfloat(_size.x - 4), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 4);
				glVertex2f(GLfloat(_size.x - 4), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 60);
				glVertex2f(GLfloat(_size.x - 12), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 60);
			}
			glEnd();
			m_colorTheme.m_primary.useColor();
			glBegin(GL_QUADS);
			{
				glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 5, GLfloat(_size.y - 11));
				glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 59, GLfloat(_size.y - 11));
				glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 59, GLfloat(_size.y - 5));
				glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet->getSize().x - _size.x + m_tileSize - 1)) * (_size.x - 80) + 5, GLfloat(_size.y - 5));

				glVertex2f(GLfloat(_size.x - 11), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 5);
				glVertex2f(GLfloat(_size.x - 5), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 5);
				glVertex2f(GLfloat(_size.x - 5), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 59);
				glVertex2f(GLfloat(_size.x - 11), (GLfloat(m_scroll.y) / (m_tileSheet->getSize().y - _size.y + m_tileSize - 1)) * (_size.y - 80) + 59);
			}
			glEnd();
		}

		MScissor::getInstance().pop();
	}
	glPopMatrix();
}

void CTileSet::setSelectedTile(Uint16 p_index) {
	if(m_tileSheet->getGlId() > 0 && m_tileCount.x > 0) {
		m_selectedTile.x = p_index % m_tileCount.x;
		m_selectedTile.y = Sint16(floor(GLfloat(p_index) / m_tileCount.x));
	}
	else
		m_selectedTile = {};
}
Uint16 CTileSet::getSelectedTile() {
	return Uint16(m_selectedTile.x + m_selectedTile.y * m_tileCount.x);
}

Sint8 CTileSet::isSelected() {
	return m_selected;
}
