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

class Gui {
private:
	static Container* m_mainContainer;
public:
	static void init();

	static Container* getContainer();
	static void openDialog(CDialog* p_dialog);

	static void resize();

	static void input(Sint8& p_iFlags);
	static void update(GLfloat p_deltaTime);
	static void render();
};
