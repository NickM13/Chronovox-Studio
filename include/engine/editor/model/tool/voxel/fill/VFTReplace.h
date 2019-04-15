#pragma once
#include "VoxelFillTool.h"

class VFTReplace : public VoxelFillTool {
protected:

public:
	VFTReplace();

	void inputTool();
	void updateTool();
	void renderTool();
};
