#pragma once

#include "..\VoxelTool.h"

class VoxelSingleTool : public VoxelTool {
protected:
	// Renders outline for a single voxel
	void renderSingleMesh(bool p_inset);

	virtual void inputTool();
	virtual void updateTool();
	virtual void renderTool();
public:
	VoxelSingleTool();
	~VoxelSingleTool();

	virtual void enable();
	virtual void disable();
};
