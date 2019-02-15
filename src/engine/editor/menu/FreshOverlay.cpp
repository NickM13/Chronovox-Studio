#include "engine\editor\menu\FreshOverlay.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\menu\AboutDialog.h"

Editor* FreshOverlay::m_editor = 0;
Container* FreshOverlay::m_container = 0;

Container* FreshOverlay::init(Editor* p_editor) {
	if (m_container) return 0;
	m_editor = p_editor;

	m_container = new Container("GUI_FRESH", { 0, 0 }, { 0, 0 }, true);
	m_container->setTheme(Component::Theme::WINDOW);
	m_container->setBorderFlag(static_cast<Sint8>(Component::BorderFlag::ALL));

	CMenubar* menuBar = new CMenubar("MENUBAR_MAIN", { 0, 32 }, { 0, 18 });
	menuBar->addButton("", "File")
		->addButton("File", "New")
		->addButton("File\\New", "Model", GKey::KeyBind(), []() { m_editor->fileNewModel(); })
		->addButton("File", "Open", GKey::KeyBind(GLFW_KEY_O, GLFW_MOD_CONTROL), []() { m_editor->fileOpen(); })
		->addButton("File", "Exit", GKey::KeyBind(GLFW_KEY_F4, GLFW_MOD_ALT), []() { m_editor->fileExit(); });
	if (GScreen::m_developer) {
		menuBar->addButton("File\\New", "Animation", GKey::KeyBind(), []() { m_editor->fileNewAnimation(); });
	}
	menuBar->addButton("", "Help")
		->addButton("Help", "About Voxel Model Editor", GKey::KeyBind(), []() { EditorOverlay::getContainer()->openDialog(AboutDialog::getInstance().getDialog()); });
	m_container->addComponent(menuBar, Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)->setPriorityLayer(16);

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", { 0, 69 }, { 60, -20 },
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::RIGHT + (Sint8)Component::BorderFlag::TOP), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_LEFT);

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", { -256, 69 }, { 0, -20 },
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::LEFT + (Sint8)Component::BorderFlag::TOP), Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT);

	return m_container;
}
