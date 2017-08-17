#include "engine\gfx\gui\list\List.h"

CList::CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight, Sint16 *p_selectedItem, std::vector<std::string> *p_itemList)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_size = p_size - Vector2<Sint32>(0, 1);
	m_itemHeight = p_itemHeight;
	m_colorTheme = m_colorThemes[Theme::PRIMARY];
	m_selectedItem = p_selectedItem;
	m_itemList = p_itemList;

	m_scroll = 0;
	m_maxVisible = ceil(GLfloat(m_size.y) / p_itemHeight);
}

Component* CList::addItem(std::string p_itemName)
{
	m_itemList->push_back(p_itemName);
	m_maxScroll = Sint16((m_itemList->size() - m_maxVisible) * m_itemHeight);
	*m_selectedItem = Sint8(m_itemList->size());
	return this;
}
void CList::removeItem(Uint16 p_index)
{
	if(p_index < m_itemList->size())
	{
		m_itemList->erase(m_itemList->begin() + p_index);
		if(*m_selectedItem > Sint8(m_itemList->size())) *m_selectedItem--;
		m_maxScroll = Sint16((m_itemList->size() - m_maxVisible) * m_itemHeight);
	}
}
void CList::setItem(Uint16 p_index, std::string p_itemName)
{
	if(p_index < m_itemList->size())
		m_itemList->at(p_index) = p_itemName;
}
Uint16 CList::getItemCount()
{
	return Uint16(m_itemList->size());
}
std::string CList::getItem(Uint16 p_index)
{
	return m_itemList->at(p_index);
}

void CList::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	if(((p_interactFlags & 1) == 0) &&
		p_mousePos.x >= 0 && p_mousePos.x <= m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y <= m_size.y)
		m_hover = true;
	else
		m_hover = false;

	if(((p_interactFlags & 1) == 0) &&
		p_mousePos.x >= 0 && p_mousePos.x <= m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y <= m_size.y)
	{
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS)
		{
			if(Sint32((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight) <= Sint32(m_itemList->size()))
			{
				if(Uint16((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight) != *m_selectedItem)
				{
					*m_selectedItem = Uint16((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight);
				}
			}

			m_dragging = false;
		}
		else if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS || (p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_DOWN) && m_dragging)
			m_dragging = true;
		p_interactFlags += 1;
	}

	if(m_dragging)
	{
		if(p_mouseStates[1] == 0 || p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS)
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
		m_scroll = m_scroll - GMouse::m_mouseScroll * 4;
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
		renderBack();
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);

		glPushMatrix();
		{
			m_colorTheme.m_text.useColor();
			Font::getInstance().setAlignment(ALIGN_CENTER);
			Font::getInstance().print(m_title, m_size.x / 2, -12);

			MScissor::getInstance().push(Rect(0, 0, GLfloat(m_size.x), GLfloat(m_size.y)));

			glTranslatef(0, -GLfloat(m_scroll % m_itemHeight), 0);

			glBindTexture(GL_TEXTURE_2D, 0);

			glBegin(GL_QUADS);
			{
				for(Uint16 y = 0; y < m_maxVisible; y++)
				{
					if(m_scroll / m_itemHeight + y >= m_itemList->size()) continue;

					if(m_scroll / m_itemHeight + y == *m_selectedItem)
						m_colorTheme.m_select.useColor(1);
					else
						m_colorTheme.m_border.useColor(1);

					glVertex2f(0, GLfloat(y * m_itemHeight));
					glVertex2f(GLfloat(m_size.x), GLfloat(y * m_itemHeight));
					glVertex2f(GLfloat(m_size.x), GLfloat((y + 1) * m_itemHeight));
					glVertex2f(0, GLfloat((y + 1) * m_itemHeight));

					glColor3f(0, 0, 0);
					glVertex2f(GLfloat(m_size.x), GLfloat((y + 1) * m_itemHeight) - 1);
					glVertex2f(0, GLfloat((y + 1) * m_itemHeight) - 1);
					glVertex2f(0, GLfloat((y + 1) * m_itemHeight));
					glVertex2f(GLfloat(m_size.x), GLfloat((y + 1) * m_itemHeight));
				}
			}
			glEnd();

			m_colorTheme.m_text.useColor();
			Font::getInstance().setAlignment(ALIGN_LEFT);
			for(Uint16 i = 0; i < m_maxVisible; i++)
			{
				if(i + round(m_scroll / m_itemHeight) < m_itemList->size())
				{
					if(Sint32(m_itemList->at(i + m_scroll / m_itemHeight).length()) > m_size.x / 16 - 2)
						Font::getInstance().print(m_itemList->at(i + m_scroll / m_itemHeight).substr(0, m_size.x / 16 - 5) + "...", 2, Sint32((i + 0.5f) * m_itemHeight));
					else
						Font::getInstance().print(m_itemList->at(i + m_scroll / m_itemHeight), 2, Sint32((i + 0.5f) * m_itemHeight));
				}
			}
		}
		glPopMatrix();

		if(m_selectedItem)
			m_colorTheme.m_select.useColor();
		else
		{
			if(m_hover)
				Color((m_colorTheme.m_select + m_colorTheme.m_primary) / 2).useColor();
			else
				m_colorTheme.m_primary.useColor();
		}
		if(m_maxScroll > 0)
		{
			GLfloat _mod = GLfloat(m_scroll) / m_maxScroll * (m_size.y - 64);
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
