#include "engine\gfx\gui\container\ContainerPanel.h"

ContainerPanel::ContainerPanel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme, Sint8 p_borderFlags)
	: Container(p_compName, p_pos, p_size, true) {
	m_title = p_title;
	m_colorTheme = m_colorThemes[p_colorTheme];
	m_border = p_borderFlags;

	m_scroll = {0, 0};
	m_minScroll = m_maxScroll = {0, 0};

	m_draggable = false; //currently disabled
	m_scrollX = m_scrollY = false;

	m_panel = new Panel(p_compName.append("_PANEL"), p_title, p_pos, p_size, p_colorTheme, p_borderFlags);
}

Component* ContainerPanel::addComponent(Component* p_component, Anchor p_posAnchor, Anchor p_sizeAnchor) {
	Component* _comp = Container::addComponent(p_component, p_posAnchor, p_sizeAnchor);

	m_minScroll.x = std::fminf(m_minScroll.x, m_contentArea.x1);
	m_minScroll.y = std::fminf(m_minScroll.y, m_contentArea.y1);

	m_maxScroll.x = std::fmaxf(m_maxScroll.x, m_contentArea.x2 - m_size.x);
	m_maxScroll.y = std::fmaxf(m_maxScroll.y, m_contentArea.y2 - m_size.y);

	m_scroll = m_minScroll;

	if((m_minScroll.y != m_maxScroll.y) != m_scrollY)
		setScrollY(!m_scrollY);

	return _comp;
}

ContainerPanel::~ContainerPanel() {
	delete m_panel;
	for(std::pair<std::string, Comp> c : m_componentMap) {
		delete c.second.m_component;
	}
	m_componentMap.clear();
}

void ContainerPanel::calcSize(Vector2<Sint32> p_offset) {
	if(p_offset.getLength() == 0)
		m_minScroll = m_maxScroll = Vector2<Sint32>();
	m_contentArea = Vector4<Sint32>();
	Vector2<Sint32> cpos1, cpos2;
	for (std::pair<std::string, Comp> comp : m_componentMap) {
		Comp _comp = comp.second;
		Component* _component = _comp.m_component;
		if((Sint8)_comp.m_posAnchor & (Sint8)BAnchor::CENTER)
			_component->setPosition(_component->getPosition() + Vector2<Sint32>(p_offset.x / 2, 0));
		else if((Sint8)_comp.m_posAnchor & (Sint8)BAnchor::RIGHT)
			_component->setPosition(_component->getPosition() + Vector2<Sint32>(p_offset.x, 0));
		if(_component->isVisible()) {
			cpos1 = _component->getRealPosition() - m_pos;
			cpos2 = _component->getRealPosition() + _component->getRealSize() - m_pos;
			m_contentArea = Vector4<Sint32>(std::fminf(cpos1.x, m_contentArea.x1),
											std::fminf(cpos1.y, m_contentArea.y1),
											std::fmaxf(cpos2.x, m_contentArea.x2),
											std::fmaxf(cpos2.y, m_contentArea.y2));
		}
	}

	m_minScroll.x = std::fminf(m_minScroll.x, m_contentArea.x1);
	m_minScroll.y = std::fminf(m_minScroll.y, m_contentArea.y1);

	m_maxScroll.x = std::fmaxf(m_maxScroll.x, m_contentArea.x2 - m_size.x);
	m_maxScroll.y = std::fmaxf(m_maxScroll.y, m_contentArea.y2 - m_size.y);

	if((m_minScroll.y != m_maxScroll.y) != m_scrollY)
		setScrollY(!m_scrollY);
}
void ContainerPanel::resize() {
	Container::resize();
	m_panel->setPosition(m_pos);
	m_panel->setSize(m_size);
	calcSize();
}
//TODO: Set whether scroll bar on the bottom is visible
void ContainerPanel::setScrollX(bool p_state) {

}
//Set whether scroll bar on the right is visible
void ContainerPanel::setScrollY(bool p_state) {
	if(p_state != m_scrollY) {
		m_scrollY = p_state;
	}
}

void ContainerPanel::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	if(_mousePos.x - m_pos.x >= 0 && _mousePos.x - m_pos.x <= m_size.x + (m_scrollY ? 10 : 0) && 
		_mousePos.y - m_pos.y >= 0 && _mousePos.y - m_pos.y <= m_size.y)
		Container::input(p_interactFlags);
	else {
		bool _1 = (p_interactFlags & (Sint8)EventFlag::MOUSEOVER), _4 = (p_interactFlags & (Sint8)EventFlag::MOUSESCROLL);
		if(_1)
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		if(_4)
			p_interactFlags -= (Sint8)EventFlag::MOUSESCROLL;
		Container::input(p_interactFlags);
		if(_1)
			p_interactFlags += (Sint8)EventFlag::MOUSEOVER;
		if(_4)
			p_interactFlags += (Sint8)EventFlag::MOUSESCROLL;
	}

	m_moveToFront = (m_lHeld != 0);

	// Scroll window
	if((p_interactFlags & (Sint8)EventFlag::MOUSESCROLL) &&
		_mousePos.x - m_pos.x >= 0 && _mousePos.x - m_pos.x <= m_size.x + 10 &&
		_mousePos.y - m_pos.y >= 0 && _mousePos.y - m_pos.y <= m_size.y) {
		m_scroll.y -= GMouse::getMouseScroll() * 8;
		p_interactFlags -= (Sint8)EventFlag::MOUSESCROLL;
	}
	if(m_rHeld != 0) {
		if(m_minScroll.x != 0 || m_maxScroll.x != 0)
			m_scroll.x += _mousePos.x - m_mousePos.x;
		if(m_minScroll.y != 0 || m_maxScroll.y != 0)
			m_scroll.y += _mousePos.y - m_mousePos.y;
	}

	if(m_scroll.x < m_minScroll.x)
		m_scroll.x = m_minScroll.x;
	if(m_scroll.y < m_minScroll.y)
		m_scroll.y = m_minScroll.y;

	if(m_scroll.x > m_maxScroll.x)
		m_scroll.x = m_maxScroll.x;
	if(m_scroll.y > m_maxScroll.y)
		m_scroll.y = m_maxScroll.y;

	if((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x - m_pos.x >= 0 && _mousePos.x - m_pos.x <= m_size.x &&
		_mousePos.y - m_pos.y >= 0 && _mousePos.y - m_pos.y <= m_size.y)
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;

	// Drag window -- disabled
	if(((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) || m_lHeld) && m_draggable) {
		if(m_lHeld != 0)
			m_pos = m_pos + (_mousePos - m_mousePos);
		if(_mousePos.x - m_pos.x >= 0 && _mousePos.x - m_pos.x <= m_size.x && 
			_mousePos.y - m_pos.y >= 0 && _mousePos.y - m_pos.y <= m_size.y) {
			if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
				m_lHeld = 2;
			else if(!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
				m_lHeld = 0;
				if(m_pos.x < 0)
					m_pos.x = 0;
				else if(m_pos.x > Sint32(GScreen::m_screenSize.x - m_size.x))
					m_pos.x = Sint32(GScreen::m_screenSize.x - m_size.x);
				if(m_pos.y > 0)
					m_pos.y = 0;
				else if(m_pos.y < -Sint32(GScreen::m_screenSize.y - m_size.y) + 24)
					m_pos.y = -Sint32(GScreen::m_screenSize.y - m_size.y) + 24;
			}
		}
		else {
			if(!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
				m_lHeld = 0;
				if(m_pos.x < 0)
					m_pos.x = 0;
				else if(m_pos.x > Sint32(GScreen::m_screenSize.x - m_size.x))
					m_pos.x = Sint32(GScreen::m_screenSize.x - m_size.x);
				if(m_pos.y > 0)
					m_pos.y = 0;
				else if(m_pos.y < -Sint32(GScreen::m_screenSize.x - m_size.x) + 24)
					m_pos.y = -Sint32(GScreen::m_screenSize.x - m_size.x) + 24;
			}
		}
	}
	if(m_scrollY && 
		_mousePos.x - m_pos.x >= m_size.x && _mousePos.x - m_pos.x <= m_size.x + 10 && 
		_mousePos.y - m_pos.y >= 0 && _mousePos.y - m_pos.y <= m_size.y) {
		if(m_scrollY) {
			if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
				m_rHeld = 2;
			else if(!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT))
				m_rHeld = 0;
		}
		if((p_interactFlags & (Sint8)EventFlag::MOUSEOVER))
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
	else {
		if(!(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)))
			m_rHeld = 0;
		else
			if(m_rHeld && (p_interactFlags & (Sint8)EventFlag::MOUSEOVER))
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
	m_mousePos = _mousePos;
}
void ContainerPanel::update(GLfloat p_updateTime) {
	Container::update(p_updateTime);
}
void ContainerPanel::render() {
	if(m_visible) {
		MScissor::getInstance().toggle();
		m_panel->render();
		MScissor::getInstance().toggle();
		MScissor::getInstance().push(Rect(GLfloat(m_pos.x + 1), GLfloat(m_pos.y), GLfloat(m_size.x - 2), GLfloat(m_size.y - 1)));
		glPushMatrix();
		{
			glTranslatef(-GLfloat(m_scroll.x), -GLfloat(m_scroll.y), 0);
			Container::render();
		}
		glPopMatrix();
		MScissor::getInstance().pop();
		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			Vector2<Sint32> _scrollDist = m_maxScroll - m_minScroll;
			glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
			if(m_scrollX)
			{
				m_colorTheme.m_border.useColor();
				glBegin(GL_QUADS);
				{
					glVertex2f(0, GLfloat(m_size.y));
					glVertex2f(0, GLfloat(m_size.y + 10));
					glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y + 10));
					glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
				}
				glEnd();
				m_colorTheme.m_primary.useColor();
			}
			if(m_scrollY) {
				m_colorTheme.m_border.useColor();
				glBegin(GL_QUADS);
				{
					glVertex2f(GLfloat(m_size.x), 0);
					glVertex2f(GLfloat(m_size.x + 10), 0);
					glVertex2f(GLfloat(m_size.x + 10), GLfloat(m_size.y));
					glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
				}
				glEnd();
				m_colorTheme.m_primary.useColor();
				glBegin(GL_QUADS);
				{
					glVertex2f(GLfloat(m_size.x + 1), 
						(GLfloat(m_scroll.y - m_minScroll.y) / _scrollDist.y) * (m_size.y * _scrollDist.y) / (m_size.y + _scrollDist.y) + 1);
					glVertex2f(GLfloat(m_size.x + 9), 
						(GLfloat(m_scroll.y - m_minScroll.y) / _scrollDist.y) * (m_size.y * _scrollDist.y) / (m_size.y + _scrollDist.y) + 1);
					glVertex2f(GLfloat(m_size.x + 9), 
						(GLfloat(m_scroll.y - m_minScroll.y) / _scrollDist.y) * (m_size.y * _scrollDist.y) / (m_size.y + _scrollDist.y) + ((m_size.y * m_size.y) / (m_size.y + _scrollDist.y)) - 1);
					glVertex2f(GLfloat(m_size.x + 1), 
						(GLfloat(m_scroll.y - m_minScroll.y) / _scrollDist.y) * (m_size.y * _scrollDist.y) / (m_size.y + _scrollDist.y) + ((m_size.y * m_size.y) / (m_size.y + _scrollDist.y)) - 1);
				}
				glEnd();
			}
		}
		glPopMatrix();
	}
}
