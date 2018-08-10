#include "engine\editor\model\menu\ModelOverlay.h"
#include "engine\editor\Editor.h"

Model* ModelOverlay::m_model = 0;
Container* ModelOverlay::m_container = 0;
ColorOverlay* ModelOverlay::m_colorOverlay = 0;

Container* ModelOverlay::init(Model* p_model) {
	if(m_container) return 0;
	m_model = p_model;

	m_container = new Container("GUI_MODEL", {0, 0}, {}, true);

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
	m_container->addComponent(new CToolbar("TOOLBAR_MAIN", {0, 28}, {0, 18}), Component::Anchor::TOP_LEFT, Component::Anchor::TOP_RIGHT)->setPriorityLayer(4);
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "File")->
		addButton("File", "New", "", []() { m_model->fileNew(); })->
		addButton("File", "Open", "Ctrl+O", []() { m_model->fileOpen(); })->
		addButton("File", "Save", "Ctrl+S", []() { m_model->fileSave(); })->
		addButton("File", "Exit", "Alt+F4", []() { m_model->fileExit(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Edit")->
		addButton("Edit", "Animation Mode", "", []() { Editor::setEditorMode(Editor::EditorMode::ANIMATION); })->
		addButton("Edit", "Undo", "Ctrl+Z", []() { m_model->editUndo(); })->
		addButton("Edit", "Redo", "Ctrl+Y", []() { m_model->editRedo(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Model")->
		addButton("Model", "New Matrix", "Ctrl+N", []() { ModelOverlay::getContainer()->setPauseScreen("DIALOG_NEWMATRIX"); })->
		addButton("Model", "Properties", "F2", []() { ModelOverlay::getContainer()->setPauseScreen("DIALOG_PROPERTIES"); })->
		addButton("Model", "Flip X", "", []() { m_model->flipMatrix(AXIS_X); })->
		addButton("Model", "Flip Y", "", []() { m_model->flipMatrix(AXIS_Y); })->
		addButton("Model", "Flip Z", "", []() { m_model->flipMatrix(AXIS_Z); })->
		addButton("Model", "Rotate X", "", []() { m_model->rotateMatrix(AXIS_X); })->
		addButton("Model", "Rotate Y", "", []() { m_model->rotateMatrix(AXIS_Y); })->
		addButton("Model", "Rotate Z", "", []() { m_model->rotateMatrix(AXIS_Z); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "View")->
		addButton("View", "Focus Matrix", "Space", []() { m_model->focus(); })->
		addButton("View", "Toggle Grid", "Ctrl+G", []() { m_model->toggleGrid(); })->
		addButton("View", "Toggle Outline", "Ctrl+H", []() { m_model->toggleOutline(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Help")->
		addButton("Help", "About Voxel Model Editor", "", []() { ModelOverlay::getContainer()->setPauseScreen("DIALOG_ABOUT"); });

	CButtonRadio* _toolbar;
	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR_VOXEL", "", {32, 0}, {32, -20},
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::RIGHT), Component::Anchor::NONE, Component::Anchor::BOTTOM_LEFT)
		->setVisible(true);
	_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 50}, {24, 24}, {0, 24});
	_toolbar->setPressFunction([]() {
		m_model->updateTool();
	});
	m_container->findComponent("GUI_TOOLBAR_VOXEL")->addComponent(_toolbar, Component::Anchor::TOP_CENTER);
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\VoxelSingle.png"))->setTooltip("Single - 1");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\VoxelBox.png"))->setTooltip("Box - 2");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\VoxelFill.png"))->setTooltip("Fill - 3");

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR_MATRIX", "", {32, 0}, {32, -20},
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::RIGHT), Component::Anchor::NONE, Component::Anchor::BOTTOM_LEFT)
		->setVisible(false);
	_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 50}, {24, 24}, {0, 24});
	m_container->findComponent("GUI_TOOLBAR_MATRIX")->addComponent(_toolbar, Component::Anchor::TOP_CENTER);
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixSelect.png"))->setTooltip("Select Matrix - 1");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixMove.png"))->setTooltip("Move Matrix - 2");
	_toolbar->addButton(MTexture::getTexture("gui\\icon\\tool\\MatrixResize.png"))->setTooltip("Resize Matrix - 3");

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", {0, 0}, {32, -20}, 
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::RIGHT), Component::Anchor::NONE, Component::Anchor::BOTTOM_LEFT);

	_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 50}, {24, 24}, {0, 24});
	m_container->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, Component::Anchor::TOP_CENTER);
	_toolbar->setPressFunction([]() {
		Tool* ptool = MTool::getTool(m_container->findComponent("GUI_TOOLBAR\\TOOLBAR")->getPrevSelectedItem());
		Tool* ctool = MTool::getTool(m_container->findComponent("GUI_TOOLBAR\\TOOLBAR")->getSelectedItem());
		if(ptool->hasSubmenu()) {
			switch(ptool->getType()) {
			case Tool::ToolType::VOXEL: m_container->findComponent("GUI_TOOLBAR_VOXEL")->setVisible(false); break;
			case Tool::ToolType::MATRIX: m_container->findComponent("GUI_TOOLBAR_MATRIX")->setVisible(false); break;
			}
		}
		if(ctool->hasSubmenu()) {
			switch(ctool->getType()) {
			case Tool::ToolType::VOXEL: m_container->findComponent("GUI_TOOLBAR_VOXEL")->setVisible(true); break;
			case Tool::ToolType::MATRIX: m_container->findComponent("GUI_TOOLBAR_MATRIX")->setVisible(true); break;
			}
		}
		m_model->updateTool();
	});
	for(Tool* tool : MTool::getToolList()) {
		_toolbar->addButton(tool->getIcon())->setTooltip(tool->getDescription());
	}

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", {-256, 0}, {0, -20}, 
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::LEFT), Component::Anchor::TOP_RIGHT, Component::Anchor::BOTTOM_RIGHT);

	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_COLOR", "Color Palette", {0, 71}, {256, 200}, 
		Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::BOTTOM));
	m_colorOverlay = new ColorOverlay({80, 20}, {160, 160});
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
		{0, 0}, {256, 256}, Component::Theme::PRIMARY, (Sint8)Component::BorderFlag::TOP), Component::Anchor::BOTTOM_CENTER);
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_NEW_MATRIX", "", 
		MTexture::getTexture("gui\\icon\\tool\\Add.png"), {0, 0}, {32, 32}, CButton::RenderStyle::FILL, []() {
		m_container->setPauseScreen("DIALOG_NEWMATRIX");
	}), Component::Anchor::NONE)->setTooltip("New Matrix");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_DELETE_MATRIX", "", 
		MTexture::getTexture("gui\\icon\\tool\\Delete.png"), {32, 0}, {32, 32}, CButton::RenderStyle::FILL, []() {
		m_model->deleteSelectedMatrices();
	}), Component::Anchor::NONE)->setTooltip("Delete Matrices");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_PROPERTIES", "", 
		MTexture::getTexture("gui\\icon\\tool\\Properties.png"), {0, 0}, {32, 32}, CButton::RenderStyle::FILL, []() {
		m_container->setPauseScreen("DIALOG_PROPERTIES");
	}), Component::Anchor::TOP_CENTER)->setTooltip("Edit Properties");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_MOVE_UP_MATRIX", "", 
		MTexture::getTexture("gui\\icon\\tool\\ArrowUp.png"), {-32, 0}, {32, 32}, CButton::RenderStyle::FILL, []() {
		m_model->moveMatrix(true);
	}), Component::Anchor::TOP_RIGHT)->setTooltip("Move Up");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CButton("BUTTON_MOVE_DOWN_MATRIX", "", 
		MTexture::getTexture("gui\\icon\\tool\\ArrowDown.png"), {0, 0}, {32, 32}, CButton::RenderStyle::FILL, []() {
		m_model->moveMatrix(false);
	}), Component::Anchor::TOP_RIGHT)->setTooltip("Move Down");
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CList("LIST_MATRICES", "", 
		{0, 32}, {}, 24), Component::Anchor::TOP_LEFT, Component::Anchor::BOTTOM_RIGHT);
	
	m_container->addPauseScreen(new Dialog("DIALOG_NEWMATRIX", "New Matrix", {}, {300, 200}), Component::Anchor::MIDDLE_CENTER)
		->setPressFunction([]() {})
		->setReleaseFunction([]() {
		if(m_container->findComponent("DIALOG_NEWMATRIX\\MATRIXNAME")->getTitle() != "") {
			glm::vec3 pos = glm::vec3(m_container->findComponent("DIALOG_NEWMATRIX\\WIDTH")->getValue() / -2, 
				m_container->findComponent("DIALOG_NEWMATRIX\\HEIGHT")->getValue() / -2, 
				m_container->findComponent("DIALOG_NEWMATRIX\\DEPTH")->getValue() / -2) + Camera::getFocus();
			m_model->addMatrix(m_container->findComponent("DIALOG_NEWMATRIX\\MATRIXNAME")->getTitle(), pos, 
				glm::ivec3(m_container->findComponent("DIALOG_NEWMATRIX\\WIDTH")->getValue(), 
					m_container->findComponent("DIALOG_NEWMATRIX\\HEIGHT")->getValue(), 
					m_container->findComponent("DIALOG_NEWMATRIX\\DEPTH")->getValue()));
		}
	});
	m_container->findComponent("DIALOG_NEWMATRIX")->addComponent(new TextField("MATRIXNAME", "Matrix Name", {0, 20}, {200, 1}, 0), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([]() { m_container->findComponent("DIALOG_NEWMATRIX\\WIDTH")->setState(1); });
	m_container->findComponent("DIALOG_NEWMATRIX")->addComponent(new NumberField("WIDTH", "Width: ", {50, 60}, {100, 1}, {1, 999}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([]() { m_container->findComponent("DIALOG_NEWMATRIX\\HEIGHT")->setState(1); });
	m_container->findComponent("DIALOG_NEWMATRIX")->addComponent(new NumberField("HEIGHT", "Height: ", {50, 84}, {100, 1}, {1, 999}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([]() { m_container->findComponent("DIALOG_NEWMATRIX\\DEPTH")->setState(1); });
	m_container->findComponent("DIALOG_NEWMATRIX")->addComponent(new NumberField("DEPTH", "Depth: ", {50, 108}, {100, 1}, {1, 999}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER);
	
	m_container->addPauseScreen(new Dialog("DIALOG_PROPERTIES", "Matrix Properties", {}, {300, 200}), Component::Anchor::MIDDLE_CENTER)
		->setPressFunction([&]() {
		Matrix* m = m_model->getSelectedMatrix();
		if(!m) {
			m_container->findComponent("DIALOG_PROPERTIES")->setVisible(false);
			return;
		}
		m_container->findComponent("DIALOG_PROPERTIES\\WINDOW")->setTitle("Matrix Properties: " + m->getName());
		m_container->findComponent("DIALOG_PROPERTIES\\MATRIXNAME")->setTitle(m->getName());
		m_container->findComponent("DIALOG_PROPERTIES\\PARENTNAME")->setTitle(m->getParent());
		m_container->findComponent("DIALOG_PROPERTIES\\OFFX")->setValue(m->getPos().x);
		m_container->findComponent("DIALOG_PROPERTIES\\OFFY")->setValue(m->getPos().y);
		m_container->findComponent("DIALOG_PROPERTIES\\OFFZ")->setValue(m->getPos().z);
		m_container->findComponent("DIALOG_PROPERTIES\\WIDTH")->setValue(m->getSize().x);
		m_container->findComponent("DIALOG_PROPERTIES\\HEIGHT")->setValue(m->getSize().y);
		m_container->findComponent("DIALOG_PROPERTIES\\DEPTH")->setValue(m->getSize().z);
	})->setReleaseFunction([]() {
		Matrix* m = m_model->getSelectedMatrix();
		if(!m) {
			m_container->findComponent("DIALOG_PROPERTIES")->setVisible(false);
			return;
		}
		if(m_container->findComponent("DIALOG_PROPERTIES\\MATRIXNAME")->getTitle() != "")
			m_model->renameMatrix(m->getId(), m_container->findComponent("DIALOG_PROPERTIES\\MATRIXNAME")->getTitle());
		m->setParent(m_container->findComponent("DIALOG_PROPERTIES\\PARENTNAME")->getTitle());
		m->setPosition(glm::vec3(m_container->findComponent("DIALOG_PROPERTIES\\OFFX")->getValue(),
			m_container->findComponent("DIALOG_PROPERTIES\\OFFY")->getValue(),
			m_container->findComponent("DIALOG_PROPERTIES\\OFFZ")->getValue()));
		m->setSize(glm::ivec3(m_container->findComponent("DIALOG_PROPERTIES\\WIDTH")->getValue(),
			m_container->findComponent("DIALOG_PROPERTIES\\HEIGHT")->getValue(),
			m_container->findComponent("DIALOG_PROPERTIES\\DEPTH")->getValue()));
	});
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new TextField("MATRIXNAME", "Matrix Name", {0, 8}, {200, 1}, 0), Component::Anchor::TOP_CENTER);
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new TextField("PARENTNAME", "Parent Matrix", {0, 32}, {200, 1}, 0), Component::Anchor::TOP_CENTER);
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new CText("TEXT_OFFSET", "Position", {-58, 66}, {0, 0}, Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new NumberField("OFFX", "x: ", {-58, 80}, {30, 1}, {-64, 64}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([]() { m_container->findComponent("DIALOG_PROPERTIES\\OFFY")->setState(1); });
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new NumberField("OFFY", "y: ", {-58, 104}, {30, 1}, {-64, 64}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([]() { m_container->findComponent("DIALOG_PROPERTIES\\OFFZ")->setState(1); });
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new NumberField("OFFZ", "z: ", {-58, 128}, {30, 1}, {-64, 64}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([]() { m_container->findComponent("DIALOG_PROPERTIES\\WIDTH")->setState(1); });
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new CText("TEXT_SIZE", "Size", {58, 66}, {0, 0}, Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new NumberField("WIDTH", "x: ", {58, 80}, {30, 1}, {1, 64}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([]() { m_container->findComponent("DIALOG_PROPERTIES\\HEIGHT")->setState(1); });
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new NumberField("HEIGHT", "y: ", {58, 104}, {30, 1}, {1, 64}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER)
		->setReleaseFunction([]() { m_container->findComponent("DIALOG_PROPERTIES\\DEPTH")->setState(1); });
	m_container->findComponent("DIALOG_PROPERTIES")->addComponent(new NumberField("DEPTH", "z: ", {58, 128}, {30, 1}, {1, 64}, NumberField::NumType::INT), Component::Anchor::TOP_CENTER);

	m_container->addPauseScreen(new Dialog("DIALOG_ABOUT", "About Voxel Model Editor", {}, {300, 200}), Component::Anchor::MIDDLE_CENTER)
		->setPressFunction([]() {})
		->setReleaseFunction([]() {});
	// TODO: Read this from a file instead, or find an easier way to update version
	m_container->findComponent("DIALOG_ABOUT")->addComponent(new CText("TEXT_1", "Voxel Model Editor", {0, 26}, {0, 0}, Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);
	m_container->findComponent("DIALOG_ABOUT")->addComponent(new CText("TEXT_2", "Version 1.1", {0, 46}, {0, 0}, Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);
	m_container->findComponent("DIALOG_ABOUT")->addComponent(new CText("TEXT_3", "@ Nick's Voxels 2017 - 2018", {0, 66}, {0, 0}, Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);
	m_container->findComponent("DIALOG_ABOUT")->addComponent(new CText("TEXT_4", "All Rights Reserved", {0, 86}, {0, 0}, Alignment::ALIGN_CENTER, Color(1, 1, 1)), Component::Anchor::TOP_CENTER);

	return m_container;
}
