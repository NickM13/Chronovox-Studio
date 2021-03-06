#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"

Editor* EditorOverlay::m_editor = 0;
Container* EditorOverlay::m_container = 0;

Container* EditorOverlay::init(Editor* p_editor) {
	if (m_container) return 0;
	m_editor = p_editor;
	
	m_container = new Container("GUI_EDITOR", { 0, 0 }, { 0, 0 }, []() { return true; });
	// By default use prefix top for primary colors
	m_container->setElementPos("top");
	m_container->setBorderFlag(static_cast<Sint8>(Component::BorderFlag::NONE));

	Container* topContainer = static_cast<Container*>(m_container->addComponent(new Container("GUI_TOP", { 0, 0 }, { 0, 92 }, []() { return true; }),
		Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT));

	Container* titleBar = (Container*)topContainer->addComponent(new Container("TITLE", { 0, 0 }, { 0, 93 }, []() { return true; }), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)
		->setPriorityLayer(4);
	titleBar->addComponent(new Panel("DRAGBAR", "", { 0, 0 }, { 0, 0 }, (Sint8)Component::BorderFlag::BOTTOM), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setPressFunction([]() { GScreen::startWindowDrag(); })
		->setReleaseFunction([]() { GScreen::endWindowDrag(); })
		->setDoubleclickFunction([]() { GScreen::setWindowCommand(GScreen::WindowCommand::RESIZE); })
		->setFocused(true); // NOT DROGBAR
	titleBar->addComponent(new CIcon("WINDOW_ICON", MTexture::getTexture("gui\\icon\\window\\LogoFlat.png"), { 6, 4 }, { 24, 24 }));
	titleBar->addComponent(new CText("WINDOW_TITLE", GScreen::getWindowTitle(), { 0, 8 }, { 0, 0 }, Alignment::ALIGN_CENTER, "topText1"),
		Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_MINIMIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Minimize.png"),
		{ -68, 0 }, { 34, 28 }, CButton::RenderStyle::EMPTY, []() { GScreen::setWindowCommand(GScreen::WindowCommand::MINIMIZE); }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_RESIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Resize.png"),
		{ -34, 0 }, { 34, 28 }, CButton::RenderStyle::EMPTY, []() { GScreen::setWindowCommand(GScreen::WindowCommand::RESIZE); }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_CLOSE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Close.png"),
		{ 0, 0 }, { 34, 28 }, CButton::RenderStyle::EMPTY, []() { m_editor->attemptClose(); }), Component::Anchor::TOP_RIGHT);

	CMenubar* menuBar = new CMenubar("MENUBAR_MAIN", { 0, 32 }, { 0, 18 });
	menuBar->setPriorityLayer(16);
	CMenubar::Submenu* submenu = 0;

	submenu = new CMenubar::Submenu("File", "File");
	submenu->setPriority(0);
	menuBar->addElement("", submenu);
	menuBar->addElement("File", new CMenubar::MenuButton("New Model...", GKey::KeyBind(GLFW_KEY_N, GLFW_MOD_SHIFT + GLFW_MOD_CONTROL), []() { m_editor->fileNewModel(); }))
		->setPriority(0);
	menuBar->addElement("File", new CMenubar::MenuButton("Open...", GKey::KeyBind(GLFW_KEY_O, GLFW_MOD_CONTROL), []() { m_editor->fileOpen(); }))
		->setPriority(0);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setPriority(0);
	menuBar->addElement("File", new CMenubar::MenuButton("Exit", GKey::KeyBind(GLFW_KEY_F4, GLFW_MOD_ALT), []() { m_editor->attemptClose(); }))
		->setPriority(10);

	submenu = new CMenubar::Submenu("Edit", "Edit");
	submenu->setPriority(1);
	menuBar->addElement("", submenu);
	menuBar->addElement("Edit", new CMenubar::MenuButton("Preferences...",
		GKey::KeyBind(GLFW_KEY_F7, GLFW_MOD_CONTROL), []() { m_editor->editPreferences(); }))
		->setPriority(16);
	

	submenu = new CMenubar::Submenu("Help", "Help");
	submenu->setPriority(10);
	menuBar->addElement("", submenu);
	menuBar->addElement("Help", new CMenubar::MenuButton("About Chronovox Studio", GKey::KeyBind(), []() { m_editor->helpAbout(); }));

	topContainer->addComponent(menuBar, Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT);

	topContainer->addComponent(new ContainerPanel("GUI_SETTINGS", "", { 0, 50 }, { 0, 24 },
		static_cast<Sint8>(Component::BorderFlag::BOTTOM)), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)
		->setVisibleFunction([]() { return true; });

	CTabBar* tabbar = new CTabBar("TAB_FILES", { 0, 74 }, { 0, 18 });
	tabbar->setPriorityLayer(15);
	tabbar->setPressFunction([]() { m_editor->setProject(m_container->findComponent("GUI_TOP\\TAB_FILES")->getSelectedItem()); });
	tabbar->setReleaseFunction([]() { m_editor->attemptCloseProject(((CTabBar*)m_container->findComponent("GUI_TOP\\TAB_FILES"))->getClosedItem()); });
	tabbar->setBorderFlag((Sint8)Component::BorderFlag::BOTTOM);
	topContainer->addComponent(tabbar, Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT);


	// Information bar section (Bottom of screen)

	m_container->addComponent((new ContainerPanel("PANEL_INFOBAR", "", { 0, -24 }, { 0, 0 }, (Sint8)Component::BorderFlag::NONE))
		->setPriorityLayer(4), Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setElementPos("bottom");

	std::string* _matrixData = new std::string("");
	m_editor->setDataString(_matrixData);
	m_container->findComponent("PANEL_INFOBAR")->addComponent(new DataField("DATA_MATRIX", _matrixData, { 6, 0 }, { 0, 24 }), Component::Anchor::MIDDLE_LEFT);

	// Top
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_TOP", "", 0, { 12, 0 }, { -12, 5 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)
		->setVisibleFunction([]() { return !GScreen::isMaximized(); })
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::NSRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::TOP); });
	// Right
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_RIGHT", "", 0, { -5, 12 }, { 0, -18 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return !GScreen::isMaximized(); })
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::EWRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::RIGHT); });
	// Bottom
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_BOTTOM", "", 0, { 12, -5 }, { -18, 0 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return !GScreen::isMaximized(); })
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::NSRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::BOTTOM); });
	// Left
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_LEFT", "", 0, { 0, 12 }, { 5, -12 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_LEFT)
		->setVisibleFunction([]() { return !GScreen::isMaximized(); })
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::EWRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::LEFT); });

	// Top Left
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_TOP_LEFT1", "", 0, { 0, 0 }, { 8, 8 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_LEFT)
		->setVisibleFunction([]() { return !GScreen::isMaximized(); })
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::NWSERESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::TOP_LEFT); });
	// Top Right
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_TOP_RIGHT1", "", 0, { -8, 0 }, { 0, 8 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::TOP_RIGHT, Component::Anchor::TOP_RIGHT)
		->setVisibleFunction([]() { return !GScreen::isMaximized(); })
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::NESWRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::TOP_RIGHT); });
	// Bottom Left
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_BOTTOM_LEFT", "", 0, { 0, -8 }, { 8, 0 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_LEFT)
		->setVisibleFunction([]() { return !GScreen::isMaximized(); })
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::NESWRESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::BOTTOM_LEFT); });
	// Bottom Right
	m_container->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG_BOTTOM_RIGHT", "", MTexture::getTexture("gui\\icon\\tool\\ResizeWindow.png"), { -18, -18 }, { 0, 0 },
		CButton::RenderStyle::EMPTY))->setPriorityLayer(120), Component::Anchor::BOTTOM_RIGHT, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return !GScreen::isMaximized(); })
		->setHighlightActive(false)
		->setHoverCursor(GGui::CursorType::NWSERESIZE)
		->setPressFunction([]() { GScreen::startResizing(GScreen::ResizeType::BOTTOM_RIGHT); });

	return m_container;
}
