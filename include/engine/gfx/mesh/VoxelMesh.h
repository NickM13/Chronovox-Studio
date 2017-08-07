#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"

#include "engine\utils\OpenGL.h"

#include "engine\utils\variable\manager\ColorManager.h"

#include <vector>

class VoxelMesh
{
public:
	// Generates vertices in a vertex array to render on screen faster
	// Parameter p_voxelId is an array of voxel ids to VoxelManager converted from compressed storage
	// External voxels are stored in p_voxelIds -- adds {2, 2, 2} to p_dimensions
	void createMesh(Uint16*** p_voxelIds, Sint8*** p_faceData, Vector3<Sint16> p_dimensions);

	// TODO: Returns a buffer for vertex array.  Only useful for static objects like models.
	//void createMeshBuffer();

	void renderMesh();

private:
	std::vector<Vector3<GLfloat>> m_vertices;
	std::vector<Color> m_colors;
};
