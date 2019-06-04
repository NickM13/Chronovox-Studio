#pragma once

#include "..\Tool.h"

class VoxelTool : public Tool {
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
	enum class VoxelMode {
		SINGLE,
		BOX
	} m_voxelMode;

	const GLfloat SELECT_CORRECTION = 0.025f;

	static glm::ivec3* m_selectedVoxel, *m_selectedVoxelOffset;
	static Sint8* m_selectedSide;
	static Color* m_color;

	glm::ivec3 m_boxStart, m_boxEnd;
	bool m_boxReplace = false;
	bool m_boxing = false;


	FillArea* m_fillArea = 0;

	// Renders outline for a single voxel
	void renderSingleMesh(bool p_inset);
	// Renders outline for a box
	void renderBoxMesh(bool p_insetVoxel, bool p_insetBox);
	// Renders outline for selected fill voxels (TODO: make voxels highlighted instead of outlined)
	void renderFillMesh();

	void box(glm::ivec3 p_pos, Voxel p_voxel);

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
