#include "engine\editor\animation\menu\AnimationOverlay.h"
#include "engine\editor\Editor.h"

Animation* AnimationOverlay::m_animation = 0;
Container* AnimationOverlay::m_container = 0;

Container* AnimationOverlay::init(Animation* p_animation) {
	if(m_container) return 0;
	m_animation = p_animation;

	m_container = new Container("GUI_ANIMATION", {}, {}, true);

	Container* titleBar = (Container*)m_container->addComponent(new Container("TITLE", {0, 0}, {0, 28}, true), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)->setPriorityLayer(4);
	titleBar->addComponent(new Panel("DRAGBAR", "", {0, 0}, {0, 28}, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::NONE), Component::Anchor::NONE, Component::Anchor::TOP_RIGHT)
		->setPressFunction([]() { GScreen::startWindowDrag(); })->setReleaseFunction([]() { GScreen::endWindowDrag(); }); // NOT DROGBAR
	titleBar->addComponent(new CIcon("WINDOW_ICON", MTexture::getTexture("gui\\icon\\window\\Logo.png"), {2, 2}, {24, 24}));
	titleBar->addComponent(new CText("WINDOW_TITLE", GScreen::m_windowTitle, {30, 14}, {0, 0}, Alignment::ALIGN_LEFT, Color(1, 1, 1)));
	titleBar->addComponent(new CButton("BUTTON_MINIMIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Minimize.png"),
		{-64, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::MINIMIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_RESIZE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Resize.png"),
		{-32, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::RESIZE; }), Component::Anchor::TOP_RIGHT);
	titleBar->addComponent(new CButton("BUTTON_CLOSE_WINDOW", "", MTexture::getTexture("gui\\icon\\window\\Close.png"),
		{0, 0}, {32, 28}, CButton::RenderStyle::FILL, []() { GScreen::m_windowCommand = GScreen::WindowCommand::CLOSE; }), Component::Anchor::TOP_RIGHT);
	m_container->addComponent(new CMenubar("TOOLBAR_MAIN", {0, 28}, {0, 18}), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)->setPriorityLayer(4);
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "File")->
		addButton("File", "New", "", []() { m_animation->fileNew(); })->
		addButton("File", "Open", "Ctrl+O", []() { m_animation->fileOpen(); })->
		addButton("File", "Save", "Ctrl+S", []() { m_animation->fileSave(); })->
		addButton("File", "Exit", "Alt+F4", []() { m_animation->fileExit(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Edit")->
		addButton("Edit", "Model Mode", "", []() { Editor::setEditorMode(Editor::EditorMode::MODEL); })->
		addButton("Edit", "Undo", "Ctrl+Z", []() { m_animation->editUndo(); })->
		addButton("Edit", "Redo", "Ctrl+Y", []() { m_animation->editRedo(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "View");
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Help")->
		addButton("Help", "About Voxel Model Editor", "", []() { AnimationOverlay::getContainer()->setPauseScreen("DIALOG_ABOUT"); });

	m_container->addComponent(new KeyframeTimeline({32, -172}, {-257, -20}), Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setPressFunction([]() {
		m_animation->loadKeyframeProperties();
		m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->setVisible(true);
	})->setReleaseFunction([]() {
		m_animation->saveKeyframeProperties();
		m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->setVisible(false);
	});

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", {-256, 46}, {0, 0}, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::BOTTOM),
		Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT);

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", {0, 0}, {32, 0}, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::RIGHT),
		Component::Anchor::NONE, Component::Anchor::BOTTOM_LEFT);

	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_PROPERTIES", "Properties", {0, 24}, {0, 0},
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::ALL), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT)->setVisible(false);
	m_container->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CDropDown("DROPDOWN_TRANSFORMATION", "Transformation Type",
		{0, 40}, 230, Component::Theme::ACTION), Component::Anchor::TOP_CENTER)
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
		Component::Anchor::BOTTOM_CENTER)->setPressFunction([&]() { m_animation->saveKeyframeProperties(); });

	return m_container;
}
