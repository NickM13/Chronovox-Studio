#include "engine\gfx\gui\component\dialog\Dialog.h"
#include "engine\gfx\gui\component\button\Button.h"
#include "engine\gfx\gui\component\container\ColorPanel.h"
#include "engine\gfx\gui\component\container\Panel.h"

CDialog::CDialog(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Container(p_compName, p_pos, p_size, []() { return true; }) {
	setPriorityLayer(100);
	m_optionBorder = 20;
	m_optionWidth = 60;
	m_optionHeight = 28;
	Sint16 m_padding = 16;
	addComponent(new Panel("WINDOW", ""/*p_title*/, -m_padding - 1, m_padding + 1, (Sint8)Component::BorderFlag::ALL), Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setHighlightActive(true);
	addComponent(new CButton("BUTTON_CLOSE_DIALOG", "", MTexture::getTexture("gui\\icon\\window\\Close.png"),
		{ m_padding, -m_padding }, { 34, 28 }, CButton::RenderStyle::FILL, [&]() { setActive(false); }), Component::Anchor::TOP_RIGHT);
	setElementPos("dialog");
}

void CDialog::setActive(bool p_active) {
	setPosition(GScreen::getScreenSize() / 2 - (getSize() / 2));
	resize();
	m_active = p_active;
}
bool CDialog::isActive() {
	return m_active;
}

CDialog* CDialog::addOption(std::string p_title, std::string p_description, Sint32 p_funcIndex) {
	CButton* button = new CButton(p_title, p_title,
		Vector2<Sint32>(-m_currWidth, 0),
		Vector2<Sint32>(m_optionWidth + m_optionBorder, m_optionHeight),
		CButton::RenderStyle::ALL);
	addComponent(button, Component::Anchor::BOTTOM_RIGHT)
		->setFocused(true)
		->setTooltip(p_description);
	m_optionList.push_back({ button, p_funcIndex });
	m_currWidth += (m_optionWidth + m_optionBorder * 1.5);
	return this;
}

void CDialog::input(Sint8& p_interactFlags) {
	if (GKey::keyPressed(GLFW_KEY_ESCAPE)) m_active = false;
	Container::input(p_interactFlags);
	for (Option o : m_optionList) {
		if (o.button->isSelected() == 3) {
			if (o.function != -1) {
				m_functionList.at(o.function)();
			}
			setActive(false);
		}
	}
}
void CDialog::update(GLfloat p_updateTime) {
	Container::update(p_updateTime);
}
void CDialog::render() {
	Container::render();
}
