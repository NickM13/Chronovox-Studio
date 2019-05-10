#include "engine\editor\model\menu\ModelOverlay.h"
#include "engine\editor\model\menu\ModelPropertiesDialog.h"
#include "engine\editor\model\menu\NewModelDialog.h"
#include "engine\editor\camera\Camera.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\menu\AboutDialog.h"
#include "engine\utils\directory\LDirectory.h"

Editor* ModelOverlay::m_editor = 0;
Container* ModelOverlay::m_container = 0;
ColorOverlay* ModelOverlay::m_colorOverlay = 0;

Container* ModelOverlay::init(Editor* p_editor) {
	if (m_container) return 0;
	m_editor = p_editor;

	m_container = new Container("GUI_MODEL", { 0, 0 }, {}, []() { return true; });

	CMenubar* menuBar = static_cast<CMenubar*>(EditorOverlay::getContainer()->findComponent("GUI_TOP\\MENUBAR_MAIN"));
	CMenubar::Submenu* submenu;

	menuBar->addElement("File", new CMenubar::MenuButton("Add",
		GKey::KeyBind(GLFW_KEY_O, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->getModel()->fileAdd(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(4);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(4);
	menuBar->addElement("File", new CMenubar::MenuButton("Close",
		GKey::KeyBind(), []() { m_editor->closeSelectedProject(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(4);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Save",
		GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL), []() { m_editor->fileSave(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Save As...",
		GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->fileSaveAs(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(5);

	submenu = new CMenubar::Submenu("Edit");
	submenu->setVisibleFunction([]() -> bool { return (m_editor->getModel() != 0); });
	submenu->setPriority(1);
	menuBar->addElement("", submenu);
	menuBar->addElement("Edit", new CMenubar::MenuButton("New Matrix",
		GKey::KeyBind(GLFW_KEY_N, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editNewMatrix(); }));
	menuBar->addElement("Edit", new CMenubar::MenuButton("Properties",
		GKey::KeyBind(GLFW_KEY_F2, 0), []() { m_editor->getModel()->editMatrixProperties(); }));
	menuBar->addElement("Edit", new CMenubar::MenuButton("Delete Matrices",
		GKey::KeyBind(GLFW_KEY_DELETE), []() { m_editor->getModel()->deleteSelectedMatrices(); }));
	menuBar->addElement("Edit", new CMenubar::MenuDivider());
	menuBar->addElement("Edit", new CMenubar::MenuButton("Copy",
		GKey::KeyBind(GLFW_KEY_C, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editCopy(); }));
	menuBar->addElement("Edit", new CMenubar::MenuButton("Paste",
		GKey::KeyBind(GLFW_KEY_V, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editPaste(); }));
	menuBar->addElement("Edit", new CMenubar::MenuDivider());
	menuBar->addElement("Edit", new CMenubar::MenuButton("Undo Changes",
		GKey::KeyBind(GLFW_KEY_Z, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editUndo(); }));
	menuBar->addElement("Edit", new CMenubar::MenuButton("Redo Changes",
		GKey::KeyBind(GLFW_KEY_Y, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editRedo(); }));
	menuBar->addElement("Edit", new CMenubar::MenuDivider());
	menuBar->addElement("Edit", new CMenubar::Submenu("Transform"));
	menuBar->addElement("Edit\\Transform", new CMenubar::Submenu("Flip"));
	menuBar->addElement("Edit\\Transform\\Flip", new CMenubar::MenuButton("X-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_X); }));
	menuBar->addElement("Edit\\Transform\\Flip", new CMenubar::MenuButton("Y-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_Y); }));
	menuBar->addElement("Edit\\Transform\\Flip", new CMenubar::MenuButton("Z-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_Z); }));
	menuBar->addElement("Edit\\Transform", new CMenubar::Submenu("Rotate"));
	menuBar->addElement("Edit\\Transform\\Rotate", new CMenubar::MenuButton("90° X-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_X); }));
	menuBar->addElement("Edit\\Transform\\Rotate", new CMenubar::MenuButton("90° Y-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_Y); }));
	menuBar->addElement("Edit\\Transform\\Rotate", new CMenubar::MenuButton("90° Z-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_Z); }));
	menuBar->addElement("Edit\\Transform", new CMenubar::Submenu("Scale"));
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuButton("Half All",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(AXIS_X | AXIS_Y | AXIS_Z, 0.5f); }));
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuButton("Double All",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(AXIS_X | AXIS_Y | AXIS_Z, 2.f); }));
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuDivider());
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuButton("Half X-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(AXIS_X, 0.5f); }));
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuButton("Half Y-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(AXIS_Y, 0.5f); }));
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuButton("Half Z-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(AXIS_Z, 0.5f); }));
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuDivider());
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuButton("Double X-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(AXIS_X, 2.f); }));
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuButton("Double Y-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(AXIS_Y, 2.f); }));
	menuBar->addElement("Edit\\Transform\\Scale", new CMenubar::MenuButton("Double Z-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(AXIS_Z, 2.f); }));

	submenu = new CMenubar::Submenu("View");
	submenu->setPriority(9);
	submenu->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; });
	menuBar->addElement("", submenu);
	menuBar->addElement("View", new CMenubar::MenuButton("Focus to Selected",
		GKey::KeyBind(GLFW_KEY_SPACE, 0), []() { m_editor->getModel()->focus(); }));
	menuBar->addElement("View", new CMenubar::MenuDivider());
	menuBar->addElement("View", new CMenubar::MenuCheckbox("Matrix Grid",
		GKey::KeyBind(GLFW_KEY_G, GLFW_MOD_CONTROL), []() { m_editor->getModel()->toggleGrid(); },
		[&]() -> bool { return m_editor->getModel()->isGridVisible(); }));
	menuBar->addElement("View", new CMenubar::MenuCheckbox("Matrix Outline",
		GKey::KeyBind(GLFW_KEY_H, GLFW_MOD_CONTROL), []() { m_editor->getModel()->toggleOutline(); },
		[&]() -> bool { return m_editor->getModel()->isOutlineVisible(); }));
	menuBar->addElement("View", new CMenubar::MenuCheckbox("Camera Focus",
		GKey::KeyBind(), []() { m_editor->getModel()->toggleFocus(); },
		[&]() -> bool { return m_editor->getModel()->isFocusVisible(); }));
	menuBar->addElement("View", new CMenubar::MenuDivider());
	menuBar->addElement("View", new CMenubar::Submenu("Zoom"));
	menuBar->addElement("View\\Zoom", new CMenubar::MenuButton("Reset Zoom",
		GKey::KeyBind(GLFW_KEY_0, GLFW_MOD_CONTROL), []() { Camera::resetZoom(); }));
	menuBar->addElement("View\\Zoom", new CMenubar::MenuButton("Zoom In",
		GKey::KeyBind(GLFW_KEY_EQUAL, GLFW_MOD_CONTROL), []() { Camera::addZoom(5.f); }));
	menuBar->addElement("View\\Zoom", new CMenubar::MenuButton("Zoom Out",
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

	_toolbar = new CButtonRadio("TOOLBAR_MAIN", "", { 4, 4 }, { 28, 28 }, { 0, 28 });
	workContainer->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_LEFT);
	_toolbar->setPressFunction([]() {
		m_editor->getModel()->updateTool();
		});

	for (Tool* tool : MTool::getToolList()) {
		_toolbar->addButton(tool->getIcon(), tool->getParent(), tool->getName(), tool->getDescription(), tool->getKeyBind());
	}

	// Color panel

	workContainer->addComponent(new ContainerPanel("GUI_COLOR", "Colors", { -8, 32 }, { 256, 200 },
		(Sint8)Component::BorderFlag::ALL), Component::Anchor::TOP_RIGHT, Component::Anchor::NONE)
		->setPriorityLayer(6)
		->setPrimaryPos("Middle");
	m_colorOverlay = new ColorOverlay({ 48, 20 }, { 160, 160 });
	workContainer->findComponent("GUI_COLOR")->addComponent(m_colorOverlay);

	// Matrices list and buttons

	workContainer->addComponent(new ContainerPanel("GUI_MATRICES", "Matrices", { -8, -8 }, { 256, 192 },
		(Sint8)Component::BorderFlag::ALL), Component::Anchor::BOTTOM_RIGHT, Component::Anchor::NONE)
		->setPrimaryPos("Middle");
	workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_MERGE_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\Merge.png"), { -122, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
			if (m_editor->getModel()) m_editor->getModel()->editMergeMatrix();
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Merge Matrices");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_NEW_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\Add.png"), { -98, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
			if (m_editor->getModel()) m_editor->getModel()->editNewMatrix();
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("New Matrix (Ctrl+N)");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_DELETE_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\Delete.png"), { -74, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
			if (m_editor->getModel()) m_editor->getModel()->deleteSelectedMatrices();
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Delete Matrices (Del)");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_PROPERTIES", "",
		MTexture::getTexture("gui\\icon\\matrices\\Properties.png"), { -50, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
			if (m_editor->getModel()) m_editor->getModel()->editMatrixProperties();
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Edit Properties (F2)");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_MOVE_UP_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\ArrowUp.png"), { -26, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
			if (m_editor->getModel()) m_editor->getModel()->moveMatrix(true);
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Move Up");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_MOVE_DOWN_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\ArrowDown.png"), { -2, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
			if (m_editor->getModel()) m_editor->getModel()->moveMatrix(false);
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Move Down");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CList("LIST_MATRICES", "",
		{ 0, 0 }, { 0, -24 }, 24), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setPressFunction([]() {
		if (m_editor->getModel()) m_editor->getModel()
			->setSelectedMatrix(m_container->findComponent("GUI_WORKSPACE\\GUI_MATRICES\\LIST_MATRICES")->getSelectedItem());
			})->setHoldFunction([]() {
				if (m_editor->getModel()) m_editor->getModel()->updateVisibilityLtoM();
				});

	return m_container;
}
