#include "engine\editor\animation\menu\AnimationOverlay.h"

Editor* AnimationOverlay::m_editor = 0;
Container* AnimationOverlay::m_container = 0;

Container* AnimationOverlay::init(Editor* p_editor) {
	if(m_container) return 0;
	m_editor = p_editor;

	m_container = new Container("GUI_ANIMATION", {}, {}, true);

	Container* titleBar = (Container*)m_container->addComponent(new Container("TITLE", {0, 0}, {0, 28}, true), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)->setPriorityLayer(4);
	titleBar->addComponent(new Panel("DRAGBAR", "", {0, 0}, {0, 28}, (Sint8)Component::BorderFlag::NONE), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)
		->setPressFunction([]() { GScreen::startWindowDrag(); })->setReleaseFunction([]() { GScreen::endWindowDrag(); }); // NOT DROGBAR
	titleBar->addComponent(new CIcon("WINDOW_ICON", MTexture::getTexture("gui\\icon\\window\\Logo.png"), {2, 2}, {24, 24}));
	titleBar->addComponent(new CText("WINDOW_TITLE", GScreen::m_windowTitle, {30, 14}, {0, 0}, Alignment::ALIGN_LEFT, Color(1, 1, 1)));
	titleBar->addComponent(new CButton("BUTTON_MINIMIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Minimize.png"),
		{-64, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::MINIMIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_RESIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Resize.png"),
		{-32, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::RESIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_CLOSE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Close.png"),
		{0, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::CLOSE; }), Component::Anchor::TOP_RIGHT);
	
	/*
	CMenubar* menuBar = new CMenubar("TOOLBAR_MAIN", { 0, 32 }, { 0, 18 });
	menuBar->addButton("", "File")
		->addButton("File", "New")
		->addButton("File\\New", "Model", GKey::KeyBind(), []() { m_editor->fileNewModel(); })
		->addButton("File\\New", "Animation", GKey::KeyBind(), []() { m_editor->fileNewAnimation(); })
		->addButton("File", "Open", GKey::KeyBind(GLFW_KEY_O, GLFW_MOD_CONTROL), []() { m_editor->fileOpen(); })
		->addButton("File", "Save", GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL), []() { m_editor->fileSave(); })
		->addButton("File", "Save As", GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->fileSaveAs(); })
		->addButton("File", "Close", GKey::KeyBind(), []() { m_editor->closeSelectedProject(); })
		->addButton("File", "Exit", GKey::KeyBind(GLFW_KEY_F4, GLFW_MOD_ALT), []() { m_editor->attemptClose(); });
	menuBar->addButton("", "Edit")
		->addButton("Edit", "Undo", GKey::KeyBind(GLFW_KEY_Z, GLFW_MOD_CONTROL), []() { if (m_editor->getAnimation()) m_editor->getAnimation()->editUndo(); })
		->addButton("Edit", "Redo", GKey::KeyBind(GLFW_KEY_Y, GLFW_MOD_CONTROL), []() { if (m_editor->getAnimation()) m_editor->getAnimation()->editRedo(); });
	menuBar->addButton("", "View")
		->addButton("View", "Load Model", GKey::KeyBind(), []() { if (m_editor->getAnimation()) m_editor->getAnimation()->viewLoadModel(); });
	menuBar->addButton("", "Help")
		->addButton("Help", "About Voxel Model Editor", GKey::KeyBind(), []() {  });
	m_container->addComponent(menuBar, Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)->setPriorityLayer(4);
	*/

	KeyframeTimeline* timeline = new KeyframeTimeline({ 256, -172 }, { -257, -20 });
	m_container->addComponent(timeline, Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setPressFunction([]() {
		m_editor->getAnimation()->loadKeyframeProperties();
		m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->setVisible(true);
	})->setReleaseFunction([]() {
		m_editor->getAnimation()->saveKeyframeProperties();
		m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->setVisible(false);
	});

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", {-256, 69}, {0, 0}, (Sint8)Component::BorderFlag::LEFT),
		Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT);

	m_container->addComponent(new ContainerPanel("GUI_MATRICES", "", {0, 69}, {256, 0}, (Sint8)Component::BorderFlag::RIGHT),
		Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_LEFT);

	/*
	m_container->addPauseScreen(new Dialog("DIALOG_NEW_TRANSLATE", "New Translate Keyframe", {}, { 250, 120 }), Component::Anchor::MIDDLE_CENTER)
		->setPressFunction([]() {})
		->setReleaseFunction([]() {
		Keyframe* keyframe = new Keyframe();
		
		m_editor->getAnimation()->getTimeline()->addKeyframe(keyframe);
	});
	*/

	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_PROPERTIES", "Properties", {0, 24}, {0, 0},
		(Sint8)Component::BorderFlag::ALL), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT)->setVisible(false);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CDropDown("DROPDOWN_TRANSFORMATION", "Transformation Type",
		{0, 40}, 230), Component::Anchor::TOP_CENTER)
		->addItem("Translation")->addItem("Rotation")->addItem("Scale");
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CText("TEXT_MATRIX", "Matrix", {-50, 88}, {0, 0},
		Alignment::ALIGN_RIGHT, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new TextField("TEXTFIELD_MATRIX", "Matrix", {40, 80}, {150, 1}, 0), Component::Anchor::TOP_CENTER);

	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CText("TEXT_FOCUS", "Focus Point", {0, 118}, {0, 0},
		Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("FOCUS_X", "", {-60, 130}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("FOCUS_Y", "", {0, 130}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("FOCUS_Z", "", {60, 130}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);

	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CText("TEXT_VALUE", "Transform Values", {0, 168}, {0, 0},
		Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("VALUE_X", "", {-60, 180}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("VALUE_Y", "", {0, 180}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("VALUE_Z", "", {60, 180}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);

	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("START", "Start ", {60, 230}, {50, 1}, {0, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("LENGTH", "Length ", {60, 270}, {50, 1}, {0, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);

	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CButton("OK", "Update", {0, -12}, {64, 24}, CButton::RenderStyle::ALL),
		Component::Anchor::BOTTOM_CENTER)->setPressFunction([&]() { m_editor->getAnimation()->saveKeyframeProperties(); });

	return m_container;
}
