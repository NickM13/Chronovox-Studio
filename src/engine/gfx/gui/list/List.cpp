#include "engine\gfx\gui\list\List.h"

CList::CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight)
	: Component(p_compName, p_title, p_pos, p_size, ACTION)
{
	p_size = p_size - Vector2<Sint32>(0, 1);
	m_itemHeight = p_itemHeight;
	m_scroll = m_maxScroll = 0;
	m_maxVisible = ceil(GLfloat(m_size.y) / p_itemHeight);
	m_hoveredItem = -1;
}

Component* CList::addItem(std::string p_itemName)
{
	m_itemList.push_back(ListItem(p_itemName, 0));
	m_maxScroll = max(0, Sint16((m_itemList.size() - m_maxVisible) * m_itemHeight));
	return this;
}
Component* CList::insertItem(Uint16 p_index, std::string p_itemName)
{
	m_itemList.insert(m_itemList.begin() + p_index, ListItem(p_itemName, 0));
	m_maxScroll = m_maxScroll = max(0, Sint16((m_itemList.size() - m_maxVisible) * m_itemHeight));
	return this;
}
void CList::removeItem(Uint16 p_index)
{
	if(m_itemList.size() > p_index)
		m_itemList.erase(m_itemList.begin() + p_index);
}
Uint16 CList::getItemCount() {
	return Uint16(m_itemList.size());
}
CList::ListItem &CList::getItem(Uint16 p_index) {
	return m_itemList.at(p_index);
}
void CList::clear() {
	m_itemList.clear();
}

void CList::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	if((p_interactFlags & EVENT_MOUSEOVER) &&
		p_mousePos.x >= 0 && p_mousePos.x <= m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y <= m_size.y) {
		m_hover = true;
	}
	else
		m_hover = false;

	if(m_hoveredItem >= 0) {
		m_itemList[m_hoveredItem].state = 0;
		m_hoveredItem = -1;
	}

	if((p_interactFlags & EVENT_MOUSEOVER) &&
		p_mousePos.x >= 0 && p_mousePos.x <= m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y <= m_size.y) {
		if(Sint32((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight) < Sint32(m_itemList.size())) {
			Uint16 _hoveredItem = Uint16((p_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight);
			if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS) {
				Sint8 _state = m_itemList[_hoveredItem].state;
				if(!p_keyStates[GLFW_KEY_LEFT_CONTROL]) {
					for(ListItem &item : m_itemList)
						if(item.state == 2) item.state = 0;
				}
				if(_state != 2)
					m_itemList[_hoveredItem].state = 2;
				else
					m_itemList[_hoveredItem].state = 0;
				m_dragging = false;
			}
			else if(m_itemList[_hoveredItem].state != 2) {
				m_hoveredItem = _hoveredItem;
				m_itemList[m_hoveredItem].state = 1;
			}
		}
		else if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS || (p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_DOWN) && m_dragging)
			m_dragging = true;
		p_interactFlags -= EVENT_MOUSEOVER;
	}

	if(m_dragging) {
		if(p_mouseStates[1] == 0 || p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS)
			m_dragging = false;
		else {
			m_scroll = m_scroll - (p_mousePos.y - m_mouseBuffer.y);
			if(p_interactFlags & EVENT_MOUSEOVER)
				p_interactFlags -= EVENT_MOUSEOVER;
		}
	}
	if((p_interactFlags & EVENT_MOUSESCROLL) && m_hover)
	{
		m_scroll = m_scroll - GMouse::m_mouseScroll * 4;
		p_interactFlags -= EVENT_MOUSESCROLL;
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
			Font::setAlignment(ALIGN_CENTER);
			Font::print(m_title, m_size.x / 2, -12);

			MScissor::getInstance().push(Rect(0, 0, GLfloat(m_size.x), GLfloat(m_size.y)));

			glTranslatef(0, -GLfloat(m_scroll % m_itemHeight), 0);

			glBindTexture(GL_TEXTURE_2D, 0);

			glBegin(GL_QUADS);
			{
				for(Uint16 y = 0; y <= m_maxVisible; y++)
				{
					if(m_scroll / m_itemHeight + y >= Uint16(m_itemList.size())) continue;

					switch(m_itemList.at(m_scroll / m_itemHeight + y).state) {
					case 0: m_colorTheme.m_primary.useColor(); break;
					case 1: m_colorTheme.m_hover.useColor(); break;
					case 2: m_colorTheme.m_select.useColor(); break;
					default: m_colorTheme.m_primary.useColor(); break;
					}

					glVertex2f(0, GLfloat(y * m_itemHeight));
					glVertex2f(GLfloat(m_size.x), GLfloat(y * m_itemHeight));
					glVertex2f(GLfloat(m_size.x), GLfloat((y + 1) * m_itemHeight));
					glVertex2f(0, GLfloat((y + 1) * m_itemHeight));

					m_colorTheme.m_border.useColor();
					glVertex2f(GLfloat(m_size.x), GLfloat((y + 1) * m_itemHeight) - 1);
					glVertex2f(0, GLfloat((y + 1) * m_itemHeight) - 1);
					glVertex2f(0, GLfloat((y + 1) * m_itemHeight));
					glVertex2f(GLfloat(m_size.x), GLfloat((y + 1) * m_itemHeight));
				}
			}
			glEnd();

			m_colorTheme.m_text.useColor();
			Font::setAlignment(ALIGN_LEFT);
			std::string _name;
			for(Uint16 i = 0; i <= m_maxVisible; i++)
			{
				if(i + round(m_scroll / m_itemHeight) < m_itemList.size())
				{
					_name = m_itemList[i + m_scroll / m_itemHeight].name;
					if(Sint32(_name.length()) > m_size.x / 16 - 2) // If the item's name doesnt fit in box
						Font::print(_name.substr(0, m_size.x / 16 - 5) + "...", 8, Sint32((i + 0.5f) * m_itemHeight) - 1);
					else
						Font::print(_name, 8, Sint32((i + 0.5f) * m_itemHeight) - 1);
				}
			}
		}
		glPopMatrix();
		glPushMatrix();
		{
			GLfloat _scrollHeight = powf(m_size.y, 2) / (max(m_maxVisible, m_itemList.size()) * m_itemHeight);
			glTranslatef(m_size.x - 12, m_maxScroll > 0 ? ((GLfloat) m_scroll / m_maxScroll) * (m_size.y - _scrollHeight) : 0, 0);
			glBegin(GL_QUADS);
			{
				m_colorTheme.m_border.useColor();
				glVertex2f(2, 2);
				glVertex2f(10, 2);
				glVertex2f(10, _scrollHeight - 2);
				glVertex2f(2, _scrollHeight - 2);

				m_colorTheme.m_hover.useColor();
				glVertex2f(3, 3);
				glVertex2f(9, 3);
				glVertex2f(9, _scrollHeight - 3);
				glVertex2f(3, _scrollHeight - 3);
			}
			glEnd();
		}
		glPopMatrix();
		MScissor::getInstance().pop();
	}
	glPopMatrix();
}
