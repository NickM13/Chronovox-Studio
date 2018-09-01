#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\animation\menu\AnimationOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"

Editor* EditorOverlay::m_editor = 0;
Container* EditorOverlay::m_container = 0;

Container* EditorOverlay::init(Editor* p_editor) {
	if(m_container) return 0;
	m_editor = p_editor;

	m_container = new Container("GUI_OVERLAY", {0, 0}, GScreen::m_screenSize, true);

	/*
	Container* titleBar = (Container*) m_container->addComponent(new Container("TITLE", {0, 0}, {0, 28}, true), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)->setPriorityLayer(4);
	titleBar->addComponent(new Panel("DRAGBAR", "", {0, 0}, {0, 28}, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::NONE), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)
		->setPressFunction([]() { GScreen::startWindowDrag(); })->setReleaseFunction([]() { GScreen::endWindowDrag(); }); // NOT DROGBAR
	titleBar->addComponent(new CIcon("WINDOW_ICON", MTexture::getTexture("gui\\icon\\window\\Logo.png"), {2, 2}, {24, 24}));
	titleBar->addComponent(new CText("WINDOW_TITLE", "Nick's Voxel Editor v0.9", {30, 14}, {0, 0}, Alignment::ALIGN_LEFT, Color(1, 1, 1)));
	titleBar->addComponent(new CButton("BUTTON_MINIMIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Minimize.png"), 
		{-64, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::MINIMIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_RESIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Resize.png"), 
		{-32, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::RESIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_CLOSE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Close.png"), 
		{0, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::CLOSE; }), Component::Anchor::TOP_RIGHT);
	m_container->addComponent(new CToolbar("TOOLBAR_MAIN", {0, 28}, {0, 18}), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)->setPriorityLayer(4);
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "File")->
		addButton("File", "New", "", []() { m_editor->fileNew(); })->
		addButton("File", "Open", "Ctrl+O", []() { m_editor->fileOpen(); })->
		addButton("File", "Save", "Ctrl+S", []() { m_editor->fileSave(); })->
		addButton("File", "Exit", "Alt+F4", []() { m_editor->fileExit(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Edit")->
		addButton("Edit", "Animation Mode", "", []() { m_editor->setEditorMode(Editor::EditorMode::ANIMATION); })->
		addButton("Edit", "Model Mode", "", []() { m_editor->setEditorMode(Editor::EditorMode::MODEL); })->
		addButton("Edit", "Undo", "Ctrl+Z", []() { m_editor->editUndo(); })->
		addButton("Edit", "Redo", "Ctrl+Y", []() { m_editor->editRedo(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Model")->
		addButton("Model", "New Matrix", "Ctrl+N", []() { ModelOverlay::getContainer()->setPauseScreen("NEWMATRIX"); })->
		addButton("Model", "Properties", "F2", []() { ModelOverlay::getContainer()->setPauseScreen("PROPERTIES"); })->
		addButton("Model", "Flip X", "", []() { m_editor->getModel()->flipMatrix(AXIS_X); })->
		addButton("Model", "Flip Y", "", []() { m_editor->getModel()->flipMatrix(AXIS_Y); })->
		addButton("Model", "Flip Z", "", []() { m_editor->getModel()->flipMatrix(AXIS_Z); })->
		addButton("Model", "Rotate X", "", []() { m_editor->getModel()->rotateMatrix(AXIS_X); })->
		addButton("Model", "Rotate Y", "", []() { m_editor->getModel()->rotateMatrix(AXIS_Y); })->
		addButton("Model", "Rotate Z", "", []() { m_editor->getModel()->rotateMatrix(AXIS_Z); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Animation")->
		addButton("Animation", "Add Keyframe", "", []() { AnimationOverlay::getContainer()->setPauseScreen("KEYFRAME"); })->
		addButton("Animation", "Play/Pause", "Space", []() { m_editor->getAnimation()->toggleAnimationPlaying(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "View")->
		addButton("View", "Focus Matrix", "Space", []() { m_editor->getModel()->focus(); })->
		addButton("View", "Toggle Grid", "Ctrl+G", []() { m_editor->getModel()->toggleGrid(); })->
		addButton("View", "Toggle Outline", "Ctrl+H", []() { m_editor->getModel()->toggleOutline(); });
	*/

	m_container->addComponent((new ContainerPanel("PANEL_INFOBAR", "", {0, -20}, {0, 0}, Component::Theme::INFO, (Sint8)Component::BorderFlag::TOP))
		->setPriorityLayer(4), Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT);

	std::string* _matrixData = new std::string("");
	m_editor->getModel()->setDataString(_matrixData);
	m_container->findComponent("PANEL_INFOBAR")->addComponent(new DataField("DATA_MATRIX", _matrixData, {4, 0}, {0, 24}, Component::Theme::INFO), Component::Anchor::MIDDLE_LEFT);

	//TODO: Add cursor type to components that are applied on hover
	m_container->findComponent("PANEL_INFOBAR")->addComponent((new CButton("BUTTON_RESIZE_WINDOW_DRAG", "", MTexture::getTexture("gui\\icon\\tool\\ResizeWindow.png"), {0, 0}, {20, 20}, CButton::RenderStyle::EMPTY))
		, Component::Anchor::MIDDLE_RIGHT)
		->setPriorityLayer(120)
		->setPressFunction([]() { GScreen::startResizing(); })
		->setReleaseFunction([]() { GScreen::stopResizing(); });
	return m_container;
}
