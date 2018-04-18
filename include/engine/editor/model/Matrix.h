#pragma once

#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 4

#include "engine\utils\variable\manager\VoxelManager.h"
#include "engine\gfx\mesh\VoxelMesh.h"
#include "..\animation\keyframe\Keyframe.h"

enum OutlineType
{
	NONE = 0,
	OUTLINE = 1,
	HOVERED = 2,
	SELECTED = 3
};

// Data of one component of the Model
struct Matrix {
private:
	std::string m_name;
	Sint16 m_id;

	Vector3<GLfloat> m_pos;
	Vector3<Sint16> m_size;

	Uint16*** m_voxelData;
	Sint8*** m_faceData;
	VoxelMesh* m_mesh;

	bool m_needsRasterize;

	// Animation vars
	std::string m_parent;
	std::vector<Keyframe*> m_keyframes;

public:
	Matrix(Sint16 p_id, std::string p_name, std::string p_parent, Vector3<GLfloat> p_pos = {}, Vector3<Sint16> p_size = {});
	Matrix(Matrix&);
	~Matrix();
	void operator=(Matrix&);

	void setId(Sint16 p_id) { m_id = p_id; }
	void setName(std::string p_name) { m_name = p_name; }

	void setSize(Vector3<Sint32> p_size);
	void setPosition(Vector3<GLfloat> p_pos);
	void addPosition(Vector3<GLfloat> p_pos);

	bool setVoxel(Vector3<Sint32> p_pos, Voxel p_voxel);
	Voxel getVoxel(Vector3<Sint32> p_pos);
	Uint16 getVoxelId(Vector3<Sint32> p_pos);
	void shiftVoxels(Vector3<Sint32> p_direction);
	void flip(Sint8 p_axes, Vector3<GLfloat> p_focus);
	void rotate(Sint8 p_axes, Vector3<GLfloat> p_focus);

	Sint16 getId() { return m_id; }
	std::string &getName() { return m_name; }
	Vector3<GLfloat> getPos() { return m_pos; }
	Vector3<Sint32> getSize() { return m_size; }
	Vector3<GLfloat> getCenter() { return m_pos + Vector3<GLfloat>(m_size) / 2; }

	bool containsPoint(Vector3<Sint32> p_point);

	std::string &getParent() { return m_parent; }
	void setParent(std::string p_parent) { m_parent = p_parent; }
	void addKeyframe(Keyframe* p_keyFrame);
	void applyAnimation(GLfloat p_time);
	void clearAnimation() { m_keyframes.clear(); }

	void update();
	void renderMatrix();
	void renderOutline(OutlineType p_outline);

	void rasterize();
};
