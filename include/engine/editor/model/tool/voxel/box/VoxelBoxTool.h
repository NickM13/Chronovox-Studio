#pragma once

#include "..\VoxelTool.h"

class VoxelBoxTool : public VoxelTool {
protected:
	glm::ivec3 m_boxStart, m_boxEnd;
	bool m_boxReplace;
	bool m_boxing;

	void box(glm::ivec3 p_pos, Voxel p_voxel);

	// Renders outline for a box 
	// Just shows area of effect, not what is actually being changed, e.g. replace tool
	void renderBoxMesh(bool p_insetVoxel, bool p_insetBox);

	virtual void inputTool();
	virtual void updateTool();
	virtual void renderTool();
public:
	VoxelBoxTool();
	~VoxelBoxTool();

	virtual void enable();
	virtual void disable();
};
