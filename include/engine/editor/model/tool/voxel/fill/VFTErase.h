#pragma once
#include "..\VoxelTool.h"

class VFTErase : public VoxelTool {
protected:

public:
	VFTErase();

	void inputTool();
	void updateTool();
	void renderTool();
};
