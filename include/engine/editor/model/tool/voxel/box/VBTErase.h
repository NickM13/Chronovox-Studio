#pragma once
#include "VoxelBoxTool.h"

class VBTErase : public VoxelBoxTool {
protected:

public:
	VBTErase();

	void inputTool();
	void updateTool();
	void renderTool();
};
