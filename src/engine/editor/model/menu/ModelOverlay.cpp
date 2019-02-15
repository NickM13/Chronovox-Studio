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

	m_container = new Container("GUI_MODEL", { 0, 0 }, {}, true);

	CMenubar* menuBar = new CMenubar("MENUBAR_MAIN", { 0, 32 }, { 0, 18 });
	menuBar->addButton("", "File")
		->addButton("File", "New")
		->addButton("File\\New", "Model", GKey::KeyBind(), []() { m_editor->fileNewModel(); })
		->addButton("File", "Open")
		->addButton("File\\Open", "Open", GKey::KeyBind(GLFW_KEY_O, GLFW_MOD_CONTROL), []() { m_editor->fileOpen(); })
		->addButton("File\\Open", "Add", GKey::KeyBind(GLFW_KEY_O, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->getModel()->fileAdd(); })
		->addButton("File", "Save", GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL), []() { m_editor->fileSave(); })
		->addButton("File", "Save As", GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->fileSaveAs(); })
		->addButton("File", "Close", GKey::KeyBind(), []() { m_editor->closeSelectedProject(); })
		->addButton("File", "Exit", GKey::KeyBind(GLFW_KEY_F4, GLFW_MOD_ALT), []() { if (m_editor->getModel()) m_editor->getModel()->fileExit(); });
	if (GScreen::m_developer) {
		menuBar->addButton("File\\New", "Animation", GKey::KeyBind(), []() { m_editor->fileNewAnimation(); });
	}
	menuBar->addButton("", "Edit")
		->addButton("Edit", "New Matrix", GKey::KeyBind(GLFW_KEY_N, GLFW_MOD_CONTROL), []() { if (m_editor->getModel()) m_editor->getModel()->editNewMatrix(); })
		->addButton("Edit", "Properties", GKey::KeyBind(GLFW_KEY_F2, 0), []() { if (m_editor->getModel()) m_editor->getModel()->editMatrixProperties(); })
		->addButton("Edit", "Undo", GKey::KeyBind(GLFW_KEY_Z, GLFW_MOD_CONTROL), []() { if (m_editor->getModel()) m_editor->getModel()->editUndo(); })
		->addButton("Edit", "Redo", GKey::KeyBind(GLFW_KEY_Y, GLFW_MOD_CONTROL), []() { if (m_editor->getModel()) m_editor->getModel()->editRedo(); })
		->addButton("Edit", "Flip")
		->addButton("Edit\\Flip", "Flip X", GKey::KeyBind(), []() { if (m_editor->getModel()) m_editor->getModel()->flipMatrix(AXIS_X); })
		->addButton("Edit\\Flip", "Flip Y", GKey::KeyBind(), []() { if (m_editor->getModel()) m_editor->getModel()->flipMatrix(AXIS_Y); })
		->addButton("Edit\\Flip", "Flip Z", GKey::KeyBind(), []() { if (m_editor->getModel()) m_editor->getModel()->flipMatrix(AXIS_Z); })
		->addButton("Edit", "Rotate")
		->addButton("Edit\\Rotate", "Rotate X", GKey::KeyBind(), []() { if (m_editor->getModel()) m_editor->getModel()->rotateMatrix(AXIS_X); })
		->addButton("Edit\\Rotate", "Rotate Y", GKey::KeyBind(), []() { if (m_editor->getModel()) m_editor->getModel()->rotateMatrix(AXIS_Y); })
		->addButton("Edit\\Rotate", "Rotate Z", GKey::KeyBind(), []() { if (m_editor->getModel()) m_editor->getModel()->rotateMatrix(AXIS_Z); });
	menuBar->addButton("", "View")
		->addButton("View", "Focus Matrix", GKey::KeyBind(GLFW_KEY_SPACE, 0), []() { if (m_editor->getModel()) m_editor->getModel()->focus(); })
		->addButton("View", "Voxel Grid", GKey::KeyBind(GLFW_KEY_G, GLFW_MOD_CONTROL), []() { if (m_editor->getModel()) m_editor->getModel()->toggleGrid(); }, []() -> bool { if (m_editor->getModel()) return m_editor->getModel()->isGridVisible(); return false; })
		->addButton("View", "Matrix Outline", GKey::KeyBind(GLFW_KEY_H, GLFW_MOD_CONTROL), []() { if (m_editor->getModel()) m_editor->getModel()->toggleOutline(); }, []() -> bool { if (m_editor->getModel()) return m_editor->getModel()->isOutlineVisible(); return false; })
		->addButton("View", "Hide On Select", GKey::KeyBind(), []() { if (m_editor->getModel()) m_editor->getModel()->toggleHideOnSelect(); }, []() -> bool { if (m_editor->getModel()) return m_editor->getModel()->isHideOnSelect(); return false; })
		->addButton("View", "Zoom")
		->addButton("View\\Zoom", "Reset Zoom", GKey::KeyBind(GLFW_KEY_0, GLFW_MOD_CONTROL), []() { Camera::resetZoom(); })
		->addButton("View\\Zoom", "Zoom In", GKey::KeyBind(GLFW_KEY_EQUAL, GLFW_MOD_CONTROL), []() { Camera::addZoom(5.f); })
		->addButton("View\\Zoom", "Zoom Out", GKey::KeyBind(GLFW_KEY_MINUS, GLFW_MOD_CONTROL), []() { Camera::addZoom(-5.f); });
	//->addButton("View", "Toggle Wireframe", "Ctrl+O", []() { if (m_editor->getModel()) m_editor->getModel()->toggleWireframe(); });
	menuBar->addButton("", "Help")
		->addButton("Help", "About Voxel Model Editor", GKey::KeyBind(), []() { EditorOverlay::getContainer()->openDialog(AboutDialog::getInstance().getDialog()); });
	m_container->addComponent(menuBar, Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)->setPriorityLayer(16);

	CButtonRadio* _toolbar;

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", { 0, 69 }, { 60, -20 },
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::RIGHT + (Sint8)Component::BorderFlag::TOP), Component::Anchor::NONE, Component::Anchor::BOTTOM_LEFT);

	_toolbar = new CButtonRadio("TOOLBAR_MAIN", "", { 4, 4 }, { 24, 24 }, { 0, 24 });
	m_container->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_LEFT);
	_toolbar->setPressFunction([]() {
		Tool* ptool = MTool::getTool(m_container->findComponent("GUI_TOOLBAR\\TOOLBAR_MAIN")->getPrevSelectedItem());
		Tool* ctool = MTool::getTool(m_container->findComponent("GUI_TOOLBAR\\TOOLBAR_MAIN")->getSelectedItem());
		if (ptool->hasSubmenu()) {
			switch (ptool->getType()) {
			case Tool::ToolType::VOXEL: m_container->findComponent("GUI_TOOLBAR\\TOOLBAR_VOXEL")->setVisible(false); break;
			case Tool::ToolType::MATRIX: m_container->findComponent("GUI_TOOLBAR\\TOOLBAR_MATRIX")->setVisible(false); break;
			}
		}
		if (ctool->hasSubmenu()) {
			switch (ctool->getType()) {
			case Tool::ToolType::VOXEL: m_container->findComponent("GUI_TOOLBAR\\TOOLBAR_VOXEL")->setVisible(true); break;
			case Tool::ToolType::MATRIX: m_container->findComponent("GUI_TOOLBAR\\TOOLBAR_MATRIX")->setVisible(true); break;
			}
		}
		if (m_editor->getModel()) m_editor->getModel()->updateTool();
	});
	for (Tool* tool : MTool::getToolList()) {
		_toolbar->addButton(tool->getIcon())->setTooltip(tool->getDescription());
	}

	_toolbar = new CButtonRadio("TOOLBAR_VOXEL", "", { 32, 4 }, { 24, 24 }, { 0, 24 });
	_toolbar->setVisible(true);
	_toolbar->setPressFunction([]() {
		if (m_editor->getModel()) m_editor->getModel()->updateTool();
	});
	m_container->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_LEFT);
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\VoxelSingle.png"))->setTooltip("Single - 1");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\VoxelBox.png"))->setTooltip("Box - 2");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\VoxelFill.png"))->setTooltip("Fill - 3");

	_toolbar = new CButtonRadio("TOOLBAR_MATRIX", "", { 32, 4 }, { 24, 24 }, { 0, 24 });
	_toolbar->setVisible(false);
	m_container->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_LEFT);
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixSelect.png"))->setTooltip("Select Matrix - 1");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixMove.png"))->setTooltip("Move Matrix - 2");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixResize.png"))->setTooltip("Resize Matrix - 3");

	_toolbar = new CButtonRadio("TOOLBAR_MATRIX", "", { 32, 4 }, { 24, 24 }, { 0, 24 });
	_toolbar->setVisible(false);
	m_container->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_LEFT);
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixSelect.png"))->setTooltip("Select Matrix - 1");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixMove.png"))->setTooltip("Move Matrix - 2");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixResize.png"))->setTooltip("Resize Matrix - 3");

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", { -256, 69 }, { 0, -20 },
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::LEFT + (Sint8)Component::BorderFlag::TOP), Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT);

	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_COLOR", "", { 0, 0 }, { 256, 200 },
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::BOTTOM))->setPriorityLayer(6);
	m_colorOverlay = new ColorOverlay({ 80, 20 }, { 160, 160 });
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(m_colorOverlay)->setPressFunction([]() {
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\R")->setValue((m_colorOverlay->getR()));
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\G")->setValue((m_colorOverlay->getG()));
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\B")->setValue((m_colorOverlay->getB()));
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\H")->setValue((m_colorOverlay->getHue()));
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->setValue((m_colorOverlay->getSat()));
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\V")->setValue((m_colorOverlay->getVal()));
	});
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("R", "R: ",
		Vector2<Sint32>(22, 24), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 255), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setR(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\R")->getValue()); });
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("G", "G: ",
		Vector2<Sint32>(22, 50), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 255), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setG(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\G")->getValue()); });
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("B", "B: ",
		Vector2<Sint32>(22, 76), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 255), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setB(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\B")->getValue()); });
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("H", "H: ",
		Vector2<Sint32>(22, 102), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 360), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setHue(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\H")->getValue()); });
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("V", "V: ",
		Vector2<Sint32>(22, 128), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 100), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setVal(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\V")->getValue()); });
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("S", "S: ",
		Vector2<Sint32>(22, 154), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 100), NumberField::NumType::INT))
		->setPressFunction([]() {
		m_colorOverlay->setSat(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->getValue());
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\SS")->setValue(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->getValue());
	});
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new CSliderV("SS", "", Vector2<Sint32>(64, 24), 152, 100))
		->setPressFunction([]() {
		m_colorOverlay->setSat(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->getValue());
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->setValue(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\SS")->getValue());
	});

	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_MATRICES", "Matrices",
		{ 0, 200 }, { 0, 0 }, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::TOP), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT);
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_NEW_MATRIX", "",
		MTexture::getTexture("gui\\icon\\tool\\Add.png"), { -96, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
		m_container->setPauseScreen("DIALOG_NEWMATRIX");
	}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("New Matrix (Ctrl+N)");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_DELETE_MATRIX", "",
		MTexture::getTexture("gui\\icon\\tool\\Delete.png"), { -72, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
		if (m_editor->getModel()) m_editor->getModel()->deleteSelectedMatrices();
	}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Delete Matrices (Del)");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_PROPERTIES", "",
		MTexture::getTexture("gui\\icon\\tool\\Properties.png"), { -48, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
		m_container->setPauseScreen("DIALOG_PROPERTIES");
	}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Edit Properties (F2)");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_MOVE_UP_MATRIX", "",
		MTexture::getTexture("gui\\icon\\tool\\ArrowUp.png"), { -24, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
		if (m_editor->getModel()) m_editor->getModel()->moveMatrix(true);
	}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Move Up");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_MOVE_DOWN_MATRIX", "",
		MTexture::getTexture("gui\\icon\\tool\\ArrowDown.png"), { 0, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
		if (m_editor->getModel()) m_editor->getModel()->moveMatrix(false);
	}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Move Down");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CList("LIST_MATRICES", "",
		{ 0, 0 }, { 0, -24 }, 20), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT)
		->setPressFunction([]() {
		if (m_editor->getModel()) m_editor->getModel()->setSelectedMatrix(m_container->findComponent("GUI_DETAILS\\GUI_MATRICES\\LIST_MATRICES")->getSelectedItem());
	});

	return m_container;
}
