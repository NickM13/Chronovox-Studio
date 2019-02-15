#include "engine\gfx\gui\component\dialog\Dialog.h"
#include "engine\gfx\gui\component\button\Button.h"
#include "engine\gfx\gui\component\container\ColorPanel.h"
#include "engine\gfx\gui\component\container\Panel.h"

CDialog::CDialog(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Container(p_compName, p_pos, p_size, true) {
	setPriorityLayer(100);
	m_optionBorder = 10;
	addComponent(new Panel("WINDOW", p_title, { 0, 0 }, { 0, 0 }, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::ALL), Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT);
}

void CDialog::setActive(bool p_active) {
	setPosition(GScreen::m_screenSize / 2 - (getSize() / 2));
	resize();
	m_active = p_active;
}
bool CDialog::isActive() {
	return m_active;
}

CDialog* CDialog::addOption(std::string p_title, std::string p_description, function p_func) {
	Sint32 mw = Font::getMessageWidth(p_title).x;
	addComponent(new CButton(p_title, p_title, Vector2<Sint32>(-m_currWidth - m_optionBorder, (Font::getHeight() + m_optionBorder) * 0.7f), Vector2<Sint32>(mw + m_optionBorder, Font::getHeight() + m_optionBorder), CButton::RenderStyle::ALL, p_func), Component::Anchor::BOTTOM_RIGHT)->setTooltip(p_description);
	m_currWidth += (mw + m_optionBorder * 2);
	return this;
}
CDialog* CDialog::setOptionFunc(std::string p_title, function p_func) {
	findComponent(p_title)->setReleaseFunction(p_func);
	return this;
}

void CDialog::input(Sint8& p_interactFlags) {
	if (GKey::keyPressed(GLFW_KEY_ESCAPE)) m_active = false;
	Container::input(p_interactFlags);
}
void CDialog::update(GLfloat p_updateTime) {
	Container::update(p_updateTime);
}
void CDialog::render() {
	Container::render();
}
