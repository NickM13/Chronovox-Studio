#include "engine\gfx\gui\container\Container.h"

Container::Container(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_visible)
	: Component(p_compName, "", p_pos, p_size, 0)
{
	m_visible = p_visible;
	m_contentArea = Vector4<Sint32>();
}

void Container::sortInComponent(Comp p_comp)
{
	Component* _component = p_comp.m_component;
	for(Uint16 i = 0; i < m_componentOrder.size(); i++)
		if(_component->getPriorityLayer() < m_componentMap[m_componentOrder[i]].m_component->getPriorityLayer()) {
			m_componentOrder.insert(m_componentOrder.begin() + i, _component->getName());
			return;
		}
	m_componentOrder.push_back(_component->getName());
}

Component* Container::addComponent(Component* p_component, Anchor p_posAnchor, Anchor p_sizeAnchor)
{
	switch(p_posAnchor) {
	case NOANCHOR:
		p_component->setPosition(p_component->getPosition());
		break;
	case CENTER:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x) / 2, Sint32(m_size.y - p_component->getSize().y) / 2));
		break;
	case LEFT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(0, Sint32(m_size.y - p_component->getSize().y) / 2));
		break;
	case RIGHT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x), Sint32(m_size.y - p_component->getSize().y) / 2) + Vector2<Sint32>(Sint32(p_component->getPosition().x * -2), 0));
		break;
	case TOP:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x) / 2, 0));
		break;
	case BOTTOM:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x) / 2, Sint32(m_size.y - p_component->getSize().y)) + Vector2<Sint32>(0, Sint32(p_component->getPosition().y * -2)));
		break;
	case TOP_LEFT:
		p_component->setPosition(p_component->getPosition());
		break;
	case TOP_RIGHT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x), 0) + Vector2<Sint32>(Sint32(p_component->getPosition().x * -2), 0));
		break;
	case BOTTOM_LEFT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(0, Sint32(m_size.y - p_component->getSize().y)) + Vector2<Sint32>(0, Sint32(p_component->getPosition().y * -2)));
		break;
	case BOTTOM_RIGHT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x), Sint32(m_size.y - p_component->getSize().y)) + Vector2<Sint32>(Sint32(p_component->getPosition().x * -2), Sint32(p_component->getPosition().y * -2)));
		break;
	}
	switch(p_sizeAnchor) {
	case NOANCHOR:
		p_component->setSize(p_component->getSize());
		break;
	case CENTER:
		p_component->setSize(Vector2<Sint32>((m_size.x - p_component->getPosition().x) / 2 + p_component->getSize().x, (m_size.y - p_component->getPosition().y) / 2 + p_component->getSize().y));
		break;
	case LEFT:
		p_component->setSize(Vector2<Sint32>(p_component->getSize().x, (m_size.y - p_component->getPosition().y) / 2 + p_component->getSize().y));
		break;
	case RIGHT:
		p_component->setSize(Vector2<Sint32>(m_size.x - p_component->getPosition().x + p_component->getSize().x, (m_size.y - p_component->getPosition().y) / 2 + p_component->getSize().y));
		break;
	case TOP:
		p_component->setSize(Vector2<Sint32>((m_size.x - p_component->getPosition().x) / 2 + p_component->getSize().x, p_component->getSize().y));
		break;
	case BOTTOM:
		p_component->setSize(Vector2<Sint32>((m_size.x - p_component->getPosition().x) / 2 + p_component->getSize().x, m_size.y - p_component->getPosition().y + p_component->getSize().y));
		break;
	case TOP_LEFT:
		p_component->setSize(p_component->getSize());
		break;
	case TOP_RIGHT:
		p_component->setSize(Vector2<Sint32>(m_size.x + p_component->getPosition().x + p_component->getSize().x, p_component->getSize().y));
		break;
	case BOTTOM_LEFT:
		p_component->setSize(Vector2<Sint32>(p_component->getSize().x, m_size.y - p_component->getPosition().y + p_component->getSize().y));
		break;
	case BOTTOM_RIGHT:
		p_component->setSize(Vector2<Sint32>(m_size.x - p_component->getPosition().x + p_component->getSize().x, m_size.y - p_component->getPosition().y + p_component->getSize().y));
		break;
	}
	if(m_componentMap.empty())
		m_contentArea = Vector4<Sint32>(p_component->getRealPosition().x, p_component->getRealPosition().y, p_component->getRealPosition().x + p_component->getSize().x, p_component->getRealPosition().y + p_component->getSize().y);
	else if(p_component->isVisible())
		m_contentArea = Vector4<Sint32>(min(p_component->getRealPosition().x, m_contentArea.x1), min(p_component->getRealPosition().y, m_contentArea.y1), max(p_component->getRealPosition().x + p_component->getRealSize().x, m_contentArea.x2), max(p_component->getRealPosition().y + p_component->getRealSize().y, m_contentArea.y2));
	Comp c = Comp(p_posAnchor, p_sizeAnchor, p_component);
	m_componentMap.insert({p_component->getName(), c});
	sortInComponent(c);
	return p_component;
}
Container::~Container()
{
	for(std::pair<std::string, Comp> c : m_componentMap) {
		delete c.second.m_component;
	}
	m_componentMap.clear();
}

Container::Component* Container::findComponent(std::string p_compName)
{
	Uint16 _loc = (Uint16)p_compName.find('\\');
	std::string _compName = p_compName.substr(0, _loc);
	Comp _comp = m_componentMap[_compName];
	if(_loc < p_compName.length() && _comp.m_component)
		return _comp.m_component->findComponent(p_compName.substr(_loc + 1));
	else
		return _comp.m_component;
}
void Container::removeComponent(std::string p_compName)
{
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

Component* Container::setVisible(bool p_visible)
{
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
		switch(comp.m_posAnchor)
		{
		case NOANCHOR:
			component->setPosition(component->getInitialPosition());
			break;
		case CENTER:
			component->setPosition(component->getInitialPosition() + Vector2<Sint32>(Sint32(m_size.x - component->getInitialSize().x) / 2, Sint32(m_size.y - component->getInitialSize().y) / 2));
			break;
		case LEFT:
			component->setPosition(component->getInitialPosition() + Vector2<Sint32>(0, Sint32(m_size.y - component->getInitialSize().y) / 2));
			break;
		case RIGHT:
			component->setPosition(component->getInitialPosition() + Vector2<Sint32>(Sint32(m_size.x - component->getInitialSize().x), Sint32(m_size.y - component->getInitialSize().y) / 2) + Vector2<Sint32>(Sint32(component->getInitialPosition().x * -2), 0));
			break;
		case TOP:
			component->setPosition(component->getInitialPosition() + Vector2<Sint32>(Sint32(m_size.x - component->getInitialSize().x) / 2, 0));
			break;
		case BOTTOM:
			component->setPosition(component->getInitialPosition() + Vector2<Sint32>(Sint32(m_size.x - component->getInitialSize().x) / 2, Sint32(m_size.y - component->getInitialSize().y)) + Vector2<Sint32>(0, Sint32(component->getInitialPosition().y * -2)));
			break;
		case TOP_LEFT:
			component->setPosition(component->getInitialPosition());
			break;
		case TOP_RIGHT:
			component->setPosition(component->getInitialPosition() + Vector2<Sint32>(Sint32(m_size.x - component->getInitialSize().x), 0) + Vector2<Sint32>(Sint32(component->getInitialPosition().x * -2), 0));
			break;
		case BOTTOM_LEFT:
			component->setPosition(component->getInitialPosition() + Vector2<Sint32>(0, Sint32(m_size.y - component->getInitialSize().y)) + Vector2<Sint32>(0, Sint32(component->getInitialPosition().y * -2)));
			break;
		case BOTTOM_RIGHT:
			component->setPosition(component->getInitialPosition() + Vector2<Sint32>(Sint32(m_size.x - component->getInitialSize().x), Sint32(m_size.y - component->getInitialSize().y)) + Vector2<Sint32>(Sint32(component->getInitialPosition().x * -2), Sint32(component->getInitialPosition().y * -2)));
			break;
		}
		switch(comp.m_sizeAnchor) {
		case NOANCHOR:
			component->setSize(component->getInitialSize());
			break;
		case CENTER:
			component->setSize(Vector2<Sint32>((m_size.x - component->getPosition().x) / 2 + component->getInitialSize().x, (m_size.y - component->getPosition().y) / 2 + component->getInitialSize().y));
			break;
		case LEFT:
			component->setSize(Vector2<Sint32>(component->getInitialSize().x, (m_size.y - component->getPosition().y) / 2 + component->getInitialSize().y));
			break;
		case RIGHT:
			component->setSize(Vector2<Sint32>(m_size.x - component->getPosition().x + component->getInitialSize().x, (m_size.y - component->getPosition().y) / 2 + component->getInitialSize().y));
			break;
		case TOP:
			component->setSize(Vector2<Sint32>((m_size.x - component->getPosition().x) / 2 + component->getInitialSize().x, component->getInitialSize().y));
			break;
		case BOTTOM:
			component->setSize(Vector2<Sint32>((m_size.x - component->getPosition().x) / 2 + component->getInitialSize().x, m_size.y - component->getPosition().y + component->getInitialSize().y));
			break;
		case TOP_LEFT:
			component->setSize(component->getInitialSize());
			break;
		case TOP_RIGHT:
			component->setSize(Vector2<Sint32>(m_size.x - component->getPosition().x + component->getInitialSize().x, component->getInitialSize().y));
			break;
		case BOTTOM_LEFT:
			component->setSize(Vector2<Sint32>(component->getInitialSize().x, m_size.y - component->getPosition().y + component->getInitialSize().y));
			break;
		case BOTTOM_RIGHT:
			component->setSize(Vector2<Sint32>(m_size.x - component->getPosition().x + component->getInitialSize().x, m_size.y - component->getPosition().y + component->getInitialSize().y));
			break;
		}
		if(m_componentMap.empty())
			m_contentArea = Vector4<Sint32>(component->getRealPosition().x, component->getRealPosition().y, component->getRealPosition().x + component->getSize().x, component->getRealPosition().y + component->getSize().y);
		else if(component->isVisible())
			m_contentArea = Vector4<Sint32>(min(component->getRealPosition().x, m_contentArea.x1), min(component->getRealPosition().y, m_contentArea.y1), max(component->getRealPosition().x + component->getRealSize().x, m_contentArea.x2), max(component->getRealPosition().y + component->getRealSize().y, m_contentArea.y2));
		component->resize();
	}
}
Vector2<Sint32> Container::getRealPosition() { return Vector2<Sint32>(m_contentArea.x1, m_pos.y + m_contentArea.y1); }
Vector2<Sint32> Container::getRealSize() { return Vector2<Sint32>(m_contentArea.x2 - m_contentArea.x1, m_contentArea.y2 - m_contentArea.y1); }

void Container::updateSize()
{
	m_contentArea = Vector4<Sint32>();
	for(std::pair<std::string, Comp> comp : m_componentMap) {
		if(comp.second.m_component->isVisible())
			m_contentArea = Vector4<Sint32>(min(comp.second.m_component->getRealPosition().x, m_contentArea.x1), min(comp.second.m_component->getRealPosition().y, m_contentArea.y1), max(comp.second.m_component->getRealPosition().x + comp.second.m_component->getRealSize().x, m_contentArea.x2), max(comp.second.m_component->getRealPosition().y + comp.second.m_component->getRealSize().y, m_contentArea.y2));
	}
}

void Container::input()
{
	Sint8 interactFlags = 0;
	Sint8* keyStates = GKey::m_keyStates;
	Sint8* mouseStates = GMouse::m_mouseStates;
	Vector2<Sint32> mousePos = GMouse::m_guiMousePos;
	input(interactFlags, keyStates, mouseStates, mousePos);
}
void Container::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	if(m_visible) {
		Vector2<Sint32> _mousePos = p_mousePos - m_pos;;
		Component *_comp;
		for(Sint32 i = m_componentOrder.size() - 1; i >= 0; i--) {
			_comp = m_componentMap[m_componentOrder[i]].m_component;
			if(_comp->isVisible())
				_comp->input(p_interactFlags, p_keyStates, p_mouseStates, _mousePos);
		}
	}
}
void Container::update(GLfloat p_updateTime)
{
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
void Container::render()
{
	if(m_visible) {
		Component *_comp;
		for(Sint32 i = 0; i < (Sint32) m_componentOrder.size(); i++) {
			_comp = m_componentMap[m_componentOrder[i]].m_component;
			if(_comp->isVisible()) {
				glPushMatrix();
					glTranslatef(getPosition().x, getPosition().y, 0);
					_comp->render();
				glPopMatrix();
			}
		}
	}
}
