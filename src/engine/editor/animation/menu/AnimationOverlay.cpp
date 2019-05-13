#include "engine\editor\animation\menu\AnimationOverlay.h"
#include "engine\editor\menu\EditorOverlay.h"

Editor* AnimationOverlay::m_editor = 0;
Container* AnimationOverlay::m_container = 0;

Container* AnimationOverlay::init(Editor* p_editor) {
	if(m_container) return 0;
	m_editor = p_editor;

	m_container = new Container("GUI_ANIMATION", {}, {}, []() { return true; });

	CMenubar* menuBar = static_cast<CMenubar*>(EditorOverlay::getContainer()->findComponent("GUI_TOP\\MENUBAR_MAIN"));
	CMenubar::Submenu* submenu;
	
	menuBar->addElement("File", new CMenubar::MenuButton("Close",
		GKey::KeyBind(), []() { m_editor->closeSelectedProject(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getAnimation() != 0; })
		->setPriority(4);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction([&]() -> bool { return p_editor->getAnimation() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Save",
		GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL), []() { m_editor->fileSave(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getAnimation() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Save As...",
		GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->fileSaveAs(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getAnimation() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction([&]() -> bool { return p_editor->getAnimation() != 0; })
		->setPriority(5);

	submenu = new CMenubar::Submenu("Anim_Edit", "Edit");
	submenu->setVisibleFunction([]() -> bool { return (m_editor->getAnimation() != 0); });
	submenu->setPriority(1);
	menuBar->addElement("", submenu);
	menuBar->addElement("Anim_Edit", new CMenubar::MenuButton("Undo Changes",
		GKey::KeyBind(GLFW_KEY_Z, GLFW_MOD_CONTROL), []() { m_editor->getAnimation()->editUndo(); }));
	menuBar->addElement("Anim_Edit", new CMenubar::MenuButton("Redo Changes",
		GKey::KeyBind(GLFW_KEY_Y, GLFW_MOD_CONTROL), []() { m_editor->getAnimation()->editRedo(); }));


	submenu = new CMenubar::Submenu("Anim_View", "View");
	submenu->setPriority(9);
	submenu->setVisibleFunction([&]() -> bool { return p_editor->getAnimation() != 0; });
	menuBar->addElement("", submenu);
	menuBar->addElement("Anim_View", new CMenubar::MenuDivider());
	menuBar->addElement("Anim_View", new CMenubar::Submenu("Anim_Zoom", "Zoom"));
	menuBar->addElement("Anim_View\\Anim_Zoom", new CMenubar::MenuButton("Reset Zoom",
		GKey::KeyBind(GLFW_KEY_0, GLFW_MOD_CONTROL), []() { Camera::resetZoom(); }));
	menuBar->addElement("Anim_View\\Anim_Zoom", new CMenubar::MenuButton("Zoom In",
		GKey::KeyBind(GLFW_KEY_EQUAL, GLFW_MOD_CONTROL), []() { Camera::addZoom(5.f); }));
	menuBar->addElement("Anim_View\\Anim_Zoom", new CMenubar::MenuButton("Zoom Out",
		GKey::KeyBind(GLFW_KEY_MINUS, GLFW_MOD_CONTROL), []() { Camera::addZoom(-5.f); }));

	// Workspace section (Middle of screen)

	Container* workContainer = static_cast<Container*>(m_container->addComponent(new Container("GUI_WORKSPACE", { 0, 68 }, { 0, -24 }, []() { return true; }),
		Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT));
	workContainer->setPrimaryPos("Middle");

	// Toolbar (Tools on left side of workspace)

	CButtonRadio* _toolbar;

	workContainer->addComponent(new ContainerPanel("GUI_TOOLBAR", "", { 0, 0 }, { 36, MTool::getParentCount() * 28 + 8 },
		(Sint8)Component::BorderFlag::ALL), Component::Anchor::MIDDLE_LEFT, Component::Anchor::NONE)
		->setPrimaryPos("Middle");

	/*_toolbar = new CButtonRadio("TOOLBAR_MAIN", "", { 4, 4 }, { 28, 28 }, { 0, 28 });
	workContainer->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_LEFT);
	_toolbar->setPressFunction([]() {
		m_editor->getModel()->updateTool();
		});

	for (Tool* tool : MTool::getToolList()) {
		_toolbar->addButton(tool->getIcon(), tool->getParent(), tool->getName(), tool->getDescription(), tool->getKeyBind());
	}*/

	KeyframeTimeline* timeline = new KeyframeTimeline({ 256, -172 }, { -257, 0 });
	workContainer->addComponent(timeline, Component::Anchor::BOTTOM_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setPressFunction([]() {
		m_editor->getAnimation()->loadKeyframeProperties();
		m_container->findComponent("GUI_WORKSPACE\\GUI_DETAILS\\GUI_PROPERTIES")->setVisibleFunction([]() { return true; });
	})->setReleaseFunction([]() {
		m_editor->getAnimation()->saveKeyframeProperties();
		m_container->findComponent("GUI_WORKSPACE\\GUI_DETAILS\\GUI_PROPERTIES")->setVisibleFunction([]() { return false; });
	});

	workContainer->addComponent(new ContainerPanel("GUI_DETAILS", "", {-256, 0}, {0, 0}, (Sint8)Component::BorderFlag::LEFT),
		Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT);

	workContainer->addComponent(new ContainerPanel("GUI_MATRICES", "", {0, 0}, {256, 0}, (Sint8)Component::BorderFlag::RIGHT),
		Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_LEFT);

	/*
	m_container->addPauseScreen(new Dialog("DIALOG_NEW_TRANSLATE", "New Translate Keyframe", {}, { 250, 120 }), Component::Anchor::MIDDLE_CENTER)
		->setPressFunction([]() {})
		->setReleaseFunction([]() {
		Keyframe* keyframe = new Keyframe();
		
		m_editor->getAnimation()->getTimeline()->addKeyframe(keyframe);
	});
	*/

	workContainer->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_PROPERTIES", "Properties", {0, 24}, {0, 0},
		(Sint8)Component::BorderFlag::ALL), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT)->setVisibleFunction([]() { return false; });
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CDropDown("DROPDOWN_TRANSFORMATION", "Transformation Type",
		{0, 40}, 230), Component::Anchor::TOP_CENTER)
		->addItem("Translation")->addItem("Rotation")->addItem("Scale");
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CText("TEXT_MATRIX", "Matrix", {-50, 88}, {0, 0},
		Alignment::ALIGN_RIGHT, "textLight"), Component::Anchor::TOP_CENTER);
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new TextField("TEXTFIELD_MATRIX", "Matrix", {40, 80}, {150, 1}, 0), Component::Anchor::TOP_CENTER);

	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CText("TEXT_FOCUS", "Focus Point", {0, 118}, {0, 0},
		Alignment::ALIGN_CENTER, "textLight"), Component::Anchor::TOP_CENTER);
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("FOCUS_X", "", {-60, 130}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("FOCUS_Y", "", {0, 130}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("FOCUS_Z", "", {60, 130}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);

	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CText("TEXT_VALUE", "Transform Values", {0, 168}, {0, 0},
		Alignment::ALIGN_CENTER, "textLight"), Component::Anchor::TOP_CENTER);
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("VALUE_X", "", {-60, 180}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("VALUE_Y", "", {0, 180}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("VALUE_Z", "", {60, 180}, {50, 1}, {-10000, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);

	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("START", "Start ", {60, 230}, {50, 1}, {0, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);
	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new NumberField("LENGTH", "Length ", {60, 270}, {50, 1}, {0, 10000},
		NumberField::NumType::FLOAT), Component::Anchor::TOP_CENTER);

	workContainer->findComponent("GUI_DETAILS\\GUI_PROPERTIES")->addComponent(new CButton("OK", "Update", {0, -12}, {64, 24}, CButton::RenderStyle::ALL),
		Component::Anchor::BOTTOM_CENTER)->setPressFunction([&]() { m_editor->getAnimation()->saveKeyframeProperties(); });

	return m_container;
}
