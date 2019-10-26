#include "engine\editor\menu\FreshOverlay.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\utils\directory\LDirectory.h"
#include "engine\editor\GRecents.h"

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

	Container* workContainer = static_cast<Container*>(m_container->addComponent(new Container("GUI_WORKSPACE",
		{ 0, EditorOverlay::getContainer()->findComponent("GUI_TOP")->getSize().y },
		{ 0, -EditorOverlay::getContainer()->findComponent("PANEL_INFOBAR")->getSize().y }, []() { return true; }),
		Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT));
	workContainer->setElementPos("workspace");

	CListThick* recents = new CListThick("LIST_RECENTS", "", {100, 100}, {0, 0}, 56, MTexture::getTexture("gui\\icon\\list\\ListIcon.png"));
	GRecents::init();
	for (std::string f : GRecents::getRecentFiles()) {
		recents->addItem(f);
	}
	recents->setPressFunction([&]() {
		m_editor->dropFile(static_cast<CListThick*>(FreshOverlay::getContainer()->findComponent("GUI_WORKSPACE\\LIST_RECENTS"))->getItem(FreshOverlay::getContainer()->findComponent("GUI_WORKSPACE\\LIST_RECENTS")->getSelectedItem()).path.c_str());
		});
	workContainer->addComponent(new CText("TEXT_RECENTS", "Open Recents", { 103, 70 }, {}, Alignment::ALIGN_LEFT, "workspaceText1", "Header"));
	workContainer->addComponent(recents, Component::Anchor::TOP_LEFT, Component::Anchor::MIDDLE_CENTER);

	return m_container;
}
