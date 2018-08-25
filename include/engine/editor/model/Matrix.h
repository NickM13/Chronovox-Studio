#pragma once

#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 4

#include "engine\utils\variable\manager\VoxelManager.h"
#include "engine\gfx\mesh\VoxelMesh.h"
#include "..\animation\keyframe\Keyframe.h"

// Data of one component of the Model
struct Matrix {
private:
	std::string m_name;
	Sint16 m_id;

	glm::vec3 m_pos;
	glm::ivec3 m_size;

	Uint16*** m_voxelData;
	Sint8*** m_faceData;
	VoxelMesh* m_mesh;

	bool m_needsRasterize;

	// Animation vars
	std::string m_parent;
	std::vector<Keyframe*> m_keyframes;

public:
	enum class OutlineType {
		NONE = 0,
		OUTLINE = 1,
		HOVERED = 2,
		SELECTED = 3
	};

	Matrix(Sint16 p_id, std::string p_name, std::string p_parent, glm::vec3 p_pos = {}, glm::ivec3 p_size = {});
	Matrix(Matrix&);
	~Matrix();
	void operator=(Matrix&);

	void setId(Sint16 p_id) { m_id = p_id; }
	void setName(std::string p_name) { m_name = p_name; }

	void setSize(glm::ivec3 p_size);
	void setPosition(glm::vec3 p_pos);
	void addPosition(glm::vec3 p_pos);

	bool setVoxel(glm::ivec3 p_pos, Voxel p_voxel);
	Voxel getVoxel(glm::ivec3 p_pos);
	Uint16 getVoxelId(glm::ivec3 p_pos);
	void shiftVoxels(glm::ivec3 p_direction);
	void flip(Sint8 p_axes, glm::vec3 p_focus);
	void rotate(Sint8 p_axes, glm::vec3 p_focus);

	Sint16 getId() { return m_id; }
	std::string &getName() { return m_name; }
	glm::vec3 getPos() { return m_pos; }
	glm::ivec3 getSize() { return m_size; }
	glm::vec3 getCenter() { return (m_pos + (glm::vec3(m_size) / glm::vec3(2))); }

	bool containsPoint(glm::ivec3 p_point);

	std::string &getParent() { return m_parent; }
	void setParent(std::string p_parent) { m_parent = p_parent; }
	void addKeyframe(Keyframe* p_keyFrame);
	void applyAnimation(GLfloat p_time);
	void clearAnimation() { m_keyframes.clear(); }

	void update();
	void renderMatrix();
	void renderShadow();
	void renderOutline(OutlineType p_outline);

	void rasterize();
};
