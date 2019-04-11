#pragma once
#include "VoxelSingleTool.h"

class VSTErase : public VoxelSingleTool {
protected:

public:
	VSTErase();

	void inputTool();
	void updateTool();
	void renderTool();
};
