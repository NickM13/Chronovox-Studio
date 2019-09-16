#include "engine\gfx\gui\component\list\DropDown.h"
#include "engine\gfx\font\Font.h"

CDropDown::CDropDown(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Sint32 p_width)
	: Component(p_compName, p_title, p_pos, { p_width, 24 }) {
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
	m_hovered = false;
	if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
		if (m_selected == 0) {
			if (_mousePos.x >= 0 && _mousePos.x < m_size.x &&
				_mousePos.y >= 0 && _mousePos.y < m_size.y) {
				addTooltip();
				m_hovered = true;
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
	} else if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		m_selected = 0;
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
	Component::update(p_deltaUpdate);
}
void CDropDown::render() {
	Shader::pushMatrixModel();

	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	Font::setAlignment(ALIGN_LEFT);

	if (m_selected) {
		GBuffer::renderShadow({}, Vector2<Sint32>(m_size.x, m_size.y * (m_itemList.size() + 1) + 2));
	} else {
		GBuffer::renderShadow({}, m_size);
	}

	// Selected item display
		GBuffer::setTexture(0);
	GBuffer::setColor(getElementColor(getElementPos() + "Primary"));
	GBuffer::addQuadFilled({}, m_size);
	if (m_selected) {
		GBuffer::setColor(getElementColor(getElementPos() + "ActionPressed"));
		GBuffer::addQuadFilled({}, m_size);
	} else if (m_hoverTimer > 0) {
		GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered").applyScale(1, 1, 1, m_hoverTimer));
		GBuffer::addQuadFilled({}, m_size);
	}
	GBuffer::renderTexture(m_arrow, { m_size.x - m_arrow->getSize().x / 2, m_size.y / 2 }, {}, GBuffer::TextureStyle::CENTERED);
	GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
	Font::print(m_itemList[m_selectedItem], 6, 0.5f * m_size.y);
	GBuffer::setTexture(0);

	if (m_selected) {
		GBuffer::setTexture(0);
		GBuffer::setColor(getElementColor(getElementPos() + "BorderUnfocused"));
		GBuffer::addQuadOutlined({}, { m_size.x, m_size.y + 1 });
		GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered"));
		GBuffer::addQuadFilled({ 0, m_size.y + 1 }, Vector2<Sint32>(m_size.x, m_size.y * m_itemList.size() + 1));

		// Render list
		if (m_selected) {
			for (size_t i = 0; i < m_itemList.size(); i++) {
				GBuffer::setTexture(0);
				if (m_hoverItem == i)	GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered"));
				else					GBuffer::setColor(getElementColor(getElementPos() + "Primary"));
				GBuffer::addQuadFilled(Vector2<Sint32>(1, (i + 1) * m_size.y + 2), Vector2<Sint32>(m_size.x - 2, m_size.y - 1));
				GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
				Font::print(m_itemList[i], 6, (i + 1.5f) * m_size.y);
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
