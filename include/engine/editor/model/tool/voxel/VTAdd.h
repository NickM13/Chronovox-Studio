#pragma once
#include "VoxelTool.h"

class VTAdd : public VoxelTool {
protected:

public:
	VTAdd();

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
