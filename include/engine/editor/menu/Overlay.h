#pragma once

#include "engine\gfx\LGui.h"
#include "engine\utils\Globals.h"
#include "engine\editor\model\Model.h"
#include "AnimationOverlay.h"
#include "ColorOverlay.h"

class Overlay
{
private:
	static Model* m_model;
	static Container* m_container;

	static Sint32 *r, *g, *b;
	static Sint32 *hue, *sat, *val;
	static Sint32 *w, *h, *d;
	static Sint32 *x, *y, *z;

	static Uint16 *toolbar, *toolbarMeta;
public:
	static void init(Model* p_model)
	{
		if(m_container) clear();
		m_model = p_model;
		
		CButtonRadio* _toolbar;
		m_container = new Container("GUI_OVERLAY", {0, 0}, Globals::m_screenSize, true);

		//m_container->addComponent(new Panel("", "", {0, 0}, {Globals::m_screenSize.x, 24}, Component::Theme::TOOLBAR, Component::Border::BORDER_NONE), PANEL_ALIGN_TOP)->setPriorityLayer(4);
		//m_container->addComponent(new CText("", "Voxel Model Editor", {4, 12}, {100, 1}, ALIGN_LEFT, Color(0xF0F0F0)))->setPriorityLayer(5);
		//m_container->addComponent(new CButton("", "", LTexture::getTexture("gui\\Close.png"), {4, 4}, {20, 20}, []() { Globals::m_windowCommand = Globals::CLOSE; }), PANEL_ALIGN_TOP_RIGHT)->setPriorityLayer(5);
		//m_container->addComponent(new CButton("", "", LTexture::getTexture("gui\\Minimize.png"), {28, 4}, {20, 20}, []() { Globals::m_windowCommand = Globals::MINIMIZE; }), PANEL_ALIGN_TOP_RIGHT)->setPriorityLayer(5);

		m_container->addComponent(new CToolbar("TOOLBAR_MAIN", {0, 0}, {Globals::m_screenSize.x, 18}), PANEL_ALIGN_TOP)->setPriorityLayer(4);
		m_container->findComponent("TOOLBAR_MAIN")->addButton("", "File")->addButton("File", "New", "", []() { m_model->newModel(); })->addButton("File", "Open", "Ctrl+O", []() { m_model->open(); })->addButton("File", "Save", "Ctrl+S", []() { m_model->save(); });
		m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Edit")->addButton("Edit", "Undo", "Ctrl+Z", []() { m_model->undo(); })->addButton("Edit", "Redo", "Ctrl+Y", []() { m_model->redo(); });
		m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Matrix")->addButton("Matrix", "New Matrix", "Ctrl+N", []() { m_container->setPauseScreen("NEW"); })->addButton("Matrix", "Resize", "Ctrl+T", []() { m_container->setPauseScreen("RESIZE")->findComponent("MATRICES")->setList(m_model->getMatrixNames()); })->addButton("Matrix", "Rename", "", []() { m_container->setPauseScreen("RENAME")->findComponent("MATRICES")->setList(m_model->getMatrixNames()); })->addButton("Matrix", "Flip X", "", []() { m_model->flipMatrix(FLIP_X); })->addButton("Matrix", "Flip Y", "", []() { m_model->flipMatrix(FLIP_Y); })->addButton("Matrix", "Flip Z", "", []() { m_model->flipMatrix(FLIP_Z); });
		m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Animation")->addButton("Animation", "Set Parent", "", []() { m_container->setPauseScreen("PARENT"); })->addButton("Animation", "Add Keyframe", "", []() { m_container->setPauseScreen("KEYFRAME"); })->addButton("Animation", "Play/Pause", "Space", []() { m_container->setPauseScreen("KEYFRAME"); });
		m_container->findComponent("TOOLBAR_MAIN")->addButton("", "View")->addButton("View", "Focus Matrix", "Space", []() { m_model->focus(); })->addButton("View", "Toggle Grid", "Ctrl+G", []() { m_model->toggleGrid(); })->addButton("View", "Toggle Outline", "Ctrl+H", []() { m_model->toggleOutline(); });
		m_container->findComponent("TOOLBAR_MAIN")->addButton("", "Tools")->addButton("Tools", "Add Voxel", "B", []() { *toolbar = 0; })->addButton("Tools", "Remove Voxel", "E", []() { *toolbar = 1; })->addButton("Tools", "Replace Voxel", "R", []() { *toolbar = 2; })->addButton("Tools", "Eyedropper", "K", []() { *toolbar = 3; })->addButton("Tools", "Select Matrix", "S", []() { *toolbar = 4; })->addButton("Tools", "Move Matrix", "M", []() { *toolbar = 5; })->addButton("Tools", "Resize Matrix", "T", []() { *toolbar = 6; });

		m_container->addComponent(new ContainerPanel("GUI_TOOLBAR_0", "", {36, 22}, {32, 88}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL, false));
		_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 0}, {0, 0}, toolbarMeta);
		m_container->findComponent("GUI_TOOLBAR_0")->addComponent(_toolbar, PANEL_ALIGN_TOP);
		_toolbar->addButton(new CButtonToggle("ADDVOXEL", LTexture::getTexture("gui\\VoxelAdd.png"), {0, 4}, {24, 24}))->setTooltip("Single - 1");			//0
		_toolbar->addButton(new CButtonToggle("BOXVOXEL", LTexture::getTexture("gui\\VoxelBox.png"), {0, 32}, {24, 24}))->setTooltip("Box - 2");			//1
		_toolbar->addButton(new CButtonToggle("FILLVOXEL", LTexture::getTexture("gui\\VoxelFill.png"), {0, 60}, {24, 24}))->setTooltip("Fill - 3");			//2

		m_container->addComponent(new ContainerPanel("GUI_TOOLBAR_1", "", {36, 22}, {32, 88}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL, false));
		_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 0}, {0, 0}, toolbarMeta);
		m_container->findComponent("GUI_TOOLBAR_1")->addComponent(_toolbar, PANEL_ALIGN_TOP);
		_toolbar->addButton(new CButtonToggle("DELVOXEL", LTexture::getTexture("gui\\VoxelDelete.png"), {0, 4}, {24, 24}))->setTooltip("Single - 1");			//0
		_toolbar->addButton(new CButtonToggle("BOXVOXEL", LTexture::getTexture("gui\\VoxelBox.png"), {0, 32}, {24, 24}))->setTooltip("Box - 2");			//1
		_toolbar->addButton(new CButtonToggle("FILLVOXEL", LTexture::getTexture("gui\\VoxelFill.png"), {0, 60}, {24, 24}))->setTooltip("Fill - 3");			//2

		m_container->addComponent(new ContainerPanel("GUI_TOOLBAR_2", "", {36, 56}, {32, 88}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL, false));
		_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 0}, {0, 0}, toolbarMeta);
		m_container->findComponent("GUI_TOOLBAR_2")->addComponent(_toolbar, PANEL_ALIGN_TOP);
		_toolbar->addButton(new CButtonToggle("REPLACEVOXEL", LTexture::getTexture("gui\\VoxelReplace.png"), {0, 4}, {24, 24}))->setTooltip("Single - 1");	//0
		_toolbar->addButton(new CButtonToggle("BOXVOXEL", LTexture::getTexture("gui\\VoxelBox.png"), {0, 32}, {24, 24}))->setTooltip("Box - 2");			//1
		_toolbar->addButton(new CButtonToggle("FILLVOXEL", LTexture::getTexture("gui\\VoxelFill.png"), {0, 60}, {24, 24}))->setTooltip("Fill - 3");			//2

		//m_container->addComponent(new AnimationOverlay({100, 100}, {200, 200}, m_model));

		m_container->addComponent(new ContainerPanel("GUI_TOOLBAR", "", {0, 0}, {32, Globals::m_screenSize.y}, Component::Theme::PRIMARY, Component::Border::BORDER_RIGHT));

		m_model->setToolVar(toolbar, toolbarMeta);
		_toolbar = new CButtonRadio("TOOLBAR", "", {-12, 4}, {0, 0}, toolbar);
		m_container->findComponent("GUI_TOOLBAR")->addComponent(_toolbar, PANEL_ALIGN_TOP);
		_toolbar->setFunction([]()
		{
			switch(m_container->findComponent("GUI_TOOLBAR")->findComponent("TOOLBAR")->getPrevSelectedItem())
			{
			case 0: m_container->findComponent("GUI_TOOLBAR_0")->setVisible(false); break;
			case 1: m_container->findComponent("GUI_TOOLBAR_1")->setVisible(false); break;
			case 2: m_container->findComponent("GUI_TOOLBAR_2")->setVisible(false); break;
			}

			switch(m_container->findComponent("GUI_TOOLBAR")->findComponent("TOOLBAR")->getSelectedItem())
			{
			case 0: m_container->findComponent("GUI_TOOLBAR_0")->setVisible(true); break;
			case 1: m_container->findComponent("GUI_TOOLBAR_1")->setVisible(true); break;
			case 2: m_container->findComponent("GUI_TOOLBAR_2")->setVisible(true); break;
			}
		});
		_toolbar->addButton(new CButtonToggle("ADDVOXEL", LTexture::getTexture("gui\\VoxelAdd.png"), {0, 20}, {24, 24}))->setTooltip("Add Voxel - B");			//0
		_toolbar->addButton(new CButtonToggle("DELVOXEL", LTexture::getTexture("gui\\VoxelDelete.png"), {0, 48}, {24, 24}))->setTooltip("Erase Voxel - E");		//1
		_toolbar->addButton(new CButtonToggle("REPLACEVOXEL", LTexture::getTexture("gui\\VoxelReplace.png"), {0, 76}, {24, 24}))->setTooltip("Replace Voxel - R");	//2
		_toolbar->addButton(new CButtonToggle("EYEDROPVOXEL", LTexture::getTexture("gui\\VoxelEyedrop.png"), {0, 104}, {24, 24}))->setTooltip("Eyedropper - K");	//3
		_toolbar->addButton(new CButtonToggle("SELECTMATRIX", LTexture::getTexture("gui\\MatrixSelect.png"), {0, 132}, {24, 24}))->setTooltip("Select Matrix - S");	//4
		_toolbar->addButton(new CButtonToggle("MOVEMATRIX", LTexture::getTexture("gui\\MatrixMove.png"), {0, 160}, {24, 24}))->setTooltip("Move Matrix - M");		//5
		_toolbar->addButton(new CButtonToggle("RESIZEMATRIX", LTexture::getTexture("gui\\MatrixResize.png"), {0, 188}, {24, 24}))->setTooltip("Resize Matrix - T");	//6
		//_toolbar->addButton(new CButtonToggle("ANIMATEMATRIX", LTexture::getTexture("gui\\MatrixAnimate.png"), {0, 216}, {24, 24}));//7



		m_model->setColor(*r, *g, *b);

		m_container->addComponent(new ContainerPanel("GUI_COLOR", "", {0, 0}, {256, Globals::m_screenSize.y}, Component::Theme::PRIMARY, Component::Border::BORDER_LEFT), PANEL_ALIGN_RIGHT);

		m_container->findComponent("GUI_COLOR")->addComponent(new CSlider("R", "Red", Vector2<Sint32>(0, 84), 200, 255, *r), PANEL_ALIGN_TOP);
		m_container->findComponent("GUI_COLOR")->addComponent(new CSlider("G", "Green", Vector2<Sint32>(0, 120), 200, 255, *g), PANEL_ALIGN_TOP);
		m_container->findComponent("GUI_COLOR")->addComponent(new CSlider("B", "Blue", Vector2<Sint32>(0, 156), 200, 255, *b), PANEL_ALIGN_TOP);
		m_container->findComponent("GUI_COLOR")->addComponent(new CSlider("H", "Hue", Vector2<Sint32>(0, 212), 200, 360, *hue), PANEL_ALIGN_TOP);
		m_container->findComponent("GUI_COLOR")->addComponent(new CSlider("S", "Saturation", Vector2<Sint32>(0, 248), 200, 100, *sat), PANEL_ALIGN_TOP);
		m_container->findComponent("GUI_COLOR")->addComponent(new CSlider("V", "Value", Vector2<Sint32>(0, 284), 200, 100, *val), PANEL_ALIGN_TOP);

		m_container->findComponent("GUI_COLOR")->addComponent(new Panel("", "", {0, 600}, {100, 100}, Component::Theme::INFO, Component::Border::BORDER_ALL), PANEL_ALIGN_TOP);
		m_container->findComponent("GUI_COLOR")->addComponent(new ColorPanel({0, 600}, {100, 100}, *r, *g, *b, LTexture::getTexture("gui\\ColorCube.png")), PANEL_ALIGN_TOP);

		m_container->findComponent("GUI_COLOR")->addComponent(new ColorOverlay({0, 332}, {200, 200}, r, g, b, hue, sat, val), PANEL_ALIGN_TOP);



		m_container->addPauseScreen(new Container("RESIZE", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
		m_container->findComponent("RESIZE")->addComponent(new SColorPanel({0, 0}, Globals::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
		m_container->findComponent("RESIZE")->addComponent(new Panel("BACKGROUND", "Resize Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

		m_container->findComponent("RESIZE")->addComponent(new CDropDown("MATRICES", "Matrices", {0, -180}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("RESIZE")->findComponent("MATRICES")->setList(m_model->getMatrixNames());
			if(m_model->getMatrixNames().size() > 0)
			{
				Vector3<Sint32> s = m_model->getMatrix(m_container->findComponent("RESIZE")->findComponent("MATRICES")->getSelectedItem())->getSize();
				*w = s.x;
				*h = s.y;
				*d = s.z;
				*x = 0;
				*y = 0;
				*z = 0;
			}
		})->setPriorityLayer(4);

		m_container->findComponent("RESIZE")->addComponent(new NumberField("OFFX", "Offset(x): ", {0, -152}, {100, 1}, {0, 64}, x), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("RESIZE")->findComponent("OFFY")->setState(1);
		});
		m_container->findComponent("RESIZE")->addComponent(new NumberField("OFFY", "Offset(y): ", {0, -128}, {100, 1}, {0, 64}, y), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("RESIZE")->findComponent("OFFZ")->setState(1);
		});
		m_container->findComponent("RESIZE")->addComponent(new NumberField("OFFZ", "Offset(z): ", {0, -104}, {100, 1}, {0, 64}, z), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("RESIZE")->findComponent("WIDTH")->setState(1);
		});

		m_container->findComponent("RESIZE")->addComponent(new NumberField("WIDTH", "Width(x): ", {0, -80}, {100, 1}, {1, 64}, w), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("RESIZE")->findComponent("HEIGHT")->setState(1);
		});
		m_container->findComponent("RESIZE")->addComponent(new NumberField("HEIGHT", "Height(y): ", {0, -56}, {100, 1}, {1, 64}, h), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("RESIZE")->findComponent("DEPTH")->setState(1);
		});
		m_container->findComponent("RESIZE")->addComponent(new NumberField("DEPTH", "Depth(z): ", {0, -32}, {100, 1}, {1, 64}, d), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_model->resize(m_container->findComponent("RESIZE")->findComponent("MATRICES")->getSelectedItem(), {*x, *y, *z}, {*w, *h, *d});
			m_container->setPauseScreen("");
		});

		m_container->findComponent("RESIZE")->addComponent(new CButton("OK", "Resize", {-40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_model->resize(m_container->findComponent("RESIZE")->findComponent("MATRICES")->getSelectedItem(), {*x, *y, *z}, {*w, *h, *d});
			m_container->setPauseScreen("");
		});
		m_container->findComponent("RESIZE")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]() {	m_container->setPauseScreen(""); });



		m_container->addPauseScreen(new Container("NEW", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
		m_container->findComponent("NEW")->addComponent(new SColorPanel({0, 0}, Globals::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
		m_container->findComponent("NEW")->addComponent(new Panel("BACKGROUND", "New Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

		m_container->findComponent("NEW")->addComponent(new TextField("MATRIXNAME", "Matrix Name", {0, -172}, {200, 1}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("NEW")->findComponent("WIDTH")->setState(1);
		});

		m_container->findComponent("NEW")->addComponent(new NumberField("WIDTH", "Width(x): ", {0, -80}, {100, 1}, {1, 999}, w), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("NEW")->findComponent("HEIGHT")->setState(1);
		});
		m_container->findComponent("NEW")->addComponent(new NumberField("HEIGHT", "Height(y): ", {0, -56}, {100, 1}, {1, 999}, h), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("NEW")->findComponent("DEPTH")->setState(1);
		});
		m_container->findComponent("NEW")->addComponent(new NumberField("DEPTH", "Depth(z): ", {0, -32}, {100, 1}, {1, 999}, d), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			if(m_container->findComponent("NEW")->findComponent("MATRIXNAME")->getTitle() != "")
			{
				Vector3<Sint32> pos = Vector3<Sint32>(*w / -2, 0, *d / -2);
				m_model->addMatrix(m_container->findComponent("NEW")->findComponent("MATRIXNAME")->getTitle(), pos, Vector3<Sint32>(*w, *h, *d));
				m_container->setPauseScreen("");
			}
		});

		m_container->findComponent("NEW")->addComponent(new CButton("OK", "Create", {-40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			if(m_container->findComponent("NEW")->findComponent("MATRIXNAME")->getTitle() != "")
			{
				Vector3<Sint32> pos = Vector3<Sint32>(*w / -2, 0, *d / -2);
				m_model->addMatrix(m_container->findComponent("NEW")->findComponent("MATRIXNAME")->getTitle(), pos, Vector3<Sint32>(*w, *h, *d));
				m_container->setPauseScreen("");
			}
		});
		m_container->findComponent("NEW")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]() { m_container->setPauseScreen(""); });



		m_container->addPauseScreen(new Container("RENAME", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
		m_container->findComponent("RENAME")->addComponent(new SColorPanel({0, 0}, Globals::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
		m_container->findComponent("RENAME")->addComponent(new Panel("BACKGROUND", "Rename Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

		m_container->findComponent("RENAME")->addComponent(new CDropDown("MATRICES", "Matrices", {0, -176}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("RENAME")->findComponent("MATRICES")->setList(m_model->getMatrixNames());
		})->setPriorityLayer(4);

		m_container->findComponent("RENAME")->addComponent(new TextField("MATRIXNAME", "Matrix Name", {0, -120}, {200, 1}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			if(m_container->findComponent("RENAME")->findComponent("MATRIXNAME")->getTitle() != "")
			{
				m_model->renameMatrix(m_container->findComponent("RENAME")->findComponent("MATRICES")->getSelectedItem(), m_container->findComponent("RENAME")->findComponent("MATRIXNAME")->getTitle());
				m_container->setPauseScreen("");
			}
		});

		m_container->findComponent("RENAME")->addComponent(new CButton("OK", "Rename", {-40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			if(m_container->findComponent("RENAME")->findComponent("MATRIXNAME")->getTitle() != "")
			{
				m_model->renameMatrix(m_container->findComponent("RENAME")->findComponent("MATRICES")->getSelectedItem(), m_container->findComponent("RENAME")->findComponent("MATRIXNAME")->getTitle());
				m_container->setPauseScreen("");
			}
		});
		m_container->findComponent("RENAME")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]() { m_container->setPauseScreen(""); });



		m_container->addPauseScreen(new Container("PARENT", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
		m_container->findComponent("PARENT")->addComponent(new SColorPanel({0, 0}, Globals::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
		m_container->findComponent("PARENT")->addComponent(new Panel("BACKGROUND", "Set Parent Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

		m_container->findComponent("PARENT")->addComponent(new CDropDown("MATRICES", "Matrix", {0, -176}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			Matrix* _m = m_model->getMatrix(m_container->findComponent("PARENT")->findComponent("MATRICES")->getSelectedItem());
			std::vector<std::string> _list = m_model->getMatrixNames();
			m_container->findComponent("PARENT")->findComponent("MATRICES")->setList(_list);
			_list.insert(_list.begin(), "None");
			m_container->findComponent("PARENT")->findComponent("PARENTMATRIX")->setList(_list);
			if(_m->getParent() != 0)
				m_container->findComponent("PARENT")->findComponent("PARENTMATRIX")->setSelectedItem(_m->getParent()->getId() + 1);
			else
				m_container->findComponent("PARENT")->findComponent("PARENTMATRIX")->setSelectedItem(0);
		})->setPriorityLayer(4);

		m_container->findComponent("PARENT")->addComponent(new CDropDown("PARENTMATRIX", "Parent", {0, -126}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			if(!m_container->findComponent("PARENT")->findComponent("PARENTMATRIX")->hasList())
			{
				std::cout << "why" << std::endl;
				Matrix* _m = m_model->getMatrix(m_container->findComponent("PARENT")->findComponent("MATRICES")->getSelectedItem());
				std::vector<std::string> _list = m_model->getMatrixNames();
				m_container->findComponent("PARENT")->findComponent("MATRICES")->setList(_list);
				_list.insert(_list.begin(), "None");
				m_container->findComponent("PARENT")->findComponent("PARENTMATRIX")->setList(_list);
				if(_m->getParent() != 0)
					m_container->findComponent("PARENT")->findComponent("PARENTMATRIX")->setSelectedItem(_m->getParent()->getId() + 1);
				else
					m_container->findComponent("PARENT")->findComponent("PARENTMATRIX")->setSelectedItem(0);
			}
		})->setPriorityLayer(3);

		m_container->findComponent("PARENT")->addComponent(new CButton("OK", "Set", {-40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->setPauseScreen("");
			m_model->getMatrix(m_container->findComponent("PARENT")->findComponent("MATRICES")->getSelectedItem())->setParent(m_model->getMatrix(m_container->findComponent("PARENT")->findComponent("PARENTMATRIX")->getSelectedItem() - 1));
		});
		m_container->findComponent("PARENT")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]() { m_container->setPauseScreen(""); });



		m_container->addPauseScreen(new Container("KEYFRAME", {0, 0}, {500, 400}, false), PANEL_ALIGN_CENTER)->setPriorityLayer(8);
		m_container->findComponent("KEYFRAME")->addComponent(new SColorPanel({0, 0}, Globals::m_screenSize, Color(0, 0, 0, 0.5f)), PANEL_ALIGN_CENTER);
		m_container->findComponent("KEYFRAME")->addComponent(new Panel("BACKGROUND", "Rename Matrix", {0, 0}, {500, 400}, Component::Theme::PRIMARY, Component::Border::BORDER_ALL), PANEL_ALIGN_CENTER);

		m_container->findComponent("KEYFRAME")->addComponent(new CDropDown("MATRICES", "Matrices", {0, -176}, {200, 16}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			m_container->findComponent("KEYFRAME")->findComponent("MATRICES")->setList(m_model->getMatrixNames());
		})->setPriorityLayer(4);

		m_container->findComponent("KEYFRAME")->addComponent(new TextField("MATRIXNAME", "Matrix Name", {0, -120}, {200, 1}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			if(m_container->findComponent("KEYFRAME")->findComponent("MATRIXNAME")->getTitle() != "")
			{
				m_model->renameMatrix(m_container->findComponent("KEYFRAME")->findComponent("MATRICES")->getSelectedItem(), m_container->findComponent("KEYFRAME")->findComponent("MATRIXNAME")->getTitle());
				m_container->setPauseScreen("");
			}
		});

		m_container->findComponent("KEYFRAME")->addComponent(new CButton("OK", "Rename", {-40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
		{
			if(m_container->findComponent("KEYFRAME")->findComponent("MATRIXNAME")->getTitle() != "")
			{
				m_model->renameMatrix(m_container->findComponent("KEYFRAME")->findComponent("MATRICES")->getSelectedItem(), m_container->findComponent("KEYFRAME")->findComponent("MATRIXNAME")->getTitle());
				m_container->setPauseScreen("");
			}
		});
		m_container->findComponent("KEYFRAME")->addComponent(new CButton("CANCEL", "Cancel", {40, 172}, {64, 24}, 0), PANEL_ALIGN_CENTER)->setFunction([]() { m_container->setPauseScreen(""); });




		m_container->addComponent(new ContainerPanel("PANEL_INFOBAR", "", {0, 0}, {Globals::m_screenSize.x, 20}, Component::Theme::INFO, Component::Border::BORDER_TOP), PANEL_ALIGN_BOTTOM)->setPriorityLayer(-4);

		std::string* _matrixData = new std::string("");
		m_model->setDataString(_matrixData);
		m_container->findComponent("PANEL_INFOBAR")->addComponent(new DataField("DATA_MATRIX", _matrixData, {40, 0}, {200, 20}, Component::Theme::INFO), PANEL_ALIGN_LEFT);
	}
	static void clear()
	{
		delete m_container;
		m_container = 0;
	}

	static Container* getContainer()
	{
		return m_container;
	}

	~Overlay()
	{
		clear();
	}
};
