#include "engine\gfx\gui\component\dialog\Dialog.h"
#include "engine\gfx\gui\component\button\Button.h"
#include "engine\gfx\gui\component\container\ColorPanel.h"
#include "engine\gfx\gui\component\container\Panel.h"

Dialog::Dialog(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Container(p_compName, p_pos, p_size, false) {
	setPriorityLayer(100);
	addComponent(new Panel("WINDOW", p_title, { 0, 0 }, { 0, 0 }, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::ALL), Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT);
	addComponent(new CButton("OK", "Ok", { 0, 0 }, { 0, 24 }, CButton::RenderStyle::ALL), Anchor::BOTTOM_LEFT, Anchor::BOTTOM_CENTER)->setPressFunction([&]() { callReleaseFunction(); setVisible(false); });
	addComponent(new CButton("CANCEL", "Cancel", { 0, 0 }, { 0, 24 }, CButton::RenderStyle::ALL), Anchor::BOTTOM_CENTER, Anchor::BOTTOM_RIGHT)->setPressFunction([&]() { setVisible(false); });
}

void Dialog::input(Sint8& p_interactFlags) {
	if (GKey::keyPressed(GLFW_KEY_ESCAPE)) setVisible(false);
	Container::input(p_interactFlags);
}
void Dialog::update(GLfloat p_updateTime) {
	Container::update(p_updateTime);
}
void Dialog::render() {
	Container::render();
}
