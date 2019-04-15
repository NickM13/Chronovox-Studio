#pragma once

#include "..\Tool.h"

class VoxelTool : public Tool {
protected:
	const GLfloat SELECT_CORRECTION = 0.025f;

	static glm::ivec3* m_selectedVoxel, *m_selectedVoxelOffset;
	static Sint8* m_selectedSide;
	static Color* m_color;

	virtual void inputTool();
	virtual void updateTool();
	virtual void renderTool();
public:
	VoxelTool();
	~VoxelTool();
	
	static void init(glm::ivec3* p_voxel, glm::ivec3* p_voxelOffset, Sint8* p_selectedSide, Color* p_color);

	virtual void enable();
	virtual void disable();

	void input();
	void update();
	void render();
};
