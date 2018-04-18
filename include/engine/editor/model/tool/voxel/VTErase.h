#pragma once
#include "VoxelTool.h"

class VTErase : public VoxelTool {
protected:

public:
	VTErase();

	void inputSingle();
	void updateSingle();
	void renderSingle();

	void inputBox();
	void updateBox();
	void renderBox();

	void inputFill();
	void updateFill();
	void renderFill();
};
