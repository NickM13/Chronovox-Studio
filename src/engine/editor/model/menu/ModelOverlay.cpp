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

	CMenubar* menuBar = static_cast<CMenubar*>(EditorOverlay::getContainer()->findComponent("MENUBAR_MAIN"));
	CMenubar::Submenu* submenu;

	menuBar->addElement("File", new CMenubar::MenuButton("Add", GKey::KeyBind(GLFW_KEY_O, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->getModel()->fileAdd(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(4);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(4);
	menuBar->addElement("File", new CMenubar::MenuButton("Close", GKey::KeyBind(), []() { m_editor->closeSelectedProject(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(4);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Save", GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL), []() { m_editor->fileSave(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuButton("Save As...", GKey::KeyBind(GLFW_KEY_S, GLFW_MOD_CONTROL + GLFW_MOD_SHIFT), []() { m_editor->fileSaveAs(); }))
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(5);
	menuBar->addElement("File", new CMenubar::MenuDivider())
		->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; })
		->setPriority(5);

	submenu = new CMenubar::Submenu("Edit");
	submenu->setVisibleFunction([]() -> bool { return (m_editor->getModel() != 0); });
	submenu->setPriority(1);
	menuBar->addElement("", submenu);
	menuBar->addElement("Edit", new CMenubar::MenuButton("New Matrix", GKey::KeyBind(GLFW_KEY_N, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editNewMatrix(); }));
	menuBar->addElement("Edit", new CMenubar::MenuButton("Properties", GKey::KeyBind(GLFW_KEY_F2, 0), []() { m_editor->getModel()->editMatrixProperties(); }));
	menuBar->addElement("Edit", new CMenubar::MenuDivider());
	menuBar->addElement("Edit", new CMenubar::MenuButton("Undo Changes", GKey::KeyBind(GLFW_KEY_Z, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editUndo(); }));
	menuBar->addElement("Edit", new CMenubar::MenuButton("Redo Changes", GKey::KeyBind(GLFW_KEY_Y, GLFW_MOD_CONTROL), []() { m_editor->getModel()->editRedo(); }));
	menuBar->addElement("Edit", new CMenubar::MenuDivider());
	menuBar->addElement("Edit", new CMenubar::Submenu("Flip"));
	menuBar->addElement("Edit\\Flip", new CMenubar::MenuButton("X-Axis", GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_X); }));
	menuBar->addElement("Edit\\Flip", new CMenubar::MenuButton("Y-Axis", GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_Y); }));
	menuBar->addElement("Edit\\Flip", new CMenubar::MenuButton("Z-Axis", GKey::KeyBind(), []() { m_editor->getModel()->flipMatrix(AXIS_Z); }));
	menuBar->addElement("Edit", new CMenubar::Submenu("Rotate"));
	menuBar->addElement("Edit\\Rotate", new CMenubar::MenuButton("90° X-Axis", GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_X); }));
	menuBar->addElement("Edit\\Rotate", new CMenubar::MenuButton("90° Y-Axis", GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_Y); }));
	menuBar->addElement("Edit\\Rotate", new CMenubar::MenuButton("90° Z-Axis", GKey::KeyBind(), []() { m_editor->getModel()->rotateMatrix(AXIS_Z); }));

	submenu = new CMenubar::Submenu("View");
	submenu->setPriority(9);
	submenu->setVisibleFunction([&]() -> bool { return p_editor->getModel() != 0; });
	menuBar->addElement("", submenu);
	menuBar->addElement("View", new CMenubar::MenuButton("Focus Matrix", GKey::KeyBind(GLFW_KEY_SPACE, 0), []() { m_editor->getModel()->focus(); }));
	menuBar->addElement("View", new CMenubar::MenuDivider());
	menuBar->addElement("View", new CMenubar::MenuCheckbox("Matrix Grid", GKey::KeyBind(GLFW_KEY_G, GLFW_MOD_CONTROL), []() { m_editor->getModel()->toggleGrid(); }, [&]() -> bool { return m_editor->getModel()->isGridVisible(); }));
	menuBar->addElement("View", new CMenubar::MenuCheckbox("Matrix Outline", GKey::KeyBind(GLFW_KEY_H, GLFW_MOD_CONTROL), []() { m_editor->getModel()->toggleOutline(); }, [&]() -> bool { return m_editor->getModel()->isOutlineVisible(); }));
	menuBar->addElement("View", new CMenubar::MenuCheckbox("Hide On Select", GKey::KeyBind(), []() { m_editor->getModel()->toggleHideOnSelect(); }, [&]() -> bool { return m_editor->getModel()->isHideOnSelect(); }));
	menuBar->addElement("View", new CMenubar::MenuDivider());
	menuBar->addElement("View", new CMenubar::Submenu("Zoom"));
	menuBar->addElement("View\\Zoom", new CMenubar::MenuButton("Reset Zoom", GKey::KeyBind(GLFW_KEY_0, GLFW_MOD_CONTROL), []() { Camera::resetZoom(); }));
	menuBar->addElement("View\\Zoom", new CMenubar::MenuButton("Zoom In", GKey::KeyBind(GLFW_KEY_EQUAL, GLFW_MOD_CONTROL), []() { Camera::addZoom(5.f); }));
	menuBar->addElement("View\\Zoom", new CMenubar::MenuButton("Zoom Out", GKey::KeyBind(GLFW_KEY_MINUS, GLFW_MOD_CONTROL), []() { Camera::addZoom(-5.f); }));

	CButtonRadio* _toolbar;

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", { 0, 69 }, { 60, -25 },
		(Sint8)Component::BorderFlag::RIGHT + (Sint8)Component::BorderFlag::TOP), Component::Anchor::NONE, Component::Anchor::BOTTOM_LEFT)
		->setPrimaryPos("Middle");

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
	_toolbar->setPressFunction([]() {
		if (m_editor->getModel()) m_editor->getModel()->updateTool();
	});
	m_container->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_LEFT);
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixSelect.png"))->setTooltip("Select Matrix - 1");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixMove.png"))->setTooltip("Move Matrix - 2");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixResize.png"))->setTooltip("Resize Matrix - 3");

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", { -256, 69 }, { 0, -25 },
		(Sint8)Component::BorderFlag::LEFT + (Sint8)Component::BorderFlag::TOP), Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT)
		->setPrimaryPos("Middle");

	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_COLOR", "", { 0, 0 }, { 256, 200 },
		(Sint8)Component::BorderFlag::BOTTOM))->setPriorityLayer(6);
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
		->setPressFunction([]() { m_colorOverlay->setR(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\R")->getValue()); })->setTooltip("Red");
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("G", "G: ",
		Vector2<Sint32>(22, 50), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 255), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setG(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\G")->getValue()); })->setTooltip("Green");
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("B", "B: ",
		Vector2<Sint32>(22, 76), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 255), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setB(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\B")->getValue()); })->setTooltip("Blue");
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("H", "H: ",
		Vector2<Sint32>(22, 102), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 360), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setHue(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\H")->getValue()); })->setTooltip("Hue");
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("V", "V: ",
		Vector2<Sint32>(22, 128), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 100), NumberField::NumType::INT))
		->setPressFunction([]() { m_colorOverlay->setVal(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\V")->getValue()); })->setTooltip("Value");
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("S", "S: ",
		Vector2<Sint32>(22, 154), Vector2<Sint32>(32, 1), Vector2<GLfloat>(0, 100), NumberField::NumType::INT))
		->setPressFunction([]() {
		m_colorOverlay->setSat(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->getValue());
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\SS")->setValue(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->getValue());
	})->setTooltip("Saturation");
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new CSliderV("SS", "", Vector2<Sint32>(64, 24), 152, 100))
		->setPressFunction([]() {
		m_colorOverlay->setSat(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->getValue());
		m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\S")->setValue(m_container->findComponent("GUI_DETAILS\\GUI_COLOR\\SS")->getValue());
	});

	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_MATRICES", "Matrices",
		{ 0, 200 }, { 0, 0 }, (Sint8)Component::BorderFlag::TOP), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT);
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_NEW_MATRIX", "",
		MTexture::getTexture("gui\\icon\\tool\\Add.png"), { -96, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
		if (m_editor->getModel()) m_editor->getModel()->editNewMatrix();
	}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("New Matrix (Ctrl+N)");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_DELETE_MATRIX", "",
		MTexture::getTexture("gui\\icon\\tool\\Delete.png"), { -72, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
		if (m_editor->getModel()) m_editor->getModel()->deleteSelectedMatrices();
	}), Component::Anchor::BOTTOM_RIGHT)->setTooltip("Delete Matrices (Del)");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_PROPERTIES", "",
		MTexture::getTexture("gui\\icon\\tool\\Properties.png"), { -48, 0 }, { 24, 24 }, CButton::RenderStyle::FILL, []() {
		if (m_editor->getModel()) m_editor->getModel()->editMatrixProperties();
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
