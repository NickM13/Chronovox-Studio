#pragma once

#include "component\Component.h"
#include "component\button\Button.h"
#include "component\button\ButtonToggle.h"
#include "component\button\ButtonRadio.h"
#include "component\container\BackPanel.h"
#include "component\container\ColorPanel.h"
#include "component\container\Container.h"
#include "component\container\ContainerPanel.h"
#include "component\container\Panel.h"
#include "component\dialog\Dialog.h"
#include "component\field\DataField.h"
#include "component\field\FileField.h"
#include "component\field\NumberField.h"
#include "component\field\Text.h"
#include "component\field\TextField.h"
#include "component\field\TextFile.h"
#include "component\frequency\Counter.h"
#include "component\frequency\Slider.h"
#include "component\frequency\SliderV.h"
#include "component\icon\Icon.h"
#include "component\list\DropDown.h"
#include "component\list\List.h"
#include "component\list\Menubar.h"
#include "component\list\TabBar.h"
#include "component\list\Tileset.h"
#include "global\GGui.h"

#include "engine\utils\global\GLua.h"

class Gui {
private:
	static Container* m_mainContainer;
	static std::vector<CDialog*> m_dialogList;
	static std::vector<Component*> m_compBuildList;
	static Sint32 m_cHeight; // Caret height

	static Sint32 m_actionWidth;
	static Sint32 m_actionSpacing;

	// Dialog functions for lua scripts
	static void create(std::string p_title, Sint32 width, Sint32 height);
	static void addHeader(std::string p_text);
	static void addText(std::string p_text);
	static void addLineBreak();
	static void addTextField(std::string p_compName, std::string p_title, std::string p_default);
	static void addTextArea(std::string p_compName, Sint32 height);
	static void addNumField(std::string p_compName, std::string p_title, bool p_isFloat, Sint32 p_lowerBound, Sint32 p_upperBound, Sint32 p_count, Sint32 p_default);
	static void addTextFile(std::string p_compName, std::string p_filename);
	static void addDropdown(std::string p_compName, std::string p_title, LuaRef p_optionList);
	static void addOption(std::string p_option, Sint32 p_functionIndex);
	static void open();
public:
	static void init();

	static Container* getContainer();
	// Open dialog lua script (from res/script/dialog/)
	static CDialog* openDialog(std::string p_filename);
	static CDialog* topDialog() { return m_dialogList.back(); }
	static void closeTopDialog();

	//static void openDialog(CDialog* p_dialog);
	static bool isLoading() { return m_mainContainer->getLoading() < 1; }
	static void setLoading(GLfloat p_loadPercent);

	static void resize();

	static void input(Sint8& p_iFlags);
	static void update(GLfloat p_deltaTime);
	static void render();
};
