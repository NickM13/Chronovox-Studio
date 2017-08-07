#include "engine\gfx\gui\list\TabBar.h"
#include "engine\utils\Utilities.h"

CTabBar::CTabBar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_colorTheme)
{
	m_compName = p_compName;
	m_pos = p_pos;
	m_size = p_size;
	m_colorTheme = m_colorThemes[p_colorTheme];
	m_scroll = 0;
	m_selected = 0;
	m_updated = 0;

	m_buttonLShift = new CButton("", "<", {0, 0}, {20, m_size.y}, true);
	m_buttonRShift = new CButton("", ">", {m_size.x - 20, 0}, {20, m_size.y}, true);

	m_maxScroll = 0;
}

void CTabBar::setSelected(Uint16 p_selected)
{
	m_selected = p_selected;
	calcMaxScroll();
}

Uint16 CTabBar::getTabCount()
{
	return Uint16(m_tabList.size());
}
void CTabBar::removeTab(Sint16 p_index)
{
	m_tabList.erase(m_tabList.begin() + p_index);
	if(m_selected >= Sint16(m_tabList.size()))
		m_selected--;
	calcMaxScroll();
}
void CTabBar::clear()
{
	m_tabList.clear();
	calcMaxScroll();
}

Sint16 CTabBar::getSelectedItem()
{
	return m_selected;
}

Component* CTabBar::addItem(std::string p_title)
{
	m_tabList.push_back(p_title);
	calcMaxScroll();
	return this;
}

void CTabBar::calcMaxScroll()
{
	m_maxScroll = -m_size.x + 24 * 2 + 20 + (m_tabList.size()) * 12;
	for(Uint16 i = 0; i < m_tabList.size(); i++)
		m_maxScroll += Font::getInstance().getMessageWidth(m_tabList[i]).x;
	m_maxScroll = max(0, m_maxScroll);
}

void CTabBar::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;

	m_updated = 0;
	m_hovered = -1;
	if(m_selected == m_tabList.size() + 1)
		m_selected = 0;

	m_buttonLShift->input(p_interactFlags, p_keyStates, p_mouseStates, p_mousePos);
	if(m_buttonLShift->isSelected() != 0 && (p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN))
		m_scroll -= 4;
	if(m_scroll <= 0)
		m_scroll = 0;

	m_buttonRShift->input(p_interactFlags, p_keyStates, p_mouseStates, p_mousePos);
	if(m_buttonRShift->isSelected() != 0 && (p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN) && m_scroll < m_maxScroll)
		m_scroll += 4;
	if(m_scroll >= m_maxScroll)
		m_scroll = m_maxScroll;

	if(p_mousePos.x >= 0 && p_mousePos.y >= 0 && p_mousePos.x <= m_size.x && p_mousePos.y <= m_size.y)
	{
		CButton* _button = new CButton("", "", {}, {}, true);

		Sint32 _offset = 24 - m_scroll;
		for(Sint32 i = 0; i < Sint32(m_tabList.size()); i++)
		{
			_button->setTitle(m_tabList[i]);
			_button->setPosition(Vector2<Sint32>(_offset, 0));
			_button->setSize(Vector2<Sint32>(Font::getInstance().getMessageWidth(m_tabList[i]).x + 8, m_size.y));
			_button->input(p_interactFlags, p_keyStates, p_mouseStates, p_mousePos);
			if(m_selected != i && _button->isSelected() != 0 && p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
			{
				m_selected = i;
				if((m_updated & 1) == 0)
					m_updated += 1;
			}
			if(_button->isHovered())
				m_hovered = i;

			_offset += _button->getSize().x + 4;
		}
		_button->setPosition(Vector2<Sint32>(_offset, 0));
		_button->setSize(Vector2<Sint32>(20, m_size.y));
		_button->setHover(m_hovered == (m_tabList.size() + 1));
		_button->input(p_interactFlags, p_keyStates, p_mouseStates, p_mousePos);
		if(_button->isSelected() != 0 && p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
		{
			m_selected = Sint16(m_tabList.size());
			addItem(std::string("Item ") + Util::numToString(m_tabList.size()));
			if((m_updated & 1) == 0)
				m_updated += 1;
			if((m_updated & 2) == 0)
				m_updated += 2;
			m_scroll = m_maxScroll;
		}
		if(_button->isHovered())
			m_hovered = Sint16(m_tabList.size() + 1);

		delete _button;
	}
}

void CTabBar::update(GLfloat p_deltaUpdate)
{

}

void CTabBar::render()
{
	Font::getInstance().setAlignment(ALIGN_CENTER);
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		Component::render();
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);

		glPushMatrix();
		{
			CButton* _button = new CButton("", "", {}, {}, true);

			_button->setPosition(Vector2<Sint32>(24 - m_scroll, 0));
			for(Sint32 i = 0; i < Sint32(m_tabList.size()); i++)
			{
				_button->setTitle(m_tabList[i]);
				_button->setSize(Vector2<Sint32>(Font::getInstance().getMessageWidth(m_tabList[i]).x + 8, m_size.y));
				_button->setState(m_selected == i);
				_button->setHover(m_hovered == i);
				_button->render();
				glTranslatef(GLfloat(_button->getSize().x + 4), 0, 0);
			}
			_button->setTitle("+");
			_button->setSize(Vector2<Sint32>(20, m_size.y));
			_button->setState(m_selected == (m_tabList.size() + 1));
			_button->setHover(m_hovered == (m_tabList.size() + 1));
			_button->render();
			glTranslatef(GLfloat(_button->getSize().x + 4), 0, 0);
			delete _button;
		}
		glPopMatrix();
		m_buttonLShift->render();
		m_buttonRShift->render();
	}
	glPopMatrix();
}
