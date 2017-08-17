#pragma once

#define FLIP_X 1
#define FLIP_Y 2
#define FLIP_Z 4

#include "engine\utils\variable\manager\VoxelManager.h"
#include "engine\gfx\mesh\VoxelMesh.h"

enum OutlineType
{
	NONE = 0,
	OUTLINE = 1,
	HOVERED = 2,
	SELECTED = 3
};

// Data of one component of the Model
struct Matrix
{
public:
	struct KeyFrame
	{
		Sint32 m_startTime;	// In Milliseconds (1000)
		Sint32 m_length;	// In Milliseconds (1000)
		Vector3<Sint32> m_focusPoint;	// Divide by 2
		Vector3<Sint32> m_translation;	// Divide by 4
		Vector3<Sint32> m_rotation;

		KeyFrame() {};
		KeyFrame(Sint32 p_startTime, Sint32 p_length, Vector3<Sint32> p_focusPoint, Vector3<Sint32> p_translation, Vector3<Sint32> p_rotation)
		{
			m_startTime = p_startTime;
			m_length = p_length;
			m_focusPoint = p_focusPoint;
			m_translation = p_translation;
			m_rotation = p_rotation;
		}
	};

	Matrix(std::string p_name, Sint16 p_id, Vector3<GLfloat> p_pos = {}, Vector3<Sint16> p_size = {});
	Matrix(Matrix&);
	~Matrix();
	void operator=(Matrix&);

	void setName(std::string p_name) { m_name = p_name; }
	void setId(Sint16 p_id) { m_id = p_id; };

	void setSize(Vector3<Sint32> p_size);
	void setPosition(Vector3<GLfloat> p_pos);
	void addPosition(Vector3<GLfloat> p_pos);

	bool setVoxel(Vector3<Sint32> p_pos, Voxel p_voxel);
	Voxel getVoxel(Vector3<Sint32> p_pos);
	Uint16 getVoxelId(Vector3<Sint32> p_pos);
	void shiftVoxels(Vector3<Sint32> p_direction);
	void flip(Sint8 p_axes);

	std::string &getName() { return m_name; };
	Vector3<GLfloat> getPos() { return m_pos; };
	Vector3<Sint32> getSize() { return m_size; };
	Sint16 getId() { return m_id; };

	void setParent(Matrix* p_matrix);
	void addKeyFrame(KeyFrame p_keyFrame);
	void setAnimation(GLfloat p_time);

	Matrix* getParent() { return m_parent; };

	void update();
	void render(OutlineType p_highlight, GLfloat p_animTime);

	void rasterize();

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
	Matrix* m_parent;
	std::vector<KeyFrame> m_keyFrames;
};
