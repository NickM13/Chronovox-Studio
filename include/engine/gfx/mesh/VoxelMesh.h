#pragma once

#include "engine\utils\LOpenGL.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"
#include "engine\utils\variable\dictionary\DVoxel.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <math.h>
//#include <vector>

class VoxelMesh {
private:
	enum class MeshType {
		MESH,	// Regular
		MESHAO,	// Ambient Occlusion
		MESHDOT	// Triangle Outlines
	} m_meshType;

	std::vector<glm::vec3> m_vertices;
	std::vector<Color> m_colors;
	std::vector<glm::vec3> m_normals;
	GLuint m_vaoId, m_vboId[3];

	std::vector<glm::vec3> m_verticesWire;
	GLuint m_vaoIdWire, m_vboIdWire[1];

	Vector4<GLfloat> getAO(Vector3<Uint16> p_pos, Voxel*** p_voxels, Uint8 p_side);
public:
	VoxelMesh();
	~VoxelMesh();

	void destroyMesh();
	// Generates vertices in a vertex array to render on screen faster
	// Parameter p_voxelId is an array of voxel ids to VoxelManager converted from compressed storage
	// External voxels are stored in p_voxelIds -- adds {2, 2, 2} to p_dimensions
	void createMesh(Uint16*** p_voxelIds, Sint8*** p_faceData, Vector3<Sint16> p_dimensions);
	// Create a mesh with simulated ambient occlusion
	void createMeshAO(Uint16*** p_voxelIds, Sint8*** p_faceData, Vector3<Sint16> p_dimensions);

	std::vector<glm::vec3>& getVertices() { return m_vertices; }
	std::vector<Color>& getColors() { return m_colors; }
	std::vector<glm::vec3>& getNormals() { return m_normals; }

	void renderMesh();
	void renderMeshWireframe();
	void renderMeshShadow();
};
