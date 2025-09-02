#include "engine\gfx\gui\component\list\List.h"

CList::CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight)
	: Component(p_compName, p_title, p_pos, p_size) {
	p_size = p_size - Vector2<Sint32>(0, 1);
	m_itemHeight = p_itemHeight;
	m_scroll = m_maxScroll = 0;
	m_selectedItem = m_hoveredItem = m_selectedItemCtrl = -1;
	m_visibleTexture = MTexture::getTexture("gui\\icon\\list\\Visible.png");
	m_invisibleTexture = MTexture::getTexture("gui\\icon\\list\\Invisible.png");
	resize();
}

void CList::resize() {
	m_maxVisible = (GLfloat)m_size.y / m_itemHeight;
	m_maxScroll = std::fmaxf(0, Sint16((m_itemList.size() - m_maxVisible) * m_itemHeight));
	m_scrollBarHeight = (Sint32)(powf(m_size.y, 2) / (m_itemList.size() * m_itemHeight));
}

Component* CList::addItem(std::string p_itemName, bool p_isVisible, bool p_selected) {
	m_itemList.push_back(ListItem(p_itemName, 0, p_isVisible));
	m_maxScroll = std::fmaxf(0, Sint16((m_itemList.size() - m_maxVisible) * m_itemHeight));
	m_itemList.back().state = p_selected ? 2 : 0;
	resize();
	return this;
}
Component* CList::insertItem(Uint16 p_index, std::string p_itemName) {
	m_itemList.insert(m_itemList.begin() + p_index, ListItem(p_itemName, 0));
	m_maxScroll = m_maxScroll = std::fmaxf(0, Sint16((m_itemList.size() - m_maxVisible) * m_itemHeight));
	resize();
	return this;
}
void CList::removeItem(Uint16 p_index) {
	if (m_itemList.size() > p_index) {
		m_itemList.erase(m_itemList.begin() + p_index);
		resize();
	}
}
Uint16 CList::getItemCount() {
	return Uint16(m_itemList.size());
}
CList::ListItem &CList::getItem(Sint16 p_index) {
	return m_itemList.at(p_index);
}
void CList::clear() {
	m_itemList.clear();
	m_selectedItem = m_hoveredItem = m_selectedItemCtrl = -1;
	resize();
}

void CList::selectItem(Sint16 id) {
	Sint8 _state = m_itemList[id].state;
	if (!GKey::modDown(GLFW_MOD_CONTROL)) {
		for (ListItem& item : m_itemList) {
			if (item.state == 2) item.state = 0;
		}
	}
	if (_state != 2) {
		m_itemList[id].state = 2;
		m_selectedItem = id;
		if (!GKey::modDown(GLFW_MOD_CONTROL)) m_selectedItemCtrl = m_selectedItem;
	} else {
		m_itemList[id].state = 0;
		m_selectedItem = -1;
	}
}

void CList::toggleVisiblity(Sint16 id) {
	m_itemList[id].visible = !m_itemList[id].visible;
	callHoldFunction();
}

void CList::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
	m_hover = ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x >= 0 && _mousePos.x <= m_size.x &&
		_mousePos.y >= 0 && _mousePos.y <= m_size.y);

	if (m_hoveredItem >= 0) {
		if (m_hoveredItem < (Sint16)m_itemList.size() && m_itemList[m_hoveredItem].state == 1) {
			m_itemList[m_hoveredItem].state = 0;
		}
		m_hoveredItem = -1;
	}

	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x >= 0 && _mousePos.x <= m_size.x &&
		_mousePos.y >= 0 && _mousePos.y <= m_size.y) {
		if (_mousePos.x < m_size.x - 12) {
			if (Sint32((_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight) < Sint32(m_itemList.size())) {
				Uint16 _hoveredItem = Uint16((_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight);
				if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					if (_mousePos.x > 24) {
						selectItem(_hoveredItem);
						if (m_itemList[_hoveredItem].state == 0) {
							m_itemList[_hoveredItem].state = 1;
						}
						callPressFunction();
					}
					else {
						toggleVisiblity(_hoveredItem);
					}
				}
				else {
					m_hoveredItem = _hoveredItem;
					if (m_itemList[_hoveredItem].state != 2)
						m_itemList[m_hoveredItem].state = 1;
				}
				GGui::setCursorType(GGui::CursorType::LINK);
			}
		}
		else {
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if (_mousePos.y < ((GLfloat)m_scroll / m_maxScroll) * (m_size.y - m_scrollBarHeight)) {
					m_scroll -= 24;
				}
				else if (_mousePos.y > ((GLfloat)m_scroll / m_maxScroll) * (m_size.y - m_scrollBarHeight) + m_scrollBarHeight) {
					m_scroll += 24;
				}
				m_dragging = true;
			}
		}
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
	if ((p_interactFlags & (Sint8)EventFlag::KEYPRESS)) {
		bool moved = false;
		Sint32 selected = m_selectedItem;
		if (GKey::keyPressed(GLFW_KEY_UP)) {
			if (getItemCount() == 0) {
				selected = -1;
			}
			else if (getItemCount() == 1) {
				selected = 0;
			}
			else {
				if (selected == -1) selected = 0;
				else {
					selected--;
					if (selected < 0) selected = selected % getItemCount() + getItemCount();
				}
			}
			moved = true;
		}
		else if (GKey::keyPressed(GLFW_KEY_DOWN)) {
			if (getItemCount() == 0) {
				selected = -1;
			}
			else if (getItemCount() == 1) {
				selected = 0;
			}
			else {
				if (selected == -1) selected = getItemCount() - 1;
				else {
					selected++;
					if (selected >= getItemCount()) selected = selected % getItemCount();
				}
			}
			moved = true;
		}
		if (moved) {
			m_selectedItem = selected;
			callPressFunction();
			if (!GKey::modDown(GLFW_MOD_CONTROL)) {
				for (ListItem &item : m_itemList) {
					if (item.state == 2) item.state = 0;
				}
				if (m_selectedItem != -1) {
					m_itemList[m_selectedItem].state = 2;
				}
			}
			if (!GKey::modDown(GLFW_MOD_CONTROL) && !GKey::modDown(GLFW_MOD_SHIFT)) {
				m_selectedItemCtrl = m_selectedItem;
			}
			if (GKey::modDown(GLFW_MOD_SHIFT)) {
				Sint32 low = std::fminf(m_selectedItemCtrl, m_selectedItem), high = std::fmaxf(m_selectedItemCtrl, m_selectedItem);
				for (Sint32 i = std::fmaxf(0, low); i <= std::fminf(getItemCount() - 1, high); i++) {
					m_itemList[i].state = 2;
				}
			}
			if (m_selectedItem < std::ceil((GLfloat)m_scroll / m_itemHeight)) {
				m_scroll = m_selectedItem * m_itemHeight;
			}
			else if (m_selectedItem + 1 > (GLfloat)m_scroll / m_itemHeight + m_maxVisible) {
				m_scroll = (m_selectedItem + 1 - m_maxVisible) * m_itemHeight;
			}
			p_interactFlags -= (Sint8)EventFlag::KEYPRESS;
		}
	}

	if (m_dragging) {
		if (!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			m_dragging = false;
		}
		else {
			m_scroll += static_cast<Sint16>((GLfloat(_mousePos.y - m_mouseBuffer.y) / (m_size.y - m_scrollBarHeight)) * m_maxScroll);
			if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER)
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
	}
	if ((p_interactFlags & (Sint8)EventFlag::MOUSESCROLL) && m_hover) {
		m_scroll = m_scroll - GMouse::getMouseScroll() * m_itemHeight;
		p_interactFlags -= (Sint8)EventFlag::MOUSESCROLL;
	}

	if (m_scroll > m_maxScroll)	m_scroll = m_maxScroll;
	if (m_scroll < 0)			m_scroll = 0;

	m_mouseBuffer = _mousePos;
}
void CList::update(GLfloat p_deltaUpdate) {
	for (Uint16 y = 0; y <= m_maxVisible; y++) {
		if (m_scroll / m_itemHeight + y >= Uint16(m_itemList.size())) continue;

		if (m_itemList.at(m_scroll / m_itemHeight + y).state == 1 || m_hoveredItem == m_scroll / m_itemHeight + y) {
			m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer += p_deltaUpdate * 8;
			if (m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer > 1) {
				m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer = 1;
			}
		}
		else {
			m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer -= p_deltaUpdate * 8;
			if (m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer < 0) {
				m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer = 0;
			}
		}
	}
}
void CList::renderItems() {
	Shader::pushMatrixModel();
	GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
	Font::setAlignment(ALIGN_CENTER);
	Font::print(m_title, m_size.x / 2, -12);

	GBuffer::pushScissor(Rect(0, 0, GLfloat(m_size.x), GLfloat(m_size.y)));

	GBuffer::setTexture(0);
	Shader::translate(glm::vec3(0.f, -(GLfloat)(m_scroll % m_itemHeight), 0.f));

	for (Uint16 y = 0; y <= m_maxVisible; y++) {
		if (m_scroll / m_itemHeight + y >= Uint16(m_itemList.size())) continue;

		Shader::pushMatrixModel();
		Shader::translate(glm::vec3(0, y * m_itemHeight, 0));

		if (m_itemList.at(m_scroll / m_itemHeight + y).state == 2) {
			GBuffer::setColor(getElementColor(getElementPos() + "ActionHighlight"));
			GBuffer::addQuadFilled({ 0, 1 }, { m_size.x, m_itemHeight - 1 });
		}
		else {
			GBuffer::setColor(getPrimaryColor());
			if (m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer > 0) {
				GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered").applyScale(1.f, 1.f, 1.f, m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer));
				GBuffer::addQuadFilled({ 0, 1 }, { m_size.x, m_itemHeight - 1 });
			}
		}
		if (m_scroll / m_itemHeight + y == m_selectedItem) {
			GBuffer::setColor(getElementColor(getElementPos() + "ActionHighlight"));
			GBuffer::addQuadOutlined({ 0, 1 }, { m_size.x, m_itemHeight - 1 });
		}
		
		GBuffer::setColor(getElementColor(getElementPos() + "BorderUnfocused"));
		GBuffer::addQuadFilled({ 0, m_itemHeight }, { m_size.x, 1 });

		GBuffer::setColor(Color());
		GBuffer::setTexture(m_itemList.at(y + m_scroll / m_itemHeight).visible ? m_visibleTexture->getTexId() : m_invisibleTexture->getTexId());
		GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, 0);
		GBuffer::setUV(1, 1); GBuffer::addVertexQuad(24, 0);
		GBuffer::setUV(1, 0); GBuffer::addVertexQuad(24, 24);
		GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, 24);
		GBuffer::setTexture(0);

		Shader::popMatrixModel();
	}

	GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
	Font::setAlignment(ALIGN_LEFT);
	std::string _name;
	for (Uint16 i = 0; i <= m_maxVisible; i++) {
		if (i + round(m_scroll / m_itemHeight) < m_itemList.size()) {
			_name = m_itemList[i + m_scroll / m_itemHeight].name;
			_name = Font::getMessageSubstr(m_itemList[i + m_scroll / m_itemHeight].name, m_size.x - 112);
			Font::print(_name, 24, Sint32((i + 0.5f) * m_itemHeight) - 1);
		}
	}
	GBuffer::setTexture(0);
	Shader::popMatrixModel();
}
void CList::render() {
	Shader::pushMatrixModel();
	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	renderItems();

	if (m_maxScroll > 0) {
		Shader::pushMatrixModel();

		GBuffer::setColor(getElementColor(getElementPos() + "Primary"));
		GBuffer::addVertexQuad(m_size.x - 11, 1);
		GBuffer::addVertexQuad(m_size.x - 1, 1);
		GBuffer::addVertexQuad(m_size.x - 1, m_size.y - 1);
		GBuffer::addVertexQuad(m_size.x - 11, m_size.y - 1);

		Shader::translate(glm::vec3(m_size.x - 12, ((GLfloat)m_scroll / m_maxScroll) * (m_size.y - m_scrollBarHeight), 0.f));

		if (m_hover || m_dragging) GBuffer::setColor(getElementColor(getElementPos() + "ActionPressed"));
		else GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered"));
		GBuffer::addVertexQuad(3, 3);
		GBuffer::addVertexQuad(9, 3);
		GBuffer::addVertexQuad(9, m_scrollBarHeight - 3);
		GBuffer::addVertexQuad(3, m_scrollBarHeight - 3);

		Shader::popMatrixModel();
	}
	GBuffer::popScissor();
	Shader::popMatrixModel();
	renderBorder();
}
