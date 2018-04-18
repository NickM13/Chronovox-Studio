#pragma once
#include "VoxelTool.h"

class VTReplace : public VoxelTool {
protected:

public:
	VTReplace();

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
