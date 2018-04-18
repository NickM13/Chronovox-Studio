#pragma once
#include "engine\editor\model\Model.h"
#include "engine\gfx\LGui.h"

class PropertiesDialog : public ContainerPanel {
private:
	Model* m_model;
public:
	PropertiesDialog(Model* m);
};
