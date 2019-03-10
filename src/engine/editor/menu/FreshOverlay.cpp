#include "engine\editor\menu\FreshOverlay.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\menu\AboutDialog.h"

Editor* FreshOverlay::m_editor = 0;
Container* FreshOverlay::m_container = 0;

Container* FreshOverlay::init(Editor* p_editor) {
	if (m_container) return 0;
	m_editor = p_editor;

	m_container = new Container("GUI_FRESH", { 0, 0 }, { 0, 0 }, true);
	m_container->setPrimaryPos("Top");
	m_container->setBorderFlag(static_cast<Sint8>(Component::BorderFlag::NONE));


	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", { 0, 69 }, { 60, -25 },
		(Sint8)Component::BorderFlag::RIGHT + (Sint8)Component::BorderFlag::TOP), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_LEFT)
		->setPrimaryPos("Middle");

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", { -256, 69 }, { 0, -25 },
		(Sint8)Component::BorderFlag::LEFT + (Sint8)Component::BorderFlag::TOP), Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT)
		->setPrimaryPos("Middle");

	return m_container;
}
