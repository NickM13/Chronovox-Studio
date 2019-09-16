#include "engine\gfx\gui\component\list\TabBar.h"
#include "engine\utils\Utilities.h"

CTabBar::CTabBar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Component(p_compName, "", p_pos, p_size) {
	m_selected = -1;
	m_texClose = MTexture::getTexture("gui\\icon\\tool\\Close.png");
	m_visibleItems = 0;
	m_extList.texArrow = MTexture::getTexture("gui\\icon\\tool\\TabbarArrow.png");
	m_extList.maxShown = 16;
	m_extList.width = 10;
}

void CTabBar::setSelected(Uint16 p_selected) {
	m_selected = p_selected;
	m_pressFunction();
}

void CTabBar::refreshSize() {
	m_visibleItems = 0;
	Sint32 tWidth = 0, width;
	// Loop through list of tabs
	for (Tab t : m_tabList) {
		width = Font::getMessageWidth(t.title).x + m_tabWidthAddition;
		// If the width of the tab goes over
		// the tab bar's width, break
		if (tWidth + width > m_size.x - m_extList.texArrow->getSize().x) {
			break;
		}
		tWidth += width;
		m_visibleItems++;
	}

	m_extList.width = 10;
	for (Tab t : m_tabList) {
		width = Font::getMessageWidth(t.title).x + m_tabWidthAddition;
		if (width > m_extList.width) {
			m_extList.width = width;
		}
	}
	m_extList.shown = static_cast<Sint32>(std::fmin(m_tabList.size(), m_extList.maxShown));
}

Uint16 CTabBar::getTabCount() {
	return Uint16(m_tabList.size());
}
void CTabBar::removeTab(Sint32 p_index) {
	m_tabList.erase(m_tabList.begin() + p_index);
	if (m_selected >= (Sint32)m_tabList.size()) {
		m_selected--;
	}
	refreshSize();
}
void CTabBar::clear() {
	m_tabList.clear();
	refreshSize();
}

Sint16 CTabBar::getSelectedItem() {
	return m_selected;
}
Sint16 CTabBar::getClosedItem() {
	return m_closedItem;
}

Component* CTabBar::addItem(std::string p_name, std::string p_desc) {
	m_tabList.insert(m_tabList.begin(), { p_name, p_desc });
	m_pressFunction();
	refreshSize();
	return this;
}

void CTabBar::renameItem(Sint32 p_index, std::string p_name, std::string p_desc) {
	m_tabList[p_index] = { p_name, p_desc };
	refreshSize();
}

void CTabBar::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;

	m_pHovered = m_hovered;
	m_hovered = m_cHovered = -1;
	m_extList.hovered = false;
	m_extList.hoveredItem = -1;

	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER)) {
		if (m_extList.open) {
			if (_mousePos.x > m_size.x - m_extList.width - 1
				&& _mousePos.y > m_size.y && _mousePos.y < (m_extList.shown + 1) * m_size.y) {
				m_extList.hoveredItem = (_mousePos.y + m_extList.scroll) / m_size.y - 1;
				if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					m_selected = m_extList.hoveredItem;
					if (m_selected > m_visibleItems) {
						
					}

					m_extList.open = false;
				}
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
			}
		}
	}
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER)) {
		if (_mousePos.x >= 0 && _mousePos.y >= 0 &&
			_mousePos.x <= m_size.x && _mousePos.y <= m_size.y) {
			if (_mousePos.x >= m_size.x - m_extList.texArrow->getSize().x) {
				m_extList.hovered = true;
				if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					m_extList.open = !m_extList.open;
				}
			}
			else {
				if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					m_extList.open = false;
				}
				Sint32 offset = 0, width = 0;

				Sint32 buffer;
				Vector2<Sint32> closePos1, closePos2;

				std::string title;
				for (Sint32 i = 0; i < m_visibleItems; i++) {
					title = m_tabList[i].title + (m_tabList[i].change ? "*" : "");
					width = Font::getMessageWidth(m_tabList[i].title).x + m_tabWidthAddition;
					if (_mousePos.x >= offset && _mousePos.x < offset + width) {
						buffer = m_size.y - m_texClose->getSize().y;
						closePos1 = Vector2<Sint32>(offset + width - m_texClose->getSize().x - buffer / 2, buffer / 2);
						closePos2 = Vector2<Sint32>(offset + width - buffer / 2, m_size.y - buffer / 2);
						if (_mousePos.x > closePos1.x && _mousePos.y > closePos1.y
							&& _mousePos.x < closePos2.x && _mousePos.y < closePos2.y) {
							m_cHovered = i;
						}
						if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
							if (m_cHovered == i) {
								m_closedItem = i;
								m_releaseFunction();
								break;
							}
							else if (i != m_selected) {
								setSelected(i);
							}
						}
						m_hovered = i;
					}
					offset += Font::getMessageWidth(m_tabList[i].title).x + m_tabWidthAddition;
				}
			}
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
	}
	if (m_hovered != -1 && m_hovered == m_pHovered) {
		addTooltip();
		m_tooltip = m_tabList[m_hovered].desc;
	}
	else {
		resetTooltip();
	}
}

void CTabBar::update(GLfloat p_deltaUpdate) {
	for (Sint32 i = 0; i < m_visibleItems; i++) {
		if (m_hovered == i) {
			m_tabList[i].hoverTimer += p_deltaUpdate * 8;
			if (m_tabList[i].hoverTimer > 1) {
				m_tabList[i].hoverTimer = 1;
			}
		}
		else {
			m_tabList[i].hoverTimer -= p_deltaUpdate * 8;
			if (m_tabList[i].hoverTimer < 0) {
				m_tabList[i].hoverTimer = 0;
			}
		}
	}
	if (m_extList.hovered) {
		m_extList.hoverTimer += p_deltaUpdate * 8;
		if (m_extList.hoverTimer > 1) {
			m_extList.hoverTimer = 1;
		}
	}
	else {
		m_extList.hoverTimer -= p_deltaUpdate * 8;
		if (m_extList.hoverTimer < 0) {
			m_extList.hoverTimer = 0;
		}
	}
}

void CTabBar::render() {
	Font::setAlignment(ALIGN_CENTER);
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));

	Shader::pushMatrixModel();
	Vector2<Sint32> size;

	Sint32 buffer;
	Vector2<Sint32> closePos1, closePos2;

	std::string title;
	for (Sint32 i = 0; i < m_visibleItems; i++) {
		title = m_tabList[i].title + (m_tabList[i].change ? "*" : "");
		size = Vector2<Sint32>(Font::getMessageWidth(m_tabList[i].title).x + m_tabWidthAddition, m_size.y);

		GBuffer::setColor(getPrimaryColor());
		GBuffer::setTexture(0);
		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(size.x, 0);
		GBuffer::addVertexQuad(size.x, size.y);
		GBuffer::addVertexQuad(0, size.y);

		if (m_selected == i || m_tabList[i].hoverTimer > 0) {
			if (m_selected == i)	GBuffer::setColor(getElementColor(getElementPos() + "ActionHighlight"));
			else					GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered").applyScale(1.f, 1.f, 1.f, m_tabList[i].hoverTimer));
			GBuffer::addVertexQuad(0, 0);
			GBuffer::addVertexQuad(size.x, 0);
			GBuffer::addVertexQuad(size.x, size.y);
			GBuffer::addVertexQuad(0, size.y);
		}

		buffer = m_size.y - m_texClose->getSize().y;
		closePos1 = Vector2<Sint32>(size.x - m_texClose->getSize().x - buffer / 2,	buffer / 2);
		closePos2 = Vector2<Sint32>(size.x - buffer / 2, m_size.y - buffer / 2);

		if (m_selected == i || m_hovered == i) {
			GBuffer::setTexture(m_texClose->getTexId());
			GBuffer::setColor(Color());
			GBuffer::setUV(0, 0); GBuffer::addVertexQuad(closePos1.x, closePos1.y);
			GBuffer::setUV(1, 0); GBuffer::addVertexQuad(closePos2.x, closePos1.y);
			GBuffer::setUV(1, 1); GBuffer::addVertexQuad(closePos2.x, closePos2.y);
			GBuffer::setUV(0, 1); GBuffer::addVertexQuad(closePos1.x, closePos2.y);

			if (m_cHovered == i) {
				GBuffer::setTexture(0);
				GBuffer::setColor(Color(1, 1, 1, 0.25f));
				GBuffer::addVertexQuad(closePos1.x, closePos1.y);
				GBuffer::addVertexQuad(closePos2.x, closePos1.y);
				GBuffer::addVertexQuad(closePos2.x, closePos2.y);
				GBuffer::addVertexQuad(closePos1.x, closePos2.y);
			}
		}

		GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
		Font::setAlignment(Alignment::ALIGN_LEFT);
		Font::print(title, 5, Font::getSpacingHeight() / 2);

		Shader::translate(glm::vec3((GLfloat)size.x, 0.f, 0.f));
	}
	Shader::popMatrixModel();
	Shader::translate(glm::vec3(m_size.x - 1, 0, 0));

	Shader::pushMatrixModel();
	GBuffer::setScissorActive(false);
	Shader::translate(glm::vec3(-m_extList.width, m_size.y, 0));
	GBuffer::setTexture(0);
	if (m_extList.open) {
		GBuffer::setColor(getElementColor(getElementPos() + "BorderUnfocused"));
		GBuffer::addQuadFilled(Vector2<Sint32>(-1, -1), Vector2<Sint32>(m_extList.width, m_extList.shown * m_size.y) + 2);
		GBuffer::setColor(getElementColor(getElementPos() + "ActionPressed"));
		GBuffer::addQuadFilled(Vector2<Sint32>(), Vector2<Sint32>(m_extList.width, m_extList.shown * m_size.y));

		if (m_extList.hoveredItem >= 0 && m_extList.hoveredItem < m_extList.shown) {
			GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered"));
			GBuffer::addQuadFilled(Vector2<Sint32>(0, m_size.y * m_extList.hoveredItem), Vector2<Sint32>(m_extList.width, m_size.y));
		}

		GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
		for (size_t i = 0; i < (size_t)m_extList.shown; i++) {
			Font::print(m_tabList.at(i).title, 4, m_size.y * (i + 0.5f));
		}
	}
	GBuffer::setScissorActive(true);
	Shader::popMatrixModel();

	GBuffer::setTexture(0);
	if (m_extList.open || m_extList.hoverTimer > 0) {
		if (m_extList.open) {
			GBuffer::setColor(getElementColor(getElementPos() + "BorderUnfocused"));
			GBuffer::addQuadFilled(Vector2<Sint32>(-m_extList.texArrow->getSize().x - 1, -1), Vector2<Sint32>(m_extList.texArrow->getSize().x + 2, m_size.y + 1));
			GBuffer::setColor(getElementColor(getElementPos() + "ActionPressed"));
		}
		else {
			GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered").applyScale(1.f, 1.f, 1.f, m_extList.hoverTimer));
		}
		GBuffer::addQuadFilled(Vector2<Sint32>(-m_extList.texArrow->getSize().x, 0), Vector2<Sint32>(m_extList.texArrow->getSize().x, m_size.y));
	}

	GBuffer::setTexture(m_extList.texArrow->getTexId());
	GBuffer::setColor(Color());
	GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0,									0);
	GBuffer::setUV(1, 0); GBuffer::addVertexQuad(-m_extList.texArrow->getSize().x,	0);
	GBuffer::setUV(1, 1); GBuffer::addVertexQuad(-m_extList.texArrow->getSize().x,	m_extList.texArrow->getSize().y);
	GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0,									m_extList.texArrow->getSize().y);

	Shader::popMatrixModel();
}
