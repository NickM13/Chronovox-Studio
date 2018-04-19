#include "engine\gfx\gui\list\DropDown.h"
#include "engine\gfx\font\Font.h"

CDropDown::CDropDown(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_width, Theme p_colorTheme)
	: Component(p_compName, p_title, p_pos, {p_width, 24}, p_colorTheme) {
	m_prevSelectedItem = m_selectedItem = 0;
	m_hoverItem = -1;
	setPriorityLayer(4);
	m_arrow = MTexture::getTexture("gui\\icon\\tool\\DropdownArrow.png");
}

void CDropDown::setList(std::vector<std::string> p_items) {
	bool different = false;
	if(p_items.size() == m_itemList.size()) {
		for(Uint16 i = 0; i < p_items.size(); i++)
			if(p_items[i] != m_itemList[i]) {
				different = true;
				break;
			}
	}
	else
		different = true;
	if(different) {
		m_prevSelectedItem = m_selectedItem = 0;
		m_itemList.clear();
		m_itemList = p_items;
		callPressFunction();
	}
}

std::string CDropDown::getItem(Uint16 p_index) {
	return m_itemList[p_index];
}
Component* CDropDown::addItem(std::string p_item) {
	m_itemList.push_back(p_item);
	return this;
}

void CDropDown::setSelectedItem(Uint16 p_selectedItem) {
	m_prevSelectedItem = m_selectedItem;
	m_selectedItem = p_selectedItem;
	m_update = true;
}

void CDropDown::input(Sint8& p_interactFlags) {
	m_prevSelectedItem = m_selectedItem;
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;

	m_update = false;
	if(p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
		if(m_selected == 0) {
			if(_mousePos.x >= 0 && _mousePos.x < m_size.x &&
				_mousePos.y >= 0 && _mousePos.y < m_size.y) {
				addTooltip();
				if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					m_selected = 1;
					p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
					callPressFunction();
				}
			}
		}
		else if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			m_selected = 0;
			if(_mousePos.x >= 0 && _mousePos.x < m_size.x && _mousePos.y >= m_size.y && _mousePos.y < m_size.y * Sint32(m_itemList.size() + 1)) {
				m_selectedItem = _mousePos.y / m_size.y - 1;
				callPressFunction();
				m_update = true;
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
			}
		}
	}

	if(m_selected != 0) {
		if((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && _mousePos.x >= 0 && _mousePos.x < m_size.x && _mousePos.y >= m_size.y && _mousePos.y < m_size.y * Sint32(m_itemList.size() + 1)) {
			m_hoverItem = _mousePos.y / m_size.y - 1;
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
		else
			m_hoverItem = -1;
	}
}
void CDropDown::update(GLfloat p_deltaUpdate) {

}
void CDropDown::render() {
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);

		if(m_selected == 0) {
			glBegin(GL_QUADS);
			{
				m_colorTheme.m_border.useColor();
				glVertex2f(-1, -1);
				glVertex2f(GLfloat(m_size.x + 1), -1);
				glVertex2f(GLfloat(m_size.x + 1), GLfloat(m_size.y + 1));
				glVertex2f(-1, GLfloat(m_size.y + 1));

				m_colorTheme.m_primary.useColor();
				glVertex2f(0, 0);
				glVertex2f(GLfloat(m_size.x), 0);
				glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
				glVertex2f(0, GLfloat(m_size.y));
			}
			glEnd();
			m_arrow->bind();
			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 1); glVertex2f(m_size.x - 24, 0);
				glTexCoord2f(1, 1); glVertex2f(m_size.x, 0);
				glTexCoord2f(1, 0); glVertex2f(m_size.x, 24);
				glTexCoord2f(0, 0); glVertex2f(m_size.x - 24, 24);
			}
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			glBegin(GL_QUADS);
			{
				m_colorTheme.m_border.useColor();
				glVertex2f(-1, -1);
				glVertex2f(GLfloat(m_size.x + 1), -1);
				glVertex2f(GLfloat(m_size.x + 1), GLfloat(m_size.y * (m_itemList.size() + 1) + 1));
				glVertex2f(-1, GLfloat(m_size.y * (m_itemList.size() + 1) + 1));

				m_colorTheme.m_primary.useColor();
				glVertex2f(0, -0);
				glVertex2f(GLfloat(m_size.x), -0);
				glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y * (m_itemList.size() + 1)));
				glVertex2f(0, GLfloat(m_size.y * (m_itemList.size() + 1)));
			}
			glEnd();
		}
		if(m_selected != 0) {
			if(m_hoverItem != -1) {
				m_colorTheme.m_select.useColor();
				glBegin(GL_QUADS);
				{
					glVertex2f(0, GLfloat(m_hoverItem + 1) * m_size.y);
					glVertex2f(GLfloat(m_size.x), GLfloat(m_hoverItem + 1) * m_size.y);
					glVertex2f(GLfloat(m_size.x), GLfloat(m_hoverItem + 2) * m_size.y);
					glVertex2f(0, GLfloat(m_hoverItem + 2) * m_size.y);
				}
				glEnd();
			}
			if(m_hoverItem != m_selectedItem) {
				((m_colorTheme.m_select + m_colorTheme.m_primary) / 2).useColor();
				glBegin(GL_QUADS);
				{
					glVertex2f(0, GLfloat(m_selectedItem + 1) * m_size.y);
					glVertex2f(GLfloat(m_size.x), GLfloat(m_selectedItem + 1) * m_size.y);
					glVertex2f(GLfloat(m_size.x), GLfloat(m_selectedItem + 2) * m_size.y);
					glVertex2f(0, GLfloat(m_selectedItem + 2) * m_size.y);
				}
				glEnd();
			}
		}
		m_colorTheme.m_text.useColor();
		Font::setAlignment(ALIGN_CENTER);
		Font::print(m_title, m_size.x / 2, -(Font::getHeight()));
		m_colorTheme.m_text.useColor();
		Font::setAlignment(ALIGN_LEFT);
		glTranslatef(GLfloat(m_size.y) / 2, GLfloat(m_size.y) / 2, 0);
		if(m_itemList.size() > 0) {
			Font::print(m_itemList[m_selectedItem], 0, 0);
			if(m_selected != 0) {
				for(Uint16 i = 0; i < m_itemList.size(); i++) {
					m_colorTheme.m_text.useColor(m_hoverItem == i ? 1 : 0.8f);
					Font::print(m_itemList[i], 0, (i + 1) * m_size.y);
				}
			}
		}
	}
	glPopMatrix();
}

Uint8 CDropDown::isUpdated() {
	return m_update;
}

Sint16 CDropDown::getSelectedItem() {
	return m_selectedItem;
}
Sint16 CDropDown::getPrevSelectedItem() {
	return m_prevSelectedItem;
}

Vector2<Sint32> CDropDown::getRealPosition() {
	return Vector2<Sint32>(m_size.x, m_size.y);
}
Vector2<Sint32> CDropDown::getRealSize() {
	return Vector2<Sint32>(m_size.x, (m_size.y) * (m_itemList.size() + 1));
}
