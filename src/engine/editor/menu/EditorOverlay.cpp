#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\menu\AboutDialog.h"
#include "engine\editor\model\menu\ModelOverlay.h"
#include "engine\editor\animation\menu\AnimationOverlay.h"

Editor* EditorOverlay::m_editor = 0;
Container* EditorOverlay::m_container = 0;

Container* EditorOverlay::init(Editor* p_editor) {
	if (m_container) return 0;
	m_editor = p_editor;
	
	m_container = new Container("GUI_EDITOR", { 0, 0 }, { 0, 0 }, true);
	m_container->setPrimaryPos("Top");
	m_container->setBorderFlag(static_cast<Sint8>(Component::BorderFlag::NONE));

	Container* titleBar = (Container*)m_container->addComponent(new Container("TITLE", { 0, 0 }, { 0, 68 }, true), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)
		->setPriorityLayer(4);
	titleBar->addComponent(new Panel("DRAGBAR", "", { 0, 0 }, { 0, 0 }, (Sint8)Component::BorderFlag::BOTTOM), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setPressFunction([]() { GScreen::startWindowDrag(); })->setReleaseFunction([]() { GScreen::endWindowDrag(); }); // NOT DROGBAR
	titleBar->addComponent(new CIcon("WINDOW_ICON", MTexture::getTexture("gui\\icon\\window\\Logo.png"), { 12, 4 }, { 24, 24 }));
	titleBar->addComponent(new CText("WINDOW_TITLE", GScreen::m_windowTitle, { 0, 16 }, { 0, 0 }, Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_MINIMIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Minimize.png"),
		{ -68, 0 }, { 34, 28 }, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::MINIMIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_RESIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Resize.png"),
		{ -34, 0 }, { 34, 28 }, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::RESIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_CLOSE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Close.png"),
		{ 0, 0 }, { 34, 28 }, CButton::RenderStyle::FILL, []() { m_editor->attemptClose(); }), Component::Anchor::TOP_RIGHT);

	CMenubar* menuBar = new CMenubar("MENUBAR_MAIN", { 0, 32 }, { 0, 18 });
	menuBar->setPriorityLayer(16);
	CMenubar::Submenu* submenu = 0;

	submenu = new CMenubar::Submenu("File");
	submenu->setPriority(0);
	menuBar->addElement("", submenu);
	menuBar->addElement("File", new CMenubar::Submenu("New"))
		->setPriority(0);
	menuBar->addElement("File\\New", new CMenubar::MenuButton("Model...", GKey::KeyBind(GLFW_KEY_N, GLFW_MOD_SHIFT + GLFW_MOD_CONTROL), []() { m_editor->fileNewModel(); }));
#ifdef _DEBUG
	menuBar->addElement("File\\New", new CMenubar::MenuButton("Animation...", GKey::KeyBind(), []() { m_editor->fileNewAnimation(); }));
#endif
	menuBar->addElement("File", new CMenubar::MenuButton("Open", GKey::KeyBind(GLFW_KEY_O, GLFW_MOD_CONTROL), []() { m_editor->fileOpen(); }))
		->setPriority(0);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setPriority(0);
	menuBar->addElement("File", new CMenubar::MenuButton("Exit", GKey::KeyBind(GLFW_KEY_F4, GLFW_MOD_ALT), []() { m_editor->attemptClose(); }))
		->setPriority(10);

	submenu = new CMenubar::Submenu("Help");
	submenu->setPriority(10);
	menuBar->addElement("", submenu);
	menuBar->addElement("Help", new CMenubar::MenuButton("About Chronovox Studio", GKey::KeyBind(), []() { Gui::openDialog(AboutDialog::getInstance().getDialog()); }));

	m_container->addComponent(menuBar, Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT);

	CTabBar* tabbar = new CTabBar("TAB_FILES", { 0, 49 }, { 0, 18 });
	tabbar->setPriorityLayer(15);
	tabbar->setPressFunction([]() { m_editor->setProject(m_container->findComponent("TAB_FILES")->getSelectedItem()); });
	tabbar->setReleaseFunction([]() { m_editor->attemptCloseProject(((CTabBar*)m_container->findComponent("TAB_FILES"))->getClosedItem()); });
	tabbar->setBorderFlag((Sint8)Component::BorderFlag::BOTTOM);
	m_container->addComponent(tabbar, Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT);

	m_container->addComponent((new ContainerPanel("PANEL_INFOBAR", "", { 0, -24 }, { 0, 0 }, (Sint8)Component::BorderFlag::NONE))
		->setPriorityLayer(4), Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setPrimaryPos("Bottom");

	std::string* _matrixData = new std::string("");
	m_editor->setDataString(_matrixData);
	m_container->findComponent("PANEL_INFOBAR")->addComponent(new DataField("DATA_MATRIX", _matrixData, { 6, 0 }, { 0, 24 }), Component::Anchor::MIDDLE_LEFT);

	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_BOTTOM_RIGHT", "", MTexture::getTexture("gui\\icon\\tool\\ResizeWindow.png"), { -24, -24 }, { 0, 0 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::BOTTOM_RIGHT, Component::Anchor::BOTTOM_RIGHT)
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::HRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::BOTTOM_RIGHT); })
		->setReleaseFunction([]() { GScreen::stopResizing(); });
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_TOP", "", 0, { 0, 0 }, { 0, 2 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::VRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::TOP); })
		->setReleaseFunction([]() { GScreen::stopResizing(); });
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_RIGHT", "", 0, { -2, 0 }, { 0, -24 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT)
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::HRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::RIGHT); })
		->setReleaseFunction([]() { GScreen::stopResizing(); });
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_BOTTOM", "", 0, { 0, -2 }, { -24, 0 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::VRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::BOTTOM); })
		->setReleaseFunction([]() { GScreen::stopResizing(); });
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_LEFT", "", 0, { 0, 0 }, { 2, 0 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_LEFT)
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::HRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::LEFT); })
		->setReleaseFunction([]() { GScreen::stopResizing(); });
	/*
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_TOP_LEFT", "", 0, { 0, 0 }, { 2, 2 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_LEFT)
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::VRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::TOP_LEFT); })
		->setReleaseFunction([]() { GScreen::stopResizing(); });
		*/
	return m_container;
}
