#pragma once

#include "..\Tool.h"

class VoxelTool : public Tool {
protected:
	static Vector3<Sint32>* m_selectedVoxel, *m_selectedVoxelOffset;
	static Sint8* m_selectedSide;
	static Color* m_color;

	Vector3<Sint32> m_boxStart, m_boxEnd;
	bool m_boxReplace;
	bool m_boxing;

	Sint32 m_fillMatrix;
	Vector3<Sint32> m_fillStart;
	Sint8 m_fillSide;
	std::vector<Vector3<Sint32>> m_fillVoxels;

	void box(Vector3<Sint32> p_pos, Voxel p_voxel);
	void vectorAdd(std::vector<Vector3<Sint32>> &list1, std::vector<Vector3<Sint32>> list2, Vector3<Sint32> element);
	void fillAppend();
	void fillInsert();
	void fill(Voxel p_voxel);

	virtual void inputSingle();
	virtual void updateSingle();
	virtual void renderSingle();

	virtual void inputBox();
	virtual void updateBox();
	virtual void renderBox();

	virtual void inputFill();
	virtual void updateFill();
	virtual void renderFill();
public:
	VoxelTool();
	~VoxelTool();
	
	static void init(Vector3<Sint32>* p_voxel, Vector3<Sint32>* p_voxelOffset, Sint8* p_selectedSide, Color* p_color);

	virtual void enable();
	virtual void disable();

	void input();
	void update();
	void render();
};
