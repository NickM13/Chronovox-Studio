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



	Container* workContainer = static_cast<Container*>(m_container->addComponent(new Container("GUI_WORKSPACE",
		{ 0, EditorOverlay::getContainer()->findComponent("GUI_TOP")->getSize().y },
		{ 0, -EditorOverlay::getContainer()->findComponent("PANEL_INFOBAR")->getSize().y }, []() { return true; }),
		Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT));
	workContainer->setVisible([&]() { return true; });
	workContainer->setElementPos("workspace");

	CListThick* recents = new CListThick("LIST_RECENTS", "", {280, 100}, {-80, -80}, 56, MTexture::getTexture("gui\\icon\\list\\ListIcon.png"));
	GRecents::init();
	for (std::string f : GRecents::getRecentFiles()) {
		recents->addItem(f);
	}
	recents->setPressFunction([&]() {
		m_editor->dropFile(static_cast<CListThick*>(FreshOverlay::getContainer()->findComponent("GUI_WORKSPACE\\LIST_RECENTS"))->getItem(FreshOverlay::getContainer()->findComponent("GUI_WORKSPACE\\LIST_RECENTS")->getSelectedItem()).path.c_str());
		});
	workContainer->addComponent(new Panel("", "", {}, {}, 0), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT);
	workContainer->addComponent(new CText("TEXT_STARTED", "", { 83, 64 }, {}, Alignment::ALIGN_LEFT, "workspaceText1", "Header"));
	workContainer->addComponent(new CText("TEXT_RECENTS", "Open Recents", { 283, 64 }, {}, Alignment::ALIGN_LEFT, "workspaceText1", "Header"));
	workContainer->addComponent(recents, Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT);

	workContainer->addComponent(new CButton("BUTTON_NEWMODEL", "", MTexture::getTexture("gui\\icon\\window\\NewModel.png"),
		{ 80, 100 }, { 128, 128 }, CButton::RenderStyle::EMPTY, [&]() { m_editor->fileNewModel(); }), Component::Anchor::TOP_LEFT)->setTooltip("Create a New Model");
	workContainer->addComponent(new CButton("BUTTON_OPENMODEL", "", MTexture::getTexture("gui\\icon\\window\\OpenModel.png"),
		{ 80, 248 }, { 128, 128 }, CButton::RenderStyle::EMPTY, [&]() { m_editor->fileNewModel(); }), Component::Anchor::TOP_LEFT)->setTooltip("Open Model");

	return m_container;
}
