#include "engine\gfx\gui\LGui.h"

Container* Gui::m_mainContainer = 0;

void Gui::init() {
	m_mainContainer = new Container("GUI", { 0, 0 }, GScreen::getScreenSize(), []() { return true; });
}

Container* Gui::getContainer() {
	return m_mainContainer;
}

void Gui::openDialog(CDialog* p_dialog) {
	m_mainContainer->openDialog(p_dialog);
}

void Gui::resize() {
	m_mainContainer->setSize(GScreen::getScreenSize());
}

void Gui::input(Sint8& p_iFlags) {
	m_mainContainer->input(p_iFlags);
}

void Gui::update(GLfloat p_deltaTime) {
	m_mainContainer->update(p_deltaTime);
	GGui::update();
}

void Gui::render() {
	m_mainContainer->render();

	if (!GScreen::isMaximized()) {
		Sint32 sx = m_mainContainer->getSize().x, sy = m_mainContainer->getSize().y;

		GBuffer::setTexture(0);
		GBuffer::setColor(Component::getElementColor(GScreen::isFocused() ? "borderWindowFocused" : "borderWindowUnfocused"));

		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(1, 0);
		GBuffer::addVertexQuad(1, sy);
		GBuffer::addVertexQuad(0, sy);

		GBuffer::addVertexQuad(sx, 0);
		GBuffer::addVertexQuad(sx - 1, 0);
		GBuffer::addVertexQuad(sx - 1, sy);
		GBuffer::addVertexQuad(sx, sy);

		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(sx, 0);
		GBuffer::addVertexQuad(sx, 1);
		GBuffer::addVertexQuad(0, 1);

		GBuffer::addVertexQuad(0, sy);
		GBuffer::addVertexQuad(sx, sy);
		GBuffer::addVertexQuad(sx, sy - 1);
		GBuffer::addVertexQuad(0, sy - 1);
	}
}
