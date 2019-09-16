#include "engine\editor\model\menu\ModelOverlay.h"
#include "engine\editor\camera\Camera.h"
#include "engine\editor\menu\EditorOverlay.h"
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

	menuBar->addElement("File", new CMenubar::MenuButton("Close",
		GKey::KeyBind(), []() { m_editor->closeSelectedProject(); }))
		->setVisibleFunction(p_editor->isModel)
		->setPriority(4);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction(p_editor->isModel)
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Save",
		GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL), []() { m_editor->fileSave(); }))
		->setVisibleFunction(p_editor->isModel)
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Save As...",
		GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->fileSaveAs(); }))
		->setVisibleFunction(p_editor->isModel)
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction(p_editor->isModel)
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Import...",
		GKey::KeyBind(), []() { m_editor->getModel()->fileAdd(); }))
		->setVisibleFunction(p_editor->isModel)
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::Submenu("Model_Export", "Export"))
		->setVisibleFunction(p_editor->isModel)
		->setPriority(5);
	menuBar->addElement("File\\Model_Export", new CMenubar::MenuButton("Wavefront (.obj)",
		GKey::KeyBind(), []() { m_editor->getModel()->fileExport(LFormat::ExportType::OBJ); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("File\\Model_Export", new CMenubar::MenuButton("Stanford (.ply)",
		GKey::KeyBind(), []() { m_editor->getModel()->fileExport(LFormat::ExportType::PLY); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("File\\Model_Export", new CMenubar::MenuButton("Stl (.stl)",
		GKey::KeyBind(), []() { m_editor->getModel()->fileExport(LFormat::ExportType::STL); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction(p_editor->isModel)
		->setPriority(5);

	menuBar->addElement("Edit", new CMenubar::MenuButton("New Matrix...",
		GKey::KeyBind(GLFW_KEY_N, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editNewMatrix(); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuDivider())
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuButton("Copy",
		GKey::KeyBind(GLFW_KEY_C, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editCopy(); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuButton("Cut",
		GKey::KeyBind(GLFW_KEY_X, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editCut(); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuButton("Paste",
		GKey::KeyBind(GLFW_KEY_V, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editPaste(); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuDivider())
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuButton("Select All",
		GKey::KeyBind(GLFW_KEY_A, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editSelectAll(); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuDivider())
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuButton("Undo Changes",
		GKey::KeyBind(GLFW_KEY_Z, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editUndo(); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuButton("Redo Changes",
		GKey::KeyBind(GLFW_KEY_Y, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editRedo(); }))
		->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("Edit", new CMenubar::MenuDivider())
		->setVisibleFunction(p_editor->isModel);

	submenu = new CMenubar::Submenu("Model_View", "View");
	submenu->setPriority(2);
	submenu->setVisibleFunction(p_editor->isModel);
	menuBar->addElement("", submenu);
	menuBar->addElement("Model_View", new CMenubar::MenuButton("Focus to Selected",
		GKey::KeyBind(GLFW_KEY_SPACE, 0), []() { m_editor->getModel()->focus(); }));
	menuBar->addElement("Model_View", new CMenubar::MenuDivider());
	menuBar->addElement("Model_View", new CMenubar::MenuCheckbox("Matrix Grid",
		GKey::KeyBind(GLFW_KEY_G, GLFW_MOD_CONTROL), []() { m_editor->getModel()->toggleGrid(); },
		[&]() -> bool { return m_editor->getModel()->isGridVisible(); }));
	menuBar->addElement("Model_View", new CMenubar::MenuCheckbox("Matrix Outline",
		GKey::KeyBind(GLFW_KEY_H, GLFW_MOD_CONTROL), []() { m_editor->getModel()->toggleOutline(); },
		[&]() -> bool { return m_editor->getModel()->isOutlineVisible(); }));
	menuBar->addElement("Model_View", new CMenubar::MenuCheckbox("Matrix Wireframe",
		GKey::KeyBind(), []() { m_editor->getModel()->toggleWireframe(); },
		[&]() -> bool { return m_editor->getModel()->isWireframe(); }));
	menuBar->addElement("Model_View", new CMenubar::MenuCheckbox("Camera Focus",
		GKey::KeyBind(), []() { m_editor->getModel()->toggleFocus(); },
		[&]() -> bool { return m_editor->getModel()->isFocusVisible(); }));
	menuBar->addElement("Model_View", new CMenubar::MenuDivider());
	menuBar->addElement("Model_View", new CMenubar::MenuButton("Reset Camera",
		GKey::KeyBind(), []() { Camera::reset(); }));
	menuBar->addElement("Model_View", new CMenubar::Submenu("View_Zoom", "Zoom"));
	menuBar->addElement("Model_View\\View_Zoom", new CMenubar::MenuButton("Reset Zoom",
		GKey::KeyBind(GLFW_KEY_0, GLFW_MOD_CONTROL), []() { Camera::resetZoom(); }));
	menuBar->addElement("Model_View\\View_Zoom", new CMenubar::MenuButton("Zoom In",
		GKey::KeyBind(GLFW_KEY_EQUAL, GLFW_MOD_CONTROL), []() { Camera::addZoom(5.f); }));
	menuBar->addElement("Model_View\\View_Zoom", new CMenubar::MenuButton("Zoom Out",
		GKey::KeyBind(GLFW_KEY_MINUS, GLFW_MOD_CONTROL), []() { Camera::addZoom(-5.f); }));
	menuBar->addElement("Model_View", new CMenubar::MenuDivider());
	menuBar->addElement("Model_View", new CMenubar::Submenu("Model_Rotation", "Auto Rotation"));
	menuBar->addElement("Model_View\\Model_Rotation", new CMenubar::MenuButton("Reset Speed",
		GKey::KeyBind(), []() { Camera::resetAutoRotation(); }));
	menuBar->addElement("Model_View\\Model_Rotation", new CMenubar::MenuButton("Add Counter-clockwise",
		GKey::KeyBind(GLFW_KEY_LEFT_BRACKET), []() { Camera::addAutoRotation(15.f); }));
	menuBar->addElement("Model_View\\Model_Rotation", new CMenubar::MenuButton("Add Clockwise",
		GKey::KeyBind(GLFW_KEY_RIGHT_BRACKET), []() { Camera::addAutoRotation(-15.f); }));
	
	submenu = new CMenubar::Submenu("Model_Matrix", "Matrix");
	submenu->setVisibleFunction([]() -> bool { return m_editor->isModel(); });
	submenu->setEnabledFunction([]() -> bool { return m_editor->getModel()->getSelectedMatrix(); });
	submenu->setPriority(5);
	menuBar->addElement("", submenu);
	menuBar->addElement("Model_Matrix", new CMenubar::MenuButton("Delete",
		GKey::KeyBind(GLFW_KEY_DELETE), []() { m_editor->getModel()->deleteSelectedMatrices(); }));
	menuBar->addElement("Model_Matrix", new CMenubar::MenuButton("Resize...",
		GKey::KeyBind(), []() { m_editor->getModel()->matrixSize(); }));
	menuBar->addElement("Model_Matrix", new CMenubar::MenuDivider());
	menuBar->addElement("Model_Matrix", new CMenubar::MenuButton("Hide Matrices",
		GKey::KeyBind(), []() { m_editor->getModel()->setSelectedVisibility(false); }));
	menuBar->addElement("Model_Matrix", new CMenubar::MenuButton("Show Matrices",
		GKey::KeyBind(), []() { m_editor->getModel()->setSelectedVisibility(true); }));
	menuBar->addElement("Model_Matrix", new CMenubar::MenuDivider());
	menuBar->addElement("Model_Matrix", new CMenubar::Submenu("Model_Transform", "Transform"));
	menuBar->addElement("Model_Matrix\\Model_Transform", new CMenubar::Submenu("Model_Flip", "Flip"));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Flip", new CMenubar::MenuButton("X-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_X); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Flip", new CMenubar::MenuButton("Y-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_Y); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Flip", new CMenubar::MenuButton("Z-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_Z); }));
	menuBar->addElement("Model_Matrix\\Model_Transform", new CMenubar::Submenu("Model_Rotate", "Rotate"));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Rotate", new CMenubar::MenuButton("90° X-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_X); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Rotate", new CMenubar::MenuButton("90° Y-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_Y); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Rotate", new CMenubar::MenuButton("90° Z-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_Z); }));
	menuBar->addElement("Model_Matrix\\Model_Transform", new CMenubar::Submenu("Model_Scale", "Scale"));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuButton("Half All",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(glm::vec3(0.5f, 0.5f, 0.5f)); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuButton("Double All",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(glm::vec3(2.f, 2.f, 2.f)); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuDivider());
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuButton("Half X-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(glm::vec3(0.5f, 1, 1)); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuButton("Half Y-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(glm::vec3(1, 0.5f, 1)); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuButton("Half Z-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(glm::vec3(1, 1, 0.5f)); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuDivider());
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuButton("Double X-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(glm::vec3(2.f, 1, 1)); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuButton("Double Y-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(glm::vec3(1, 2.f, 1)); }));
	menuBar->addElement("Model_Matrix\\Model_Transform\\Model_Scale", new CMenubar::MenuButton("Double Z-Axis",
		GKey::KeyBind(), []() { m_editor->getModel()->scaleMatrix(glm::vec3(1, 1, 2.f)); }));
	menuBar->addElement("Model_Matrix", new CMenubar::MenuDivider());
	menuBar->addElement("Model_Matrix", new CMenubar::MenuButton("Properties...",
		GKey::KeyBind(GLFW_KEY_F2, 0), []() { m_editor->getModel()->editMatrixProperties(); }));

	submenu = new CMenubar::Submenu("Model_Generate", "Generate");
	submenu->setPriority(7);
	submenu->setVisibleFunction([]() -> bool { return m_editor->isModel(); });
	submenu->setEnabledFunction([]() -> bool { return m_editor->getModel()->getSelectedMatrix(); });
	menuBar->addElement("", submenu);

	size_t rootLen = std::string(LDirectory::getProjectPath() + "res\\script\\generate\\").length();
	std::string* genName;
	for (std::string path : LDirectory::getFilesInDirectory(LDirectory::getProjectPath() + "res\\script\\generate\\", ".lua")) {
		genName = new std::string(path.substr(rootLen));
		*genName = genName->substr(0, genName->length() - 4);
		menuBar->addElement("Model_Generate",
			new CMenubar::MenuButton(*genName, GKey::KeyBind(), [genName]() { m_editor->getModel()->loadScriptFile("generate\\" + *genName +".lua"); }));
	}
	menuBar->addElement("Model_Generate", new CMenubar::MenuButton("Custom Script...",
		GKey::KeyBind(), []() { m_editor->getModel()->generateCustom(); }));

	auto contSettings = static_cast<ContainerPanel*>(EditorOverlay::getContainer()->findComponent("GUI_TOP\\GUI_SETTINGS"));
	contSettings->addComponent(new CButton("UNDO", "", MTexture::getTexture("gui\\icon\\tool\\Undo.png"), Vector2<Sint32>(2, 2), Vector2<Sint32>(20, 20), CButton::RenderStyle::ALL));

	// Workspace section (Middle of screen)

	Container* workContainer = static_cast<Container*>(m_container->addComponent(new Container("GUI_WORKSPACE",
		{ 0, EditorOverlay::getContainer()->findComponent("GUI_TOP")->getSize().y },
		{ 0, -EditorOverlay::getContainer()->findComponent("PANEL_INFOBAR")->getSize().y }, []() { return true; }),
		Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT));
	workContainer->setElementPos("workspace");

	// Toolbar (Tools on left side of workspace)

	CButtonRadio* _toolbar;

	workContainer->addComponent(new ContainerPanel("GUI_TOOLBAR", "", { 0, 0 }, { 36, MTool::getParentCount() * 28 + 8 },
		(Sint8)Component::BorderFlag::ALL), Component::Anchor::MIDDLE_LEFT, Component::Anchor::NONE)
		->setElementPos("workspace");

	_toolbar = new CButtonRadio("TOOLBAR_MAIN", "", { 4, 4 }, { 28, 28 }, { 0, 28 });
	workContainer->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_LEFT);
	_toolbar->setPressFunction([]() {
		m_editor->getModel()->updateTool();
		});

	for (Tool* tool : MTool::getToolList()) {
		_toolbar->addButton(tool->getIcon(), tool->getParent(), tool->getName(), tool->getDescription(), tool->getKeyBind());
	}

	// Toolbar additional settings (Top of workspace)


	// Color panel

	workContainer->addComponent(new ContainerPanel("GUI_COLOR", "Colors", { -8, 32 }, { 256, 200 },
		(Sint8)Component::BorderFlag::ALL), Component::Anchor::TOP_RIGHT, Component::Anchor::NONE)
		->setPriorityLayer(6)
		->setElementPos("workspace");
	//workContainer->findComponent("GUI_COLOR")->addComponent(new CButtonToggle("VOXEL_EMPTY", "Empty", {}, {}, CButton::RenderStyle::ALL));
	m_colorOverlay = new ColorOverlay({ 48, 20 }, { 160, 160 });
	workContainer->findComponent("GUI_COLOR")->addComponent(m_colorOverlay);

	// Matrices list and buttons

	workContainer->addComponent(new ContainerPanel("GUI_MATRICES", "Matrices", { -8, -8 }, { 256, 192 },
		(Sint8)Component::BorderFlag::ALL), Component::Anchor::BOTTOM_RIGHT, Component::Anchor::NONE)
		->setElementPos("workspace");
	workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_MERGE_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\Merge.png"), { -50, 0 }, { 24, 24 }, CButton::RenderStyle::EMPTY, []() {
			if (m_editor->getModel()) m_editor->getModel()->editMergeMatrix();
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Merge Matrices");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_NEW_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\Add.png"), { -122, 0 }, { 24, 24 }, CButton::RenderStyle::EMPTY, []() {
			if (m_editor->getModel()) m_editor->getModel()->editNewMatrix();
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("New Matrix (Ctrl+N)");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_DELETE_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\Delete.png"), { -98, 0 }, { 24, 24 }, CButton::RenderStyle::EMPTY, []() {
			if (m_editor->getModel()) m_editor->getModel()->deleteSelectedMatrices();
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Delete Matrices (Del)");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_PROPERTIES", "",
		MTexture::getTexture("gui\\icon\\matrices\\Properties.png"), { -74, 0 }, { 24, 24 }, CButton::RenderStyle::EMPTY, []() {
			if (m_editor->getModel()) m_editor->getModel()->editMatrixProperties();
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Edit Properties (F2)");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_MOVE_UP_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\ArrowUp.png"), { -26, 0 }, { 24, 24 }, CButton::RenderStyle::EMPTY, []() {
			if (m_editor->getModel()) m_editor->getModel()->moveMatrix(true);
		}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Move Up");

		workContainer->findComponent("GUI_MATRICES")->addComponent(new CButton("BUTTON_MOVE_DOWN_MATRIX", "",
		MTexture::getTexture("gui\\icon\\matrices\\ArrowDown.png"), { -2, 0 }, { 24, 24 }, CButton::RenderStyle::EMPTY, []() {
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
