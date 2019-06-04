#pragma once

#include "VoxelTool.h"

class VTErase : public VoxelTool {
protected:

public:
	VTErase();

	void inputTool();
	void updateTool();
	void renderTool();
};
