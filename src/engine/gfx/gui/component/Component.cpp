#include "engine\gfx\gui\component\Component.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\gui\global\GGui.h"
#include "engine\utils\directory\LDirectory.h"

std::map<std::string, Color> Component::m_colorThemeMap;

Component::Component() {

}
Component::Component(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size) {
	m_compName = p_compName;
	m_title = p_title;
	m_posInit = m_pos = p_pos;
	m_sizeInit = m_size = p_size;
	m_texture = 0;
}
Component::~Component() {

}

void Component::init() {
	loadTheme(); 
}
void Component::loadTheme() {
	std::ifstream themefile(LDirectory::getProjectPath() + "res\\config\\ColorTheme.ini");
	if (themefile.good()) {
		m_colorThemeMap.clear();
		m_colorThemeMap.insert({ "NULL", Color() });
		std::string line = "";
		std::string first, second;
		std::stringstream ss;
		size_t equalPos;
		while (!themefile.eof()) {
			std::getline(themefile, line);
			if (line.empty() || line.at(0) == '#') continue;
			equalPos = line.find_first_of('=');
			first = line.substr(0, equalPos);
			second = line.substr(equalPos + 1);

			m_colorThemeMap.insert({ first, Color(second) });

			line = "";
		}
	}
	themefile.close();

	Color& bg = m_colorThemeMap.at("modelBackground");
	glClearColor(bg.r, bg.g, bg.b, 1.f);
}
void Component::terminate() {
	m_colorThemeMap.clear();
}

Component* Component::addComponent(Component* p_comp, Anchor p_posAnchor, Anchor p_sizeAnchor) { return this; }
Component* Component::findComponent(std::string p_compName) { return this; }

Color& Component::getElementColor(std::string p_element) {
	return m_colorThemeMap.at(p_element);
}
Color& Component::getPrimaryColor() {
	if (getElementPos() != "") {
		return getElementColor(getElementPos() + "Primary");
	}
	if (m_parentContainer) {
		return getElementColor(m_parentContainer->getElementPos() + "Primary");
	}
	return getElementColor("NULL");
}
std::string Component::getElementPos() {
	if (m_parentContainer != 0 && m_elementPos == "") {
		return m_parentContainer->getElementPos();
	}
	return m_elementPos;
}
void Component::setElementPos(std::string p_pp) {
	m_elementPos = p_pp;
}
Component* Component::addItem(std::string p_item) { return this; }
Uint16 Component::getItemCount() { return 0; }
void Component::setList(std::vector<std::string> p_items) { }

bool Component::exists() { return true; }

Component* Component::setPressFunction(function p_func) {
	m_pressFunction = p_func;
	return this;
}
Component* Component::setHoldFunction(function p_func) {
	m_holdFunction = p_func;
	return this;
}
Component* Component::setReleaseFunction(function p_func) {
	m_releaseFunction = p_func;
	return this;
}
Component* Component::setDoubleclickFunction(function p_func) {
	m_doubleclickFunction = p_func;
	return this;
}
Component* Component::callPressFunction() {
	if (m_pressFunction) m_pressFunction();
	return this;
}
Component* Component::callHoldFunction() {
	if (m_holdFunction) m_holdFunction();
	return this;
}
Component* Component::callReleaseFunction() {
	if (m_releaseFunction) m_releaseFunction();
	return this;
}
Component* Component::callDoubleclickFunction() {
	if (m_doubleclickFunction) m_doubleclickFunction();
	return this;
}

void Component::prevComponent() {
	if (m_prevComp) {
		m_selected = 0;
		m_prevComp->setState(1);
	}
}
void Component::nextComponent() {
	if (m_nextComp) {
		m_selected = 0;
		m_nextComp->setState(1);
	}
}

void Component::setSelectedItem(Uint16 p_selectedItem) { }
void Component::setSelectedButton(Uint16 p_selectedButton) { }
Uint8 Component::isUpdated() { return 0; }
Sint16 Component::getSelectedItem() { return 0; }
Sint16 Component::getPrevSelectedItem() { return 0; }
std::string Component::getName() { return m_compName; }
std::string Component::getTitle() { return m_title; }

void Component::setTooltip(std::string p_tooltip) { m_tooltip = p_tooltip; }
void Component::addTooltip() {
	if (!GMouse::mouseMoved() || m_tooltipTime > 0) {
		m_tooltipCounted = true;
		m_tooltipTime += GScreen::getDeltaUpdate();
	}
	else {
		m_tooltipTime = 0;
	}
	if (m_tooltipTime > 0.15f) {
		GGui::setTooltip(m_tooltip, GMouse::getMousePos());
	}
}
void Component::resetTooltip() {
	if (!m_tooltipCounted) {
		m_tooltipTime = 0;
	}
	m_tooltipCounted = false;
}

void Component::resize() {}
void Component::setTitle(std::string p_title) { m_title = p_title; }
void Component::setPosition(Vector2<Sint32> p_pos) { m_pos = p_pos; }
void Component::setSize(Vector2<Sint32> p_size) { m_size = p_size; resize(); }
Vector2<Sint32> Component::getInitialPosition() { return m_posInit; }
Vector2<Sint32> Component::getInitialSize() { return m_sizeInit; }
Vector2<Sint32> Component::getPosition() { return m_pos; }
Vector2<Sint32> Component::getSize() { return m_size; }
Vector2<Sint32> Component::getRealPosition() { return m_pos; }
Vector2<Sint32> Component::getRealSize() { return m_size; }
void Component::setState(Sint8 p_selected) { m_selected = p_selected; }

void Component::setHovered(bool p_hovered) {
	m_hovered = p_hovered;
}

Component* Component::setVisibleFunction(std::function<bool()> p_visible) {
	m_visible = p_visible;
	return this;
}

void Component::input() { }
void Component::input(Sint8& p_interactFlags) { }
void Component::update(GLfloat p_deltaUpdate) {
	if (m_hovered) {
		m_hoverTimer += p_deltaUpdate * 8;
		if (m_hoverTimer > 1) {
			m_hoverTimer = 1;
		}
	}
	else {
		m_hoverTimer -= p_deltaUpdate * 8;
		if (m_hoverTimer < 0) {
			m_hoverTimer = 0;
		}
	}
}
void Component::renderBack() {

}
void Component::renderFill(bool p_setColor) {
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	if (p_setColor) GBuffer::setColor(getPrimaryColor());
	GBuffer::setTexture(0);
	GBuffer::addVertexQuad(0, 0);
	GBuffer::addVertexQuad(m_size.x, 0);
	GBuffer::addVertexQuad(m_size.x, m_size.y);
	GBuffer::addVertexQuad(0, m_size.y);
	if (m_highlighting && (isSelected() || m_hoverTimer > 0)) {
		if (isSelected())			GBuffer::setColor(getElementColor(getElementPos() + "ActionPressed").applyScale(1.f, 1.f, 1.f, 0.5f));
		else if (m_hoverTimer > 0)	GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered").applyScale(1.f, 1.f, 1.f, m_hoverTimer));
		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(m_size.x, 0);
		GBuffer::addVertexQuad(m_size.x, m_size.y);
		GBuffer::addVertexQuad(0, m_size.y);
	}
	if (m_texture) {
		GBuffer::renderTexture(m_texture, {}, m_size, m_textureStyle);
	}
	Shader::popMatrixModel();
}
void Component::renderBorder() {
	if (m_border == 0) return;
	GBuffer::setTexture(0);
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	GBuffer::setColor(getElementColor(getElementPos() + (isFocused() ? "BorderFocused" : "BorderUnfocused")));
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
void Component::renderShadow() {

}
void Component::render() {
	//renderBack();
	renderFill();
	renderBorder();
}
Sint8 Component::isSelected() {
	return m_selected;
}

void Component::setValue(GLfloat p_value) { m_numValue = p_value; }
GLfloat Component::getValue() { return m_numValue; }
GLfloat Component::getPriorityLayer() { return m_priority + m_moveToFront; }
bool Component::hasList() { return false; }
Component* Component::setPriorityLayer(GLfloat p_priority) {
	m_priority = p_priority;
	return this;
}
