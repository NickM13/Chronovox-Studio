#pragma once
#include "VoxelFillTool.h"

class VFTErase : public VoxelFillTool {
protected:

public:
	VFTErase();

	void inputTool();
	void updateTool();
	void renderTool();
};
