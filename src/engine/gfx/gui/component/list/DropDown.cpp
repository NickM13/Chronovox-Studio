#include "engine\gfx\gui\component\list\DropDown.h"
#include "engine\gfx\font\Font.h"

CDropDown::CDropDown(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_width, Theme p_colorTheme)
	: Component(p_compName, p_title, p_pos, { p_width, 24 }, p_colorTheme) {
	m_prevSelectedItem = m_selectedItem = 0;
	m_hoverItem = -1;
	setPriorityLayer(4);
	m_arrow = MTexture::getTexture("gui\\icon\\tool\\DropdownArrow.png");
}

void CDropDown::setList(std::vector<std::string> p_items) {
	bool different = false;
	if (p_items.size() == m_itemList.size()) {
		for (Uint16 i = 0; i < p_items.size(); i++)
			if (p_items[i] != m_itemList[i]) {
				different = true;
				break;
			}
	}
	else {
		different = true;
	}
	if (different) {
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
	if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
		if (m_selected == 0) {
			if (_mousePos.x >= 0 && _mousePos.x < m_size.x &&
				_mousePos.y >= 0 && _mousePos.y < m_size.y) {
				addTooltip();
				if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					m_selected = 1;
					p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
					callPressFunction();
				}
			}
		}
		else if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			m_selected = 0;
			if (_mousePos.x >= 0 && _mousePos.x < m_size.x && _mousePos.y >= m_size.y && _mousePos.y < m_size.y * Sint32(m_itemList.size() + 1)) {
				m_selectedItem = _mousePos.y / m_size.y - 1;
				callPressFunction();
				m_update = true;
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
			}
		}
	}

	if (m_selected != 0) {
		if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && _mousePos.x >= 0 && _mousePos.x < m_size.x && _mousePos.y >= m_size.y && _mousePos.y < m_size.y * Sint32(m_itemList.size() + 1)) {
			m_hoverItem = _mousePos.y / m_size.y - 1;
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
		else {
			m_hoverItem = -1;
		}
	}
}
void CDropDown::update(GLfloat p_deltaUpdate) {

}
void CDropDown::render() {
	Shader::pushMatrixModel();

	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));

	if (m_selected == 0) {
		GBuffer::setColor(m_colorTheme.m_border);
		GBuffer::addVertexQuad(-1, -1);
		GBuffer::addVertexQuad((m_size.x + 1), -1);
		GBuffer::addVertexQuad((m_size.x + 1), (m_size.y + 1));
		GBuffer::addVertexQuad(-1, (m_size.y + 1));

		GBuffer::setColor(m_colorTheme.m_primary);
		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(m_size.x, 0);
		GBuffer::addVertexQuad(m_size.x, m_size.y);
		GBuffer::addVertexQuad(0, m_size.y);
		GBuffer::setTexture(m_arrow->getGlId());
		GBuffer::setColor(Color(1.f, 1.f, 1.f));
		GBuffer::setUV(0.f, 1.f); GBuffer::addVertexQuad(m_size.x - 24, 0);
		GBuffer::setUV(1.f, 1.f); GBuffer::addVertexQuad(m_size.x, 0);
		GBuffer::setUV(1.f, 0.f); GBuffer::addVertexQuad(m_size.x, 24);
		GBuffer::setUV(0.f, 0.f); GBuffer::addVertexQuad(m_size.x - 24, 24);
		GBuffer::setTexture(0);
	}
	else {
		GBuffer::setColor(m_colorTheme.m_border);
		GBuffer::addVertexQuad(-1, -1);
		GBuffer::addVertexQuad((m_size.x + 1), -1);
		GBuffer::addVertexQuad((m_size.x + 1), (m_size.y * (m_itemList.size() + 1) + 1));
		GBuffer::addVertexQuad(-1, (m_size.y * (m_itemList.size() + 1) + 1));

		GBuffer::setColor(m_colorTheme.m_primary);
		GBuffer::addVertexQuad(0, -0);
		GBuffer::addVertexQuad(m_size.x, -0);
		GBuffer::addVertexQuad(m_size.x, (m_size.y * (m_itemList.size() + 1)));
		GBuffer::addVertexQuad(0, (m_size.y * (m_itemList.size() + 1)));
	}
	if (m_selected != 0) {
		if (m_hoverItem != -1) {
			GBuffer::setColor(m_colorTheme.m_select);
			GBuffer::setColor(m_colorTheme.m_select);
			GBuffer::addVertexQuad(0, (m_hoverItem + 1) * m_size.y);
			GBuffer::addVertexQuad(m_size.x, (m_hoverItem + 1) * m_size.y);
			GBuffer::addVertexQuad(m_size.x, (m_hoverItem + 2) * m_size.y);
			GBuffer::addVertexQuad(0, (m_hoverItem + 2) * m_size.y);
		}
		if (m_hoverItem != m_selectedItem) {
			GBuffer::setColor((m_colorTheme.m_select + m_colorTheme.m_primary) / 2);

			GBuffer::addVertexQuad(0, (m_selectedItem + 1) * m_size.y);
			GBuffer::addVertexQuad(m_size.x, (m_selectedItem + 1) * m_size.y);
			GBuffer::addVertexQuad(m_size.x, (m_selectedItem + 2) * m_size.y);
			GBuffer::addVertexQuad(0, (m_selectedItem + 2) * m_size.y);
		}
	}
	GBuffer::setColor(m_colorTheme.m_text);
	Font::setAlignment(ALIGN_CENTER);
	Font::print(m_title, m_size.x / 2, -(Font::getHeight()));
	GBuffer::setColor(m_colorTheme.m_text);
	Font::setAlignment(ALIGN_LEFT);
	Shader::translate(glm::vec3((GLfloat)(m_size.y / 2), (GLfloat)(m_size.y) / 2, 0.f));
	if (m_itemList.size() > 0) {
		Font::print(m_itemList[m_selectedItem], 0, 0);
		if (m_selected != 0) {
			for (Uint16 i = 0; i < m_itemList.size(); i++) {
				GBuffer::setColor(m_colorTheme.m_text.applyScale(m_hoverItem ? Color(1.f, 1.f, 1.f) : Color(0.8f, 0.8f, 0.8f)));
				Font::print(m_itemList[i], 0, (i + 1) * m_size.y);
			}
		}
	}
	Shader::popMatrixModel();
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
