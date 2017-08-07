#include "engine\gfx\gui\list\List.h"

CList::CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight, Texture p_baseTileSheet, Sint8 p_colorTheme)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_size = p_size;
	m_baseTileSheet = p_baseTileSheet;
	m_itemHeight = p_itemHeight;
	m_colorTheme = m_colorThemes[p_colorTheme];

	m_scroll = 0;
}

Component* CList::addItem(std::string p_item, Uint16 p_texId, Texture p_texture)
{
	if(p_texture.getId() == -1)
		p_texture = m_baseTileSheet;
	m_itemList.push_back(ListItem(p_item, p_texture, p_texId));
	m_maxScroll = Sint16((m_itemList.size() - m_size.y + 1) * m_itemHeight);
	return this;
}
void CList::removeItem(Uint16 p_index)
{
	m_itemList.erase(m_itemList.begin() + p_index);
	m_selectedItem--;
	m_maxScroll = Sint16((m_itemList.size() - m_size.y + 1) * m_itemHeight);
}
void CList::setItem(Uint16 p_index, std::string p_item)
{
	m_itemList[p_index].m_name = p_item;
}
void CList::setItemTexId(Uint16 p_index, Uint16 p_texId)
{
	m_itemList[p_index].m_texId = p_texId;
}
void CList::setItemTexture(Uint16 p_index, Texture p_texture)
{
	m_itemList[p_index].m_texture = p_texture;
}
Uint16 CList::getItemCount()
{
	return Uint16(m_itemList.size());
}
std::string CList::getItem(Uint16 p_index)
{
	return m_itemList[p_index].m_name;
}
Texture CList::getItemTexture(Uint16 p_index)
{
	return m_itemList[p_index].m_texture;
}
Uint16 CList::getItemTexId(Uint16 p_index)
{
	return m_itemList[p_index].m_texId;
}

void CList::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	if(((p_interactFlags & 1) == 0) &&
		p_mousePos.x >= 0 && p_mousePos.x <= m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y <= m_size.y * m_itemHeight)
		m_hover = true;
	else
		m_hover = false;

	m_update = 0;
	if(((p_interactFlags & 1) == 0) &&
		p_mousePos.x >= 0 && p_mousePos.x <= m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y <= m_size.y * m_itemHeight)
	{
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
		{
			if(Sint32((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight) <= Sint32(m_itemList.size()))
			{
				if(Uint16((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight) != m_selectedItem)
				{
					m_selectedItem = Uint16((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight);
					m_update = 1;
				}
			}
			if(Sint32((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight) == Sint32(m_itemList.size()))
			{
				addItem(std::string("Item " + Util::numToString(m_itemList.size(), 0)), 0);
				m_update = 3;
				if(Sint32(m_itemList.size()) >= m_size.y)
					m_scroll += m_itemHeight;
			}

			m_dragging = false;
		}
		else if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS || (p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN) && m_dragging)
			m_dragging = true;
		p_interactFlags += 1;
	}

	if(m_dragging)
	{
		if(p_mouseStates[1] == 0 || p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
			m_dragging = false;
		else
		{
			m_scroll = m_scroll - (p_mousePos.y - m_mouseBuffer.y);
			if((p_interactFlags & 1) == 0)
				p_interactFlags += 1;
		}
	}
	if((p_interactFlags & 4) == 0 && m_hover)
	{
		m_scroll = m_scroll - MouseStates::m_mouseScroll * 4;
		p_interactFlags += 4;
	}

	if(m_scroll > m_maxScroll)
		m_scroll = m_maxScroll;
	if(m_scroll < 0)
		m_scroll = 0;

	m_mouseBuffer = p_mousePos;
}
void CList::update(GLfloat p_deltaUpdate)
{

}
void CList::render()
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);

		glPushMatrix();
		{
			MScissor::getInstance().push(Rect(-5, -25, GLfloat(m_size.x) + 10, GLfloat(m_size.y * m_itemHeight + 30)));
			m_colorTheme.m_back.useColor();
			glBegin(GL_QUADS);
			{
				glVertex2f(-5, GLfloat(m_size.y * m_itemHeight + 5));
				glVertex2f(GLfloat(m_size.x + 5), GLfloat(m_size.y * m_itemHeight + 5));
				glVertex2f(GLfloat(m_size.x + 5), -25);
				glVertex2f(-5, -25);
			}
			glEnd();

			m_colorTheme.m_fore.useColor();
			glBegin(GL_QUADS);
			{
				glVertex2f(-4, GLfloat(m_size.y * m_itemHeight + 4));
				glVertex2f(GLfloat(m_size.x + 4), GLfloat(m_size.y * m_itemHeight + 4));
				glVertex2f(GLfloat(m_size.x + 4), -24);
				glVertex2f(-4, -24);
			}
			glEnd();

			m_colorTheme.m_text.useColor();
			Font::getInstance().setAlignment(ALIGN_CENTER);
			Font::getInstance().print(m_title, m_size.x / 2, -12);

			MScissor::getInstance().pop();
			MScissor::getInstance().push(Rect(0, 0, GLfloat(m_size.x), GLfloat(m_size.y * m_itemHeight)));

			glTranslatef(0, -GLfloat(m_scroll % m_itemHeight), 0);

			glBindTexture(GL_TEXTURE_2D, 0);

			glBegin(GL_QUADS);
			{
				for(Uint16 y = 0; y < Uint16(m_size.y) + 1; y++)
				{
					if(m_scroll / m_itemHeight + y == m_selectedItem)
						m_colorTheme.m_active.useColor(1.2f);
					else
					{
						if((y + (m_scroll / m_itemHeight)) % 2 == 0) m_colorTheme.m_active.useColor(1.1f);
						else m_colorTheme.m_active.useColor(1);
					}
					glVertex2f(0, GLfloat(y * m_itemHeight));
					glVertex2f(GLfloat(m_size.x), GLfloat(y * m_itemHeight));
					glVertex2f(GLfloat(m_size.x), GLfloat((y + 1) * m_itemHeight));
					glVertex2f(0, GLfloat((y + 1) * m_itemHeight));
				}
			}
			glEnd();

			glColor3f(1, 1, 1);
			for(Uint16 i = 0; i < Uint16(m_size.y) + 1; i++)
			{
				if(i + round(m_scroll / m_itemHeight) < m_itemList.size())
				{
					Texture _tex = m_itemList[Uint16(i + round(m_scroll / m_itemHeight))].m_texture;
					if(_tex.getId() != -1)
					{
						glBindTexture(GL_TEXTURE_2D, _tex.getId());
						Vector2<Sint32> _tileSizes = {_tex.getSize().x / m_itemHeight, _tex.getSize().y / m_itemHeight};
						glBegin(GL_QUADS);
						{
							glTexCoord2f(GLfloat(m_itemList[i + m_scroll / m_itemHeight].m_texId % _tileSizes.x) / _tileSizes.x, 1.f - (floor(GLfloat(m_itemList[i + m_scroll / m_itemHeight].m_texId) / _tileSizes.y) / _tileSizes.y));
							glVertex2f(0, GLfloat(i * m_itemHeight));
							glTexCoord2f(GLfloat(m_itemList[i + m_scroll / m_itemHeight].m_texId % _tileSizes.x) / _tileSizes.x + GLfloat(m_itemHeight) / _tex.getSize().x, 1.f - (floor(GLfloat(m_itemList[i + m_scroll / m_itemHeight].m_texId) / _tileSizes.y) / _tileSizes.y));
							glVertex2f(GLfloat(m_itemHeight), GLfloat(i * m_itemHeight));
							glTexCoord2f(GLfloat(m_itemList[i + m_scroll / m_itemHeight].m_texId % _tileSizes.x) / _tileSizes.x + GLfloat(m_itemHeight) / _tex.getSize().x, 1.f - (floor(GLfloat(m_itemList[i + m_scroll / m_itemHeight].m_texId) / _tileSizes.y) / _tileSizes.y + GLfloat(m_itemHeight) / _tex.getSize().y));
							glVertex2f(GLfloat(m_itemHeight), GLfloat((i + 1) * m_itemHeight));
							glTexCoord2f(GLfloat(m_itemList[i + m_scroll / m_itemHeight].m_texId % _tileSizes.x) / _tileSizes.x, 1.f - (floor(GLfloat(m_itemList[i + m_scroll / m_itemHeight].m_texId) / _tileSizes.y) / _tileSizes.y + GLfloat(m_itemHeight) / _tex.getSize().y));
							glVertex2f(0, GLfloat((i + 1) * m_itemHeight));
						}
						glEnd();
					}
				}
			}

			m_colorTheme.m_text.useColor();
			Font::getInstance().setAlignment(ALIGN_LEFT);
			for(Uint16 i = 0; i < Uint16(m_size.y) + 1; i++)
			{
				if(i + round(m_scroll / m_itemHeight) <= m_itemList.size())
				{
					if(i + round(m_scroll / m_itemHeight) == m_itemList.size())
						Font::getInstance().print("Add Item", 34, Sint32((i + 0.5f) * m_itemHeight));
					else
					{
						if(Sint32(m_itemList[i + m_scroll / m_itemHeight].m_name.length()) > m_size.x / 16 - 2)
							Font::getInstance().print(m_itemList[i + m_scroll / m_itemHeight].m_name.substr(0, m_size.x / 16 - 5) + "...", 34, Sint32((i + 0.5f) * m_itemHeight));
						else
							Font::getInstance().print(m_itemList[i + m_scroll / m_itemHeight].m_name, 34, Sint32((i + 0.5f) * m_itemHeight));
					}
				}
			}
		}
		glPopMatrix();

		if(m_selected)
			m_colorTheme.m_active.useColor();
		else
		{
			if(m_hover)
				Color((m_colorTheme.m_active + m_colorTheme.m_fore) / 2).useColor();
			else
				m_colorTheme.m_fore.useColor();
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		if(m_maxScroll > 0)
		{
			GLfloat _mod = GLfloat(m_scroll) / m_maxScroll * (m_size.y * m_itemHeight - 64);
			glBegin(GL_QUADS);
			{
				glVertex2f(GLfloat(m_size.x - 12), GLfloat(_mod + 4));
				glVertex2f(GLfloat(m_size.x - 4), GLfloat(_mod + 4));
				glVertex2f(GLfloat(m_size.x - 4), GLfloat(_mod + 60));
				glVertex2f(GLfloat(m_size.x - 12), GLfloat(_mod + 60));
			}
			glEnd();
		}
		MScissor::getInstance().pop();
	}
	glPopMatrix();
}

Sint16 CList::getSelectedItem()
{
	return m_selectedItem;
}
Uint8 CList::isUpdated()
{
	return m_update;
}

Sint8 CList::isSelected()
{
	return m_selected;
}

void CList::clear()
{
	m_itemList.clear();
	m_selectedItem = 0;
}