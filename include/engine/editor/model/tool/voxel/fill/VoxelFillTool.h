#pragma once

#include "..\VoxelTool.h"

class VoxelFillTool : public VoxelTool {
public:
	struct Coord {
	public:
		glm::ivec3 vec;
		Coord(glm::ivec3 p_vec) : vec(p_vec) {};
		bool operator<(const Coord c) const {
			return (vec.x < c.vec.x
				|| vec.x == c.vec.x && (vec.y < c.vec.y
					|| vec.y == c.vec.y && vec.z < c.vec.z));
		}
	};
	struct FillArea {
	protected:
		Sint32 m_fillMatrix = 0;
		glm::ivec3 m_fillStart = {};
		Sint8 m_fillSide = 0;
		std::vector<glm::ivec3> m_fillVoxels = {};
		std::vector<glm::vec3> m_fillMesh = {};

		void vectorAdd(std::vector<glm::ivec3>& list1, std::vector<glm::ivec3> list2, glm::ivec3 element);
		bool append();
		bool insert();
		void createMesh();
	public:
		void create(bool p_inset);
		void destroy();
		void use(Voxel p_fill);
		std::vector<glm::vec3>& getMesh() { return m_fillMesh; }
	};
protected:
	FillArea* m_fillArea;

	// Renders outline for selected fill voxels (TODO: make voxels highlighted instead of outlined)
	void renderFillMesh();

	virtual void inputTool();
	virtual void updateTool();
	virtual void renderTool();
public:
	VoxelFillTool();
	~VoxelFillTool();

	virtual void enable();
	virtual void disable();
};
