#include "engine\gfx\gui\component\list\TabBar.h"
#include "engine\utils\Utilities.h"

CTabBar::CTabBar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme)
	: Component(p_compName, "", p_pos, p_size, p_colorTheme) {
	m_selected = -1;
	m_texClose = MTexture::getTexture("gui\\icon\\tool\\Close.png");
	m_elTexArrow = MTexture::getTexture("gui\\icon\\tool\\DropdownArrow.png");
}

void CTabBar::setSelected(Uint16 p_selected) {
	m_selected = p_selected;
	m_pressFunction();
}

void CTabBar::refreshSize() {
	m_visibleItems = 0;
	Sint32 tWidth = 0, width;
	for (Tab t : m_tabList) {
		width = Font::getMessageWidth(t.title).x + m_tabWidthAddition;
		if (tWidth + width > m_size.x - m_elTexArrow->getSize().x) {
			break;
		}
		tWidth += width;
		m_visibleItems++;
	}
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
	m_tabList.push_back({ p_name, p_desc });
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

	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER)) {
		if (_mousePos.x >= 0 && _mousePos.y >= 0 &&
			_mousePos.x <= m_size.x && _mousePos.y <= m_size.y) {
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
						else {
							setSelected(i);
						}
					}
					m_hovered = i;
				}
				offset += Font::getMessageWidth(m_tabList[i].title).x + m_tabWidthAddition;
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

}

void CTabBar::render() {
	Font::setAlignment(ALIGN_CENTER);
	Shader::pushMatrixModel();
	GBuffer::setTexture(0);
	Component::render();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));

	Shader::pushMatrixModel();
	Vector2<Sint32> size;

	Sint32 buffer;
	Vector2<Sint32> closePos1, closePos2;

	std::string title;
	for (Sint32 i = 0; i < m_visibleItems; i++) {
		title = m_tabList[i].title + (m_tabList[i].change ? "*" : "");
		size = Vector2<Sint32>(Font::getMessageWidth(m_tabList[i].title).x + m_tabWidthAddition, m_size.y);

		if (m_selected == i)		GBuffer::setColor(m_colorTheme->m_select);
		else if (m_hovered == i)	GBuffer::setColor(m_colorTheme->m_hover);
		else						GBuffer::setColor(m_colorTheme->m_primary);
		GBuffer::setTexture(0);
		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(size.x, 0);
		GBuffer::addVertexQuad(size.x, size.y);
		GBuffer::addVertexQuad(0, size.y);

		buffer = m_size.y - m_texClose->getSize().y;
		closePos1 = Vector2<Sint32>(size.x - m_texClose->getSize().x - buffer / 2,	buffer / 2);
		closePos2 = Vector2<Sint32>(size.x - buffer / 2, m_size.y - buffer / 2);

		if (m_selected == i || m_hovered == i) {
			GBuffer::setTexture(m_texClose->getGlId());
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

		GBuffer::setColor(m_colorTheme->m_text);
		Font::setAlignment(Alignment::ALIGN_LEFT);
		Font::print(title, 4, Font::getSpacingHeight() / 2);

		Shader::translate(glm::vec3((GLfloat)size.x, 0.f, 0.f));
	}
	Shader::translate(glm::vec3(m_size.x - m_elTexArrow->getSize().x, 0, 0));
	GBuffer::setTexture(m_elTexArrow->getGlId());
	GBuffer::setColor(Color());
	GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0,							0);
	GBuffer::setUV(1, 1); GBuffer::addVertexQuad(m_elTexArrow->getSize().x,	0);
	GBuffer::setUV(1, 0); GBuffer::addVertexQuad(m_elTexArrow->getSize().x, m_elTexArrow->getSize().y);
	GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0,							m_elTexArrow->getSize().y);
	Shader::popMatrixModel();
	Shader::popMatrixModel();
}
