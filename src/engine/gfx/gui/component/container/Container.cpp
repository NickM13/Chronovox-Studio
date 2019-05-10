#include "engine\gfx\gui\component\container\Container.h"
#include "engine\gfx\gui\component\dialog\Dialog.h"
#include "engine\gfx\gui\component\container\ColorPanel.h"

Container::Container(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, std::function<bool()> p_visible)
	: Component(p_compName, "", p_pos, p_size) {
	m_visible = p_visible;
	m_contentArea = Vector4<Sint32>();
	addComponent(new SColorPanel("PAUSE_BACKGROUND", { 0, 0 }, p_size, Color(0, 0, 0, 0.5f)), Anchor::NONE, Anchor::BOTTOM_RIGHT)->setPriorityLayer(99)->setVisibleFunction([]() {return false;});
	m_border = 0;
}

void Container::sortInComponent(Comp p_comp) {
	Component* _component = p_comp.m_component;
	for (Uint16 i = 0; i < m_componentOrder.size(); i++)
		if (_component->getPriorityLayer() < m_componentMap[m_componentOrder[i]].m_component->getPriorityLayer()) {
			m_componentOrder.insert(m_componentOrder.begin() + i, _component->getName());
			return;
		}
	m_componentOrder.push_back(_component->getName());
}

void Container::anchorComponent(Component* p_component, Anchor p_posAnchor, Anchor p_sizeAnchor) {
	Vector2<Sint32> pos, size;
	if ((Sint8)p_posAnchor & (Sint8)BAnchor::RIGHT) {
		pos.x = p_component->getInitialPosition().x + (m_size.x - p_component->getInitialSize().x);
	} else if ((Sint8)p_posAnchor & (Sint8)BAnchor::CENTER) {
		pos.x = p_component->getInitialPosition().x + (m_size.x - p_component->getInitialSize().x) / 2;
	} else {
		pos.x = p_component->getInitialPosition().x;
	}
	if ((Sint8)p_posAnchor & (Sint8)BAnchor::BOTTOM) {
		if ((Sint8)p_sizeAnchor & (Sint8)BAnchor::BOTTOM) {
			pos.y = p_component->getInitialPosition().y + m_size.y;
		} else {
			pos.y = p_component->getInitialPosition().y + (m_size.y - p_component->getInitialSize().y);
		}
	} else if ((Sint8)p_posAnchor & (Sint8)BAnchor::MIDDLE) {
		pos.y = p_component->getInitialPosition().y + (m_size.y - p_component->getInitialSize().y) / 2;
	} else {
		pos.y = p_component->getInitialPosition().y;
	}
	p_component->setPosition(pos);

	if ((Sint8)p_sizeAnchor & (Sint8)BAnchor::RIGHT)
		size.x = m_size.x - p_component->getPosition().x + p_component->getInitialSize().x;
	else if ((Sint8)p_sizeAnchor & (Sint8)BAnchor::CENTER)
		size.x = m_size.x / 2 - p_component->getPosition().x + p_component->getInitialSize().x;
	else // Anchor::LEFT and default
		size.x = p_component->getInitialSize().x;
	if ((Sint8)p_sizeAnchor & (Sint8)BAnchor::BOTTOM)
		size.y = m_size.y - p_component->getPosition().y + p_component->getInitialSize().y;
	else if ((Sint8)p_sizeAnchor & (Sint8)BAnchor::MIDDLE)
		size.y = m_size.y / 2 - p_component->getPosition().y + p_component->getInitialSize().y;
	else // Anchor::TOP and default
		size.y = p_component->getInitialSize().y;
	p_component->setSize(size);
	p_component->setPosition(p_component->getPosition() + getPosition());
	p_component->resize();
}

Component* Container::addComponent(Component* p_component, Anchor p_posAnchor, Anchor p_sizeAnchor) {
	p_component->setParent(this);
	anchorComponent(p_component, p_posAnchor, p_sizeAnchor);
	if (m_componentMap.empty()) {
		m_contentArea = Vector4<Sint32>(p_component->getRealPosition().x
			, p_component->getRealPosition().y
			, p_component->getRealPosition().x + p_component->getSize().x
			, p_component->getRealPosition().y + p_component->getSize().y);
	} else if (p_component->isVisible()) {
		m_contentArea = Vector4<Sint32>(std::fminf(p_component->getRealPosition().x, m_contentArea.x1)
			, std::fminf(p_component->getRealPosition().y, m_contentArea.y1)
			, std::fmaxf(p_component->getRealPosition().x + p_component->getRealSize().x, m_contentArea.x2)
			, std::fmaxf(p_component->getRealPosition().y + p_component->getRealSize().y, m_contentArea.y2));
	}
	Comp c = Comp(p_posAnchor, p_sizeAnchor, p_component);
	m_componentMap.insert({ p_component->getName(), c });
	sortInComponent(c);
	return p_component;
}
Container::~Container() {
	for (std::pair<std::string, Comp> c : m_componentMap) {
		delete c.second.m_component;
	}
	m_componentMap.clear();
}

Container::Component* Container::findComponent(std::string p_compName) {
	Uint16 _loc = (Uint16)p_compName.find('\\');
	std::string _compName = p_compName.substr(0, _loc);
	Comp _comp = m_componentMap[_compName];
	if (_loc < p_compName.length() && _comp.m_component) {
		return _comp.m_component->findComponent(p_compName.substr(_loc + 1));
	} else {
		return _comp.m_component;
	}
}
void Container::removeComponent(std::string p_compName) {
	Comp _comp = m_componentMap[p_compName];
	if (_comp.m_component) {
		delete _comp.m_component;
		m_componentMap.erase(p_compName);
	}
	for (Sint16 i = (Sint16)m_componentOrder.size() - 1; i >= 0; i--) {
		if (m_componentOrder[i] == p_compName) {
			m_componentOrder.erase(m_componentOrder.begin() + i);
		}
	}
}

Component* Container::setVisible(std::function<bool()> p_visible) {
	m_visible = p_visible;
	if (!m_visible) {
		for (std::pair<std::string, Comp> comp : m_componentMap) {
			comp.second.m_component->update(0);
		}
	}
	return this;
}

void Container::resize() {
	m_contentArea = {};
	for (std::pair<std::string, Comp> c : m_componentMap) {
		Comp comp = c.second;
		Component* component = comp.m_component;
		anchorComponent(component, comp.m_posAnchor, comp.m_sizeAnchor);
		if (m_componentMap.empty()) {
			m_contentArea = Vector4<Sint32>(component->getRealPosition().x, component->getRealPosition().y, component->getRealPosition().x + component->getSize().x, component->getRealPosition().y + component->getSize().y);
		} else if (component->isVisible()) {
			m_contentArea = Vector4<Sint32>(std::fminf(component->getRealPosition().x, m_contentArea.x1), std::fminf(component->getRealPosition().y, m_contentArea.y1), std::fmaxf(component->getRealPosition().x + component->getRealSize().x, m_contentArea.x2), std::fmaxf(component->getRealPosition().y + component->getRealSize().y, m_contentArea.y2));
		}
	}
}
Vector2<Sint32> Container::getRealPosition() { return Vector2<Sint32>(m_contentArea.x1, m_pos.y + m_contentArea.y1); }
Vector2<Sint32> Container::getRealSize() { return Vector2<Sint32>(m_contentArea.x2 - m_contentArea.x1, m_contentArea.y2 - m_contentArea.y1); }

Component* Container::openDialog(Component* p_dialog) {
	if (p_dialog) {
		m_currDialog = p_dialog;
		((CDialog*)m_currDialog)->setActive(true);
		m_currDialog->callPressFunction();
		findComponent("PAUSE_BACKGROUND")->setVisibleFunction([]() {return true;});
	}
	return this;
}
Component* Container::closeDialog() {
	m_currDialog->callReleaseFunction();
	m_currDialog = 0;
	findComponent("PAUSE_BACKGROUND")->setVisibleFunction([]() {return false;});
	return this;
}

void Container::updateSize() {
	m_contentArea = Vector4<Sint32>();
	for (std::pair<std::string, Comp> comp : m_componentMap) {
		if (comp.second.m_component->isVisible()) {
			m_contentArea = Vector4<Sint32>(std::fminf(comp.second.m_component->getRealPosition().x, m_contentArea.x1), std::fminf(comp.second.m_component->getRealPosition().y, m_contentArea.y1), std::fmaxf(comp.second.m_component->getRealPosition().x + comp.second.m_component->getRealSize().x, m_contentArea.x2), std::fmaxf(comp.second.m_component->getRealPosition().y + comp.second.m_component->getRealSize().y, m_contentArea.y2));
		}
	}
}

void Container::input() {
	Sint8 interactFlags = 0;
	input(interactFlags);
}
void Container::input(Sint8& p_interactFlags) {
	if (m_visible) {
		if (m_currDialog) {
			m_currDialog->input(p_interactFlags);
			p_interactFlags = 0;
		}
		Component* _comp;
		for (Sint32 i = m_componentOrder.size() - 1; i >= 0; i--) {
			_comp = m_componentMap[m_componentOrder[i]].m_component;
			if (_comp->isVisible()) {
				_comp->input(p_interactFlags);
			}
		}
	}
}
void Container::update(GLfloat p_updateTime) {
	if (m_visible) {
		if (m_currDialog) {
			m_currDialog->update(p_updateTime);
			if (!((CDialog*)m_currDialog)->isActive()) {
				closeDialog();
			}
		}
		Component* _comp;
		for (Sint32 i = 0; i < (Sint32)m_componentOrder.size(); i++) {
			_comp = m_componentMap[m_componentOrder[i]].m_component;
			if (_comp->isVisible()) {
				_comp->update(p_updateTime);
				//_comp->resetTooltip(); // TODO: Do I need this here?
			}
		}
	}
}
void Container::render() {
	if (m_visible) {
		Component* _comp;
		for (Sint32 i = 0; i < (Sint32)m_componentOrder.size(); i++) {
			_comp = m_componentMap[m_componentOrder[i]].m_component;
			if (_comp->isVisible()) {
				_comp->render();
			}
		}
		if (m_border != 0) {
			GBuffer::setTexture(0);
			Shader::pushMatrixModel();
			Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
			GBuffer::setColor(m_colorThemeMap.at("borderElementUnfocused"));
			if (m_border & static_cast<Sint8>(BorderFlag::TOP)) {
				GBuffer::addVertexQuad(0, 0);
				GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
				GBuffer::addVertexQuad(GLfloat(m_size.x), 1);
				GBuffer::addVertexQuad(0, 1);
			}
			if (m_border & static_cast<Sint8>(BorderFlag::RIGHT)) {
				GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
				GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
				GBuffer::addVertexQuad(GLfloat(m_size.x) - 1, GLfloat(m_size.y));
				GBuffer::addVertexQuad(GLfloat(m_size.x) - 1, 0);
			}
			if (m_border & static_cast<Sint8>(BorderFlag::BOTTOM)) {
				GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
				GBuffer::addVertexQuad(0, GLfloat(m_size.y));
				GBuffer::addVertexQuad(0, GLfloat(m_size.y) - 1);
				GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y) - 1);
			}
			if (m_border & static_cast<Sint8>(BorderFlag::LEFT)) {
				GBuffer::addVertexQuad(0, 0);
				GBuffer::addVertexQuad(0, GLfloat(m_size.y));
				GBuffer::addVertexQuad(1, GLfloat(m_size.y));
				GBuffer::addVertexQuad(1, 0);
			}
			Shader::popMatrixModel();
		}
		if (m_currDialog) {
			m_currDialog->render();
		}
	}
}
