#include "engine\editor\menu\Overlay.h"

Model* Overlay::m_model = 0;
Container* Overlay::m_container = 0;

Sint32 *Overlay::r = new Sint32(), *Overlay::g = new Sint32(255), *Overlay::b = new Sint32();
Sint32 *Overlay::hue = new Sint32(), *Overlay::sat = new Sint32(), *Overlay::val = new Sint32();
Sint32 *Overlay::w = new Sint32(), *Overlay::h = new Sint32(), *Overlay::d = new Sint32();
Sint32 *Overlay::x = new Sint32(), *Overlay::y = new Sint32(), *Overlay::z = new Sint32();
Uint16 *Overlay::toolbar = new Uint16(), *Overlay::toolbarMeta = new Uint16();


void Overlay::init(Model* p_model)
{
	if(m_container) clear();
	m_model = p_model;

	CButtonRadio* _toolbar;
	m_container = new Container("GUI_OVERLAY", {0, 0}, GScreen::m_screenSize, true);

	Container* titleBar = new Container("TITLE", {0, 0}, {GScreen::m_screenSize.x, 28}, true);
	titleBar->addComponent(new Panel("DRAGBAR", "", {0, 0}, {GScreen::m_screenSize.x, 28}, Component::Theme::PRIMARY, 0))->setPressFunction([]() { GScreen::startWindowDrag(); })->setReleaseFunction([]() { GScreen::endWindowDrag(); }); // NOT DROGBAR
	titleBar->addComponent(new CIcon("", LTexture::getTexture("gui\\icon\\window\\Logo.png"), {2, 2}, {24, 24}));
	titleBar->addComponent(new CText("", "Voxel Model Editor 0.1.0", {30, 14}, {0, 0}, Alignment::ALIGN_LEFT, Color(1, 1, 1)));
	titleBar->addComponent(new CButton("BUTTON_MINIMIZE_WINDOW", "", LTexture::getTexture("gui\\icon\\window\\Minimize.png"), {64, 0}, {32, 22}, false, []() { GScreen::m_windowCommand = GScreen::WindowCommand::MINIMIZE; }), PANEL_ALIGN_TOP_RIGHT)->setTheme(Component::Theme::PRIMARY);
	titleBar->addComponent(new CButton("BUTTON_Resize_WINDOW", "", LTexture::getTexture("gui\\icon\\window\\Resize.png"), {32, 0}, {32, 22}, false, []() { GScreen::m_windowCommand = GScreen::WindowCommand::RESIZE; }), PANEL_ALIGN_TOP_RIGHT)->setTheme(Component::Theme::PRIMARY);
	titleBar->addComponent(new CButton("BUTTON_CLOSE_WINDOW", "", LTexture::getTexture("gui\\icon\\window\\Close.png"), {0, 0}, {32, 22}, false, []() { GScreen::m_windowCommand = GScreen::WindowCommand::CLOSE; }), PANEL_ALIGN_TOP_RIGHT)->setTheme(Component::Theme::PRIMARY);
	m_container->addComponent(titleBar)->setPriorityLayer(4);
	m_container->addComponent(new CToolbar("TOOLBAR_MAIN", {0, 28}, {GScreen::m_screenSize.x, 18}), PANEL_ALIGN_TOP)->setPriorityLayer(4);
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "File")->addButton("File", "New", "", []() { m_model->newModel(); })->addButton("File", "Open", "Ctrl+O", []() { m_model->open(); })->addButton("File", "Save", "Ctrl+S", []() { m_model->save(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Edit")->addButton("Edit", "Undo", "Ctrl+Z", []() { m_model->undo(); })->addButton("Edit", "Redo", "Ctrl+Y", []() { m_model->redo(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Matrix")->addButton("Matrix", "New Matrix", "Ctrl+N", []() { m_container->setPauseScreen("NEW"); })->addButton("Matrix", "Resize", "Ctrl+T", []() { m_container->setPauseScreen("RESIZE\\MATRICES")->setList(m_model->getMatrixNames()); })->addButton("Matrix", "Rename", "", []() { m_container->setPauseScreen("RENAME\\MATRICES")->setList(m_model->getMatrixNames()); })->addButton("Matrix", "Flip X", "", []() { m_model->flipMatrix(FLIP_X); })->addButton("Matrix", "Flip Y", "", []() { m_model->flipMatrix(FLIP_Y); })->addButton("Matrix", "Flip Z", "", []() { m_model->flipMatrix(FLIP_Z); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Animation")->addButton("Animation", "Set Parent", "", []() { m_container->setPauseScreen("PARENT"); })->addButton("Animation", "Add Keyframe", "", []() { m_container->setPauseScreen("KEYFRAME"); })->addButton("Animation", "Play/Pause", "Space", []() {});
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "View")->addButton("View", "Focus Matrix", "Space", []() { m_model->focus(); })->addButton("View", "Toggle Grid", "Ctrl+G", []() { m_model->toggleGrid(); })->addButton("View", "Toggle Outline", "Ctrl+H", []() { m_model->toggleOutline(); });
	m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Tools")->addButton("Tools", "Add Voxel", "B", []() { *toolbar = 0; })->addButton("Tools", "Remove Voxel", "E", []() { *toolbar = 1; })->addButton("Tools", "Replace Voxel", "R", []() { *toolbar = 2; })->addButton("Tools", "Eyedropper", "K", []() { *toolbar = 3; })->addButton("Tools", "Select Matrix", "S", []() { *toolbar = 4; })->addButton("Tools", "Move Matrix", "M", []() { *toolbar = 5; })->addButton("Tools", "Resize Matrix", "T", []() { *toolbar = 6; });

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR_0", "", {36, 50}, {32, 80}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL, false));
	_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 4}, {24, 24}, {0, 24}, toolbarMeta);
	m_container->findComponent("GUI_TOOLBAR_0")->addComponent(_toolbar, PANEL_ALIGN_TOP);
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelAdd.png"))->setTooltip("Single - 1");		//0
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelBox.png"))->setTooltip("Box - 2");			//1
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelFill.png"))->setTooltip("Fill - 3");		//2

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR_1", "", {36, 50}, {32, 80}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL, false));
	_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 4}, {24, 24}, {0, 24}, toolbarMeta);
	m_container->findComponent("GUI_TOOLBAR_1")->addComponent(_toolbar, PANEL_ALIGN_TOP);
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelDelete.png"))->setTooltip("Single - 1");	//0
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelBox.png"))->setTooltip("Box - 2");			//1
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelFill.png"))->setTooltip("Fill - 3");		//2

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR_2", "", {36, 72}, {32, 80}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL, false));
	_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 4}, {24, 24}, {0, 24}, toolbarMeta);
	m_container->findComponent("GUI_TOOLBAR_2")->addComponent(_toolbar, PANEL_ALIGN_TOP);
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelReplace.png"))->setTooltip("Single - 1");	//0
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelBox.png"))->setTooltip("Box - 2");			//1
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelFill.png"))->setTooltip("Fill - 3");		//2

	m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", {0, 0}, {32, GScreen::m_screenSize.y}, Component::Theme::PRIMARY, Component::Border::BORDER_RIGHT));

	m_model->setToolVar(toolbar, toolbarMeta);
	_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 50}, {24, 24}, {0, 24}, toolbar);
	m_container->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, PANEL_ALIGN_TOP);
	_toolbar->setPressFunction([]()
	{
		switch(m_container->findComponent("GUI_TOOLBAR\\TOOLBAR")->getPrevSelectedItem())
		{
		case 0: m_container->findComponent("GUI_TOOLBAR_0")->setVisible(false); break;
		case 1: m_container->findComponent("GUI_TOOLBAR_1")->setVisible(false); break;
		case 2: m_container->findComponent("GUI_TOOLBAR_2")->setVisible(false); break;
		}

		switch(m_container->findComponent("GUI_TOOLBAR\\TOOLBAR")->getSelectedItem())
		{
		case 0: m_container->findComponent("GUI_TOOLBAR_0")->setVisible(true); break;
		case 1: m_container->findComponent("GUI_TOOLBAR_1")->setVisible(true); break;
		case 2: m_container->findComponent("GUI_TOOLBAR_2")->setVisible(true); break;
		}
	});
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelAdd.png"))->setTooltip("Add Voxel - B");			//0
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelDelete.png"))->setTooltip("Erase Voxel - E");		//1
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelReplace.png"))->setTooltip("Replace Voxel - R");	//2
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\VoxelEyedrop.png"))->setTooltip("Eyedropper - K");	//3
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\MatrixSelect.png"))->setTooltip("Select Matrix - S");	//4
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\MatrixMove.png"))->setTooltip("Move Matrix - M");		//5
	_toolbar->addButton(LTexture::getTexture("gui\\icon\\tool\\MatrixResize.png"))->setTooltip("Resize Matrix - T");	//6
	//_toolbar->addButton(new CButtonToggle("ANIMATEMATRIX", LTexture::getTexture("gui\\icon\\tool\\icon\\tool\\MatrixAnimate.png"), {0, 216}, {24, 24}));//7



	m_model->setColor(*r, *g, *b);

	m_container->addComponent(new ContainerPanel("GUI_DETAILS", "", {0, 0}, {256, GScreen::m_screenSize.y}, Component::Theme::PRIMARY, Component::Border::BORDER_LEFT), PANEL_ALIGN_RIGHT);

	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_COLOR", "", {2, 72}, {252, 200}, Component::Theme::PRIMARY, Component::Border::BORDER_BOTTOM, true));
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new ColorOverlay({10, 2}, {160, 160}, r, g, b, hue, sat, val));
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("R", "R: ", Vector2<Sint32>(208, 6), Vector2<Sint32>(32, 1), Vector2<Sint32>(0, 255), r));
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("G", "G: ", Vector2<Sint32>(208, 32), Vector2<Sint32>(32, 1), Vector2<Sint32>(0, 255), g));
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("B", "B: ", Vector2<Sint32>(208, 58), Vector2<Sint32>(32, 1), Vector2<Sint32>(0, 255), b));
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("H", "H: ", Vector2<Sint32>(208, 84), Vector2<Sint32>(32, 1), Vector2<Sint32>(0, 360), hue));
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("V", "V: ", Vector2<Sint32>(208, 110), Vector2<Sint32>(32, 1), Vector2<Sint32>(0, 100), val));
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new NumberField("S", "S: ", Vector2<Sint32>(208, 136), Vector2<Sint32>(32, 1), Vector2<Sint32>(0, 100), sat));
	m_container->findComponent("GUI_DETAILS\\GUI_COLOR")->addComponent(new CSliderV("SS", "", Vector2<Sint32>(177, 6), 152, 100, *sat));



	m_container->findComponent("GUI_DETAILS")->addComponent(new ContainerPanel("GUI_MATRICES", "", {0, 10}, {256, 256}, Component::Theme::PRIMARY, Component::Border::BORDER_TOP, true), PANEL_ALIGN_BOTTOM);
	m_container->findComponent("GUI_DETAILS\\GUI_MATRICES")->addComponent(new CList("LIST_MATRICES", "", {0, 2}, {256, 192}, 18, m_model->getMatrixStates(), m_model->getMatrixList()), PANEL_ALIGN_BOTTOM);



	m_container->addPauseScreen(new Container("RESIZE", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
	m_container->findComponent("RESIZE")->addComponent(new SColorPanel({0, 0}, GScreen::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
	m_container->findComponent("RESIZE")->addComponent(new Panel("BACKGROUND", "Resize Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

	m_container->findComponent("RESIZE")->addComponent(new CDropDown("MATRICES", "Matrices", {0, -180}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_container->findComponent("RESIZE\\MATRICES")->setList(m_model->getMatrixNames());
		if(m_model->getMatrixNames().size() > 0)
		{
			Vector3<Sint32> s = m_model->getMatrix(m_container->findComponent("RESIZE\\MATRICES")->getSelectedItem())->getSize();
			*w = s.x;
			*h = s.y;
			*d = s.z;
			*x = 0;
			*y = 0;
			*z = 0;
		}
	})->setPriorityLayer(4);

	m_container->findComponent("RESIZE")->addComponent(new NumberField("OFFX", "Offset(x): ", {0, -152}, {100, 1}, {0, 64}, x), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{ m_container->findComponent("RESIZE\\OFFY")->setState(1); });
	m_container->findComponent("RESIZE")->addComponent(new NumberField("OFFY", "Offset(y): ", {0, -128}, {100, 1}, {0, 64}, y), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{ m_container->findComponent("RESIZE\\OFFZ")->setState(1); });
	m_container->findComponent("RESIZE")->addComponent(new NumberField("OFFZ", "Offset(z): ", {0, -104}, {100, 1}, {0, 64}, z), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{ m_container->findComponent("RESIZE\\WIDTH")->setState(1); });

	m_container->findComponent("RESIZE")->addComponent(new NumberField("WIDTH", "Width(x): ", {0, -80}, {100, 1}, {1, 64}, w), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{ m_container->findComponent("RESIZE\\HEIGHT")->setState(1); });
	m_container->findComponent("RESIZE")->addComponent(new NumberField("HEIGHT", "Height(y): ", {0, -56}, {100, 1}, {1, 64}, h), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{ m_container->findComponent("RESIZE\\DEPTH")->setState(1); });
	m_container->findComponent("RESIZE")->addComponent(new NumberField("DEPTH", "Depth(z): ", {0, -32}, {100, 1}, {1, 64}, d), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_model->resize(m_container->findComponent("RESIZE\\MATRICES")->getSelectedItem(), {*x, *y, *z}, {*w, *h, *d});
		m_container->setPauseScreen("");
	});

	m_container->findComponent("RESIZE")->addComponent(new CButton("OK", "Resize", {-40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_model->resize(m_container->findComponent("RESIZE\\MATRICES")->getSelectedItem(), {*x, *y, *z}, {*w, *h, *d});
		m_container->setPauseScreen("");
	});
	m_container->findComponent("RESIZE")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]() {	m_container->setPauseScreen(""); });



	m_container->addPauseScreen(new Container("NEW", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
	m_container->findComponent("NEW")->addComponent(new SColorPanel({0, 0}, GScreen::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
	m_container->findComponent("NEW")->addComponent(new Panel("BACKGROUND", "New Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

	m_container->findComponent("NEW")->addComponent(new TextField("MATRIXNAME", "Matrix Name", {0, -172}, {200, 1}, 0), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_container->findComponent("NEW\\WIDTH")->setState(1);
	});

	m_container->findComponent("NEW")->addComponent(new NumberField("WIDTH", "Width(x): ", {0, -80}, {100, 1}, {1, 999}, w), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_container->findComponent("NEW\\HEIGHT")->setState(1);
	});
	m_container->findComponent("NEW")->addComponent(new NumberField("HEIGHT", "Height(y): ", {0, -56}, {100, 1}, {1, 999}, h), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_container->findComponent("NEW\\DEPTH")->setState(1);
	});
	m_container->findComponent("NEW")->addComponent(new NumberField("DEPTH", "Depth(z): ", {0, -32}, {100, 1}, {1, 999}, d), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		if(m_container->findComponent("NEW\\MATRIXNAME")->getTitle() != "")
		{
			Vector3<Sint32> pos = Vector3<Sint32>(*w / -2, 0, *d / -2);
			m_model->addMatrix(m_container->findComponent("NEW\\MATRIXNAME")->getTitle(), pos, Vector3<Sint32>(*w, *h, *d));
			m_container->setPauseScreen("");
		}
	});

	m_container->findComponent("NEW")->addComponent(new CButton("OK", "Create", {-40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		if(m_container->findComponent("NEW\\MATRIXNAME")->getTitle() != "")
		{
			Vector3<Sint32> pos = Vector3<Sint32>(*w / -2, 0, *d / -2);
			m_model->addMatrix(m_container->findComponent("NEW\\MATRIXNAME")->getTitle(), pos, Vector3<Sint32>(*w, *h, *d));
			m_container->setPauseScreen("");
		}
	});
	m_container->findComponent("NEW")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]() { m_container->setPauseScreen(""); });



	m_container->addPauseScreen(new Container("RENAME", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
	m_container->findComponent("RENAME")->addComponent(new SColorPanel({0, 0}, GScreen::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
	m_container->findComponent("RENAME")->addComponent(new Panel("BACKGROUND", "Rename Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

	m_container->findComponent("RENAME")->addComponent(new CDropDown("MATRICES", "Matrices", {0, -176}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_container->findComponent("RENAME\\MATRICES")->setList(m_model->getMatrixNames());
	})->setPriorityLayer(4);

	m_container->findComponent("RENAME")->addComponent(new TextField("MATRIXNAME", "Matrix Name", {0, -120}, {200, 1}, 0), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		if(m_container->findComponent("RENAME\\MATRIXNAME")->getTitle() != "")
		{
			m_model->renameMatrix(m_container->findComponent("RENAME\\MATRICES")->getSelectedItem(), m_container->findComponent("RENAME\\MATRIXNAME")->getTitle());
			m_container->setPauseScreen("");
		}
	});

	m_container->findComponent("RENAME")->addComponent(new CButton("OK", "Rename", {-40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		if(m_container->findComponent("RENAME\\MATRIXNAME")->getTitle() != "")
		{
			m_model->renameMatrix(m_container->findComponent("RENAME\\MATRICES")->getSelectedItem(), m_container->findComponent("RENAME\\MATRIXNAME")->getTitle());
			m_container->setPauseScreen("");
		}
	});
	m_container->findComponent("RENAME")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]() { m_container->setPauseScreen(""); });



	m_container->addPauseScreen(new Container("PARENT", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
	m_container->findComponent("PARENT")->addComponent(new SColorPanel({0, 0}, GScreen::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
	m_container->findComponent("PARENT")->addComponent(new Panel("BACKGROUND", "Set Parent Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

	m_container->findComponent("PARENT")->addComponent(new CDropDown("MATRICES", "Matrix", {0, -176}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		Matrix* _m = m_model->getMatrix(m_container->findComponent("PARENT\\MATRICES")->getSelectedItem());
		std::vector<std::string> _list = m_model->getMatrixNames();
		m_container->findComponent("PARENT\\MATRICES")->setList(_list);
		_list.insert(_list.begin(), "None");
		m_container->findComponent("PARENT\\PARENTMATRIX")->setList(_list);
		if(_m->getParent() != 0)
			m_container->findComponent("PARENT\\PARENTMATRIX")->setSelectedItem(_m->getParent()->getId() + 1);
		else
			m_container->findComponent("PARENT\\PARENTMATRIX")->setSelectedItem(0);
	})->setPriorityLayer(4);

	m_container->findComponent("PARENT")->addComponent(new CDropDown("PARENTMATRIX", "Parent", {0, -126}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		if(!m_container->findComponent("PARENT\\PARENTMATRIX")->hasList())
		{
			Matrix* _m = m_model->getMatrix(m_container->findComponent("PARENT\\MATRICES")->getSelectedItem());
			std::vector<std::string> _list = m_model->getMatrixNames();
			m_container->findComponent("PARENT\\MATRICES")->setList(_list);
			_list.insert(_list.begin(), "None");
			m_container->findComponent("PARENT\\PARENTMATRIX")->setList(_list);
			if(_m->getParent() != 0)
				m_container->findComponent("PARENT\\PARENTMATRIX")->setSelectedItem(_m->getParent()->getId() + 1);
			else
				m_container->findComponent("PARENT\\PARENTMATRIX")->setSelectedItem(0);
		}
	})->setPriorityLayer(3);

	m_container->findComponent("PARENT")->addComponent(new CButton("OK", "Set", {-40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_container->setPauseScreen("");
		m_model->getMatrix(m_container->findComponent("PARENT\\MATRICES")->getSelectedItem())->setParent(m_model->getMatrix(m_container->findComponent("PARENT\\PARENTMATRIX")->getSelectedItem() - 1));
	});
	m_container->findComponent("PARENT")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]() { m_container->setPauseScreen(""); });



	m_container->addPauseScreen(new Container("KEYFRAME", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
	m_container->findComponent("KEYFRAME")->addComponent(new SColorPanel({0, 0}, GScreen::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
	m_container->findComponent("KEYFRAME")->addComponent(new Panel("BACKGROUND", "Rename Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

	m_container->findComponent("KEYFRAME")->addComponent(new CDropDown("MATRICES", "Matrices", {0, -176}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		m_container->findComponent("KEYFRAME\\MATRICES")->setList(m_model->getMatrixNames());
	})->setPriorityLayer(4);

	m_container->findComponent("KEYFRAME")->addComponent(new TextField("MATRIXNAME", "Matrix Name", {0, -120}, {200, 1}, 0), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		if(m_container->findComponent("KEYFRAME\\MATRIXNAME")->getTitle() != "")
		{
			m_model->renameMatrix(m_container->findComponent("KEYFRAME\\MATRICES")->getSelectedItem(), m_container->findComponent("KEYFRAME\\MATRIXNAME")->getTitle());
			m_container->setPauseScreen("");
		}
	});

	m_container->findComponent("KEYFRAME")->addComponent(new CButton("OK", "Rename", {-40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]()
	{
		if(m_container->findComponent("KEYFRAME\\MATRIXNAME")->getTitle() != "")
		{
			m_model->renameMatrix(m_container->findComponent("KEYFRAME\\MATRICES")->getSelectedItem(), m_container->findComponent("KEYFRAME\\MATRIXNAME")->getTitle());
			m_container->setPauseScreen("");
		}
	});
	m_container->findComponent("KEYFRAME")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, true), PANEL_ALIGN_CENTER)->setPressFunction([]() { m_container->setPauseScreen(""); });




	m_container->addComponent(new ContainerPanel("PANEL_INFOBAR", "", {0, 0}, {GScreen::m_screenSize.x, 20}, Component::Theme::INFO, Component::Border::BORDER_TOP), PANEL_ALIGN_BOTTOM)->setPriorityLayer(-4);

	std::string* _matrixData = new std::string("");
	m_model->setDataString(_matrixData);
	m_container->findComponent("PANEL_INFOBAR")->addComponent(new DataField("DATA_MATRIX", _matrixData, {40, 0}, {200, 20}, Component::Theme::INFO), PANEL_ALIGN_LEFT);
}
