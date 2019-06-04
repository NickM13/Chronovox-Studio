#include "engine\editor\menu\FreshOverlay.h"
#include "engine\editor\menu\EditorOverlay.h"

Editor* FreshOverlay::m_editor = 0;
Container* FreshOverlay::m_container = 0;

Container* FreshOverlay::init(Editor* p_editor) {
	if (m_container) return 0;
	m_editor = p_editor;

	m_container = new Container("GUI_FRESH", { 0, 0 }, { 0, 0 }, []() { return true; });
	m_container->setElementPos("top");
	m_container->setBorderFlag(static_cast<Sint8>(Component::BorderFlag::NONE));


	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", { 0, 68 }, { 60, -24 },
		(Sint8)Component::BorderFlag::RIGHT), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_LEFT)
		->setElementPos("workspace");

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", { -256, 68 }, { 0, -24 },
		(Sint8)Component::BorderFlag::LEFT), Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT)
		->setElementPos("workspace");

	return m_container;
}
