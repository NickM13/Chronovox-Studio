#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\animation\menu\AnimationOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"

Editor* EditorOverlay::m_editor = 0;
Container* EditorOverlay::m_container = 0;

Container* EditorOverlay::init(Editor* p_editor) {
	if (m_container) return 0;
	m_editor = p_editor;

	m_container = new Container("GUI_OVERLAY", { 0, 0 }, GScreen::m_screenSize, true);
	m_container->setTheme(Component::Theme::WINDOW);
	m_container->setBorderFlag(static_cast<Sint8>(Component::BorderFlag::ALL));

	Container* titleBar = (Container*)m_container->addComponent(new Container("TITLE", { 0, 0 }, { 0, 28 }, true), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)->setPriorityLayer(4);
	titleBar->addComponent(new Panel("DRAGBAR", "", { 0, 0 }, { 0, 32 }, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::NONE), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)
		->setPressFunction([]() { GScreen::startWindowDrag(); })->setReleaseFunction([]() { GScreen::endWindowDrag(); }); // NOT DROGBAR
	titleBar->addComponent(new CIcon("WINDOW_ICON", MTexture::getTexture("gui\\icon\\window\\Logo.png"), { 4, 4 }, { 24, 24 }));
	titleBar->addComponent(new CText("WINDOW_TITLE", GScreen::m_windowTitle, { 32, 16 }, { 0, 0 }, Alignment::ALIGN_LEFT, Color(1, 1, 1)));
	titleBar->addComponent(new CButton("BUTTON_MINIMIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Minimize.png"),
		{ -68, 0 }, { 34, 28 }, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::MINIMIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_RESIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Resize.png"),
		{ -34, 0 }, { 34, 28 }, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::RESIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_CLOSE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Close.png"),
		{ 0, 0 }, { 34, 28 }, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::CLOSE; }), Component::Anchor::TOP_RIGHT);

	m_container->addComponent(new CTabBar("TAB_FILES", { 1, 50 }, { -1, 18 }, Component::Theme::PRIMARY), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)
		->setPressFunction([]() { m_editor->setProject(m_container->findComponent("TAB_FILES")->getSelectedItem()); })
		->setReleaseFunction([]() { m_editor->attemptCloseProject(((CTabBar*)m_container->findComponent("TAB_FILES"))->getClosedItem()); })
		->setBorderFlag((Sint8)Component::BorderFlag::ALL - (Sint8)Component::BorderFlag::TOP)
		->setPriorityLayer(-4);

	m_container->addComponent((new ContainerPanel("PANEL_INFOBAR", "", { 0, -20 }, { 0, 0 }, Component::Theme::INFO, (Sint8)Component::BorderFlag::TOP))
		->setPriorityLayer(4), Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT);

	std::string* _matrixData = new std::string("");
	m_editor->setDataString(_matrixData);
	m_container->findComponent("PANEL_INFOBAR")->addComponent(new DataField("DATA_MATRIX", _matrixData, { 4, 0 }, { 0, 24 }, Component::Theme::INFO), Component::Anchor::MIDDLE_LEFT);

	m_container->findComponent("PANEL_INFOBAR")->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG", "", MTexture::getTexture("gui\\icon\\tool\\ResizeWindow.png"), { 0, 0 }, { 20, 20 },
		CButton::RenderStyle::EMPTY)), Component::Anchor::MIDDLE_RIGHT)
		->setPriorityLayer(120)
		->setPressFunction([]() { GScreen::startResizing(); })
		->setReleaseFunction([]() { GScreen::stopResizing(); });
	return m_container;
}
