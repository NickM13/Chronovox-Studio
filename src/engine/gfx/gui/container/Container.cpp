#include "engine\gfx\gui\container\Container.h"
#include "engine\gfx\gui\dialog\Dialog.h"
#include "engine\gfx\gui\container\ColorPanel.h"

Container::Container(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_visible)
	: Component(p_compName, "", p_pos, p_size, Theme::PRIMARY) {
	m_visible = p_visible;
	m_contentArea = Vector4<Sint32>();
	addComponent(new SColorPanel("PAUSE_BACKGROUND", {0, 0}, p_size, Color(0, 0, 0, 0.5f)), Anchor::NONE, Anchor::BOTTOM_RIGHT)->setPriorityLayer(99)->setVisible(false);
}

void Container::sortInComponent(Comp p_comp) {
	Component* _component = p_comp.m_component;
	for(Uint16 i = 0; i < m_componentOrder.size(); i++)
		if(_component->getPriorityLayer() < m_componentMap[m_componentOrder[i]].m_component->getPriorityLayer()) {
			m_componentOrder.insert(m_componentOrder.begin() + i, _component->getName());
			return;
		}
	m_componentOrder.push_back(_component->getName());
}

void Container::anchorComponent(Component* p_component, Anchor p_posAnchor, Anchor p_sizeAnchor) {
	Vector2<Sint32> pos, size;
	if((Sint8)p_posAnchor & (Sint8)BAnchor::RIGHT)
		pos.x = p_component->getInitialPosition().x + (m_size.x - p_component->getInitialSize().x);
	else if((Sint8)p_posAnchor & (Sint8)BAnchor::CENTER)
		pos.x = p_component->getInitialPosition().x + (m_size.x - p_component->getInitialSize().x) / 2;
	else // Anchor::LEFT and default
		pos.x = p_component->getInitialPosition().x;
	if((Sint8)p_posAnchor & (Sint8)BAnchor::BOTTOM)
		pos.y = p_component->getInitialPosition().y + (m_size.y - p_component->getInitialSize().y);
	else if((Sint8)p_posAnchor & (Sint8)BAnchor::MIDDLE)
		pos.y = p_component->getInitialPosition().y + (m_size.y - p_component->getInitialSize().y) / 2;
	else // Anchor::TOP and default
		pos.y = p_component->getInitialPosition().y;
	p_component->setPosition(pos);

	if((Sint8)p_sizeAnchor & (Sint8)BAnchor::RIGHT)
		size.x = m_size.x - p_component->getPosition().x + p_component->getInitialSize().x;
	else if((Sint8)p_sizeAnchor & (Sint8)BAnchor::CENTER)
		size.x = m_size.x / 2 - p_component->getPosition().x + p_component->getInitialSize().x;
	else // Anchor::LEFT and default
		size.x = p_component->getInitialSize().x;
	if((Sint8)p_sizeAnchor & (Sint8)BAnchor::BOTTOM)
		size.y = m_size.y - p_component->getPosition().y + p_component->getInitialSize().y;
	else if((Sint8)p_sizeAnchor & (Sint8)BAnchor::MIDDLE)
		size.y = m_size.y / 2 - p_component->getPosition().y + p_component->getInitialSize().y;
	else // Anchor::TOP and default
		size.y = p_component->getInitialSize().y;
	p_component->setSize(size);
	p_component->setPosition(p_component->getPosition() + getPosition());
	p_component->resize();
}

Component* Container::addComponent(Component* p_component, Anchor p_posAnchor, Anchor p_sizeAnchor) {
	anchorComponent(p_component, p_posAnchor, p_sizeAnchor);
	if(m_componentMap.empty())
		m_contentArea = Vector4<Sint32>(p_component->getRealPosition().x, p_component->getRealPosition().y, p_component->getRealPosition().x + p_component->getSize().x, p_component->getRealPosition().y + p_component->getSize().y);
	else if(p_component->isVisible())
		m_contentArea = Vector4<Sint32>(std::fminf(p_component->getRealPosition().x, m_contentArea.x1), std::fminf(p_component->getRealPosition().y, m_contentArea.y1), std::fmaxf(p_component->getRealPosition().x + p_component->getRealSize().x, m_contentArea.x2), std::fmaxf(p_component->getRealPosition().y + p_component->getRealSize().y, m_contentArea.y2));
	Comp c = Comp(p_posAnchor, p_sizeAnchor, p_component);
	m_componentMap.insert({p_component->getName(), c});
	sortInComponent(c);
	return p_component;
}
Container::~Container() {
	for(std::pair<std::string, Comp> c : m_componentMap)
		delete c.second.m_component;
	m_componentMap.clear();
}

Container::Component* Container::findComponent(std::string p_compName) {
	Uint16 _loc = (Uint16)p_compName.find('\\');
	std::string _compName = p_compName.substr(0, _loc);
	Comp _comp = m_componentMap[_compName];
	if(_loc < p_compName.length() && _comp.m_component)
		return _comp.m_component->findComponent(p_compName.substr(_loc + 1));
	else
		return _comp.m_component;
}
void Container::removeComponent(std::string p_compName) {
	Comp _comp = m_componentMap[p_compName];
	if(_comp.m_component) {
		delete _comp.m_component;
		m_componentMap.erase(p_compName);
	}
	for(Sint16 i = (Sint16) m_componentOrder.size() - 1; i >= 0; i--) {
		if(m_componentOrder[i] == p_compName) {
			m_componentOrder.erase(m_componentOrder.begin() + i);
		}
	}
}

Component* Container::setVisible(bool p_visible) {
	m_visible = p_visible;
	if(!m_visible)
		for(std::pair<std::string, Comp> comp : m_componentMap)
			comp.second.m_component->update(0);
	return this;
}

void Container::resize() {
	m_contentArea = {};
	for(std::pair<std::string, Comp> c : m_componentMap) {
		Comp comp = c.second;
		Component* component = comp.m_component;
		anchorComponent(component, comp.m_posAnchor, comp.m_sizeAnchor);
		if(m_componentMap.empty())
			m_contentArea = Vector4<Sint32>(component->getRealPosition().x, component->getRealPosition().y, component->getRealPosition().x + component->getSize().x, component->getRealPosition().y + component->getSize().y);
		else if(component->isVisible())
			m_contentArea = Vector4<Sint32>(std::fminf(component->getRealPosition().x, m_contentArea.x1), std::fminf(component->getRealPosition().y, m_contentArea.y1), std::fmaxf(component->getRealPosition().x + component->getRealSize().x, m_contentArea.x2), std::fmaxf(component->getRealPosition().y + component->getRealSize().y, m_contentArea.y2));
		
	}
}
Vector2<Sint32> Container::getRealPosition() { return Vector2<Sint32>(m_contentArea.x1, m_pos.y + m_contentArea.y1); }
Vector2<Sint32> Container::getRealSize() { return Vector2<Sint32>(m_contentArea.x2 - m_contentArea.x1, m_contentArea.y2 - m_contentArea.y1); }

Component* Container::addPauseScreen(Component* p_comp, Anchor p_posAnchor, Anchor p_sizeAnchor) {
	m_pauseScreens.insert({p_comp->getName(), addComponent(p_comp, p_posAnchor, p_sizeAnchor)});
	return p_comp;
}
Component* Container::setPauseScreen(std::string p_compName) {
	if(m_currentPause != "")
		m_pauseScreens[m_currentPause]->setVisible(false);
	SColorPanel* p = (SColorPanel*)findComponent("PAUSE_BACKGROUND");
	if(p_compName != "") {
		Component *c = m_pauseScreens[p_compName];
		if(c) {
			m_currentPause = p_compName;
			c->setVisible(true);
			c->callPressFunction();
			p->setVisible(true);
			return c;
		}
	}
	p->setVisible(false);
	m_currentPause = "";
	return 0;
}

void Container::updateSize() {
	m_contentArea = Vector4<Sint32>();
	for(std::pair<std::string, Comp> comp : m_componentMap) {
		if(comp.second.m_component->isVisible())
			m_contentArea = Vector4<Sint32>(std::fminf(comp.second.m_component->getRealPosition().x, m_contentArea.x1), std::fminf(comp.second.m_component->getRealPosition().y, m_contentArea.y1), std::fmaxf(comp.second.m_component->getRealPosition().x + comp.second.m_component->getRealSize().x, m_contentArea.x2), std::fmaxf(comp.second.m_component->getRealPosition().y + comp.second.m_component->getRealSize().y, m_contentArea.y2));
	}
}

void Container::input() {
	Sint8 interactFlags = 0;
	input(interactFlags);
}
void Container::input(Sint8& p_interactFlags) {
	if(m_visible) {
		Vector2<Sint32> _mousePos = _mousePos - m_pos;;
		Component *_comp;
		for(Sint32 i = m_componentOrder.size() - 1; i >= 0; i--) {
			_comp = m_componentMap[m_componentOrder[i]].m_component;
			if(_comp->isVisible())
				_comp->input(p_interactFlags);
		}
	}
	if(m_currentPause != "" &&
		!m_pauseScreens[m_currentPause]->isVisible()) {
		setPauseScreen("");
	}
}
void Container::update(GLfloat p_updateTime) {
	if(m_visible) {
		Component *_comp;
		for(Sint32 i = 0; i < (Sint32) m_componentOrder.size(); i++) {
			_comp = m_componentMap[m_componentOrder[i]].m_component;
			if(_comp->isVisible()) {
				_comp->update(p_updateTime);
				_comp->resetTooltip(); // TODO: Do I need this here?
			}
		}
	}
}
void Container::render() {
	if(m_visible) {
		Component *_comp;
		for(Sint32 i = 0; i < (Sint32) m_componentOrder.size(); i++) {
			_comp = m_componentMap[m_componentOrder[i]].m_component;
			if(_comp->isVisible()) {
				glPushMatrix();
					_comp->render();
				glPopMatrix();
			}
		}
	}
}
