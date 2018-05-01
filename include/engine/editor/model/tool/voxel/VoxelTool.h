#pragma once

#include "..\Tool.h"

class VoxelTool : public Tool {
protected:
	struct BoxArea {

	};
	struct FillArea {
	protected:
		Sint32 m_fillMatrix;
		glm::ivec3 m_fillStart;
		Sint8 m_fillSide;
		std::vector<glm::ivec3> m_fillVoxels;
		std::vector<glm::vec3> m_fillMesh;

		void vectorAdd(std::vector<glm::ivec3> &list1, std::vector<glm::ivec3> list2, glm::ivec3 element);
		bool append();
		bool insert();
	public:
		void create(bool p_inset);
		void destroy();
		void use(Voxel p_fill);
		std::vector<glm::vec3>& getMesh() { return m_fillMesh; }
	};

	static glm::ivec3* m_selectedVoxel, *m_selectedVoxelOffset;
	static Sint8* m_selectedSide;
	static Color* m_color;

	glm::ivec3 m_boxStart, m_boxEnd;
	bool m_boxReplace;
	bool m_boxing;

	FillArea* m_fillArea;

	void box(glm::ivec3 p_pos, Voxel p_voxel);

	// Renders outline for a single voxel
	void renderSingleMesh(bool p_inset);
	// Renders outline for a box 
	// Just shows area of effect, not what is actually being changed, e.g. replace tool
	void renderBoxMesh(bool p_insetVoxel, bool p_insetBox);
	// Renders outline for selected fill voxels (TODO: make voxels highlighted instead of outlined)
	void renderFillMesh(bool p_insetVoxel, bool p_insetBox);

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
	
	static void init(glm::ivec3* p_voxel, glm::ivec3* p_voxelOffset, Sint8* p_selectedSide, Color* p_color);

	virtual void enable();
	virtual void disable();

	void input();
	void update();
	void render();
};
