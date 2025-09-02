#include "engine\gfx\mesh\VoxelMesh.h"

#include "engine\gfx\shader\Shader.h"
#include "engine\editor\camera\Camera.h"

VoxelMesh::VoxelMesh() {
	// No longer initialize here due to OpenGL being finicky
	// when running in a multi-threaded application.
	// Easier to just singleton-style gen on the run

	/*glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);
	glGenBuffers(3, m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[1]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[2]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

VoxelMesh::~VoxelMesh() {
	if (m_vaoId != 0) {
		glDeleteVertexArrays(1, &m_vaoIdWire);
		glDeleteBuffers(3, m_vboIdWire);

		glDeleteVertexArrays(1, &m_vaoId);
		glDeleteBuffers(3, m_vboId);
	}
	destroyMesh();
}

void VoxelMesh::destroyMesh() {
	m_colors.clear();
	m_normals.clear();
	m_vertices.clear();
	m_verticesWire.clear();
}

Vector4<GLfloat> VoxelMesh::getAO(Vector3<Uint16> p_pos, Voxel*** p_voxels, Uint8 p_side) {
	GLfloat intensity = 12;
	GLfloat ao00, ao10, ao11, ao01;
	ao00 = ao10 = ao11 = ao01 = 0;
	switch (p_side) {
	case 0: // North
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType != 0);
		break;
	case 1: // South
		ao00 = GLfloat(p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType != 0);
		break;
	case 2: // West
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		break;
	case 3: // East
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType != 0);
		break;
	case 4: // Top
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		break;
	case 5: // Bottom
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType != 0);
		break;
	}
	return (Vector4<GLfloat>(1.f - std::fmaxf(0, std::fminf(2.25f, ao00 + 0.25f)) / intensity, 1.f - std::fmaxf(0, std::fminf(2.25f, ao10 + 0.25f)) / intensity, 1.f - std::fmaxf(0, std::fminf(2.25f, ao11 + 0.25f)) / intensity, 1.f - std::fmaxf(0, std::fminf(2.25f, ao01 + 0.25f) / intensity)));
}

void VoxelMesh::createMesh(Uint16*** p_voxelIds, Sint8*** p_faceData, Vector3<Sint16> p_dimensions) {
	destroyMesh();

	m_meshType = MeshType::MESH;
	//Logger::logDiagnostic("Creating VoxelMesh (No AO), size {" + p_dimensions.toString() + "}...");
	GLfloat start = glfwGetTime();
	
	glm::vec3 normals[6] = {
		glm::vec3(-1,  0,  0),
		glm::vec3( 1,  0,  0),
		glm::vec3( 0,  0,  1),
		glm::vec3( 0,  0, -1),
		glm::vec3( 0,  1,  0),
		glm::vec3( 0, -1,  0)
	};

	// Outside voxels are for continuing lighting, and are not rendered
	Voxel*** _voxels;
	Sint8*** _faces;
	_voxels = new Voxel**[p_dimensions.x + 2];
	_faces = new Sint8**[p_dimensions.x + 2];
	for(Uint16 x = 0; x < p_dimensions.x + 2; x++) {
		_voxels[x] = new Voxel*[p_dimensions.y + 2];
		_faces[x] = new Sint8*[p_dimensions.y + 2];
		for(Uint16 y = 0; y < p_dimensions.y + 2; y++) {
			_voxels[x][y] = new Voxel[p_dimensions.z + 2];
			_faces[x][y] = new Sint8[p_dimensions.z + 2];
			for(Uint16 z = 0; z < p_dimensions.z + 2; z++) {
				_voxels[x][y][z] = DVoxel::getVoxel(p_voxelIds[x][y][z]);
				if(_voxels[x][y][z].interactionType)
					_faces[x][y][z] = p_faceData[x][y][z];
				else
					_faces[x][y][z] = 0;
			}
		}
	}

	Uint16 x, y, z;
	Uint16 x2, y2, z2;
	Uint16 _interactionType;
	Color color;
	bool _done1, _done2;
	// North		X+
	for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
		x = x1 + 1;
		for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
			y = y1 + 1;
			for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
				z = z1 + 1;
				if(_faces[x][y][z] & FACE_NORTH) {
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					y2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done1 = true;
							break;
						}
						if(!(_faces[x][y + y2][z] & FACE_NORTH) || _voxels[x][y + y2][z].interactionType != _interactionType || _voxels[x][y + y2][z].color != color)
							_done1 = true;
						if(!_done1)
							y2++;
					}
					while(!_done2) {
						if(z2 + z1 >= p_dimensions.z) {
							z2 = p_dimensions.z - z1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < y2; i++) {
							if(!(_faces[x][y + i][z + z2] & FACE_NORTH) || _voxels[x][y + i][z + z2].interactionType != _interactionType || _voxels[x][y + i][z + z2].color != color) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							z2++;
					}
					for(Uint16 i = 0; i < y2; i++)
						for(Uint16 j = 0; j < z2; j++)
							_faces[x][y + i][z + j] -= FACE_NORTH;

					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1, z1 + z2));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1 + y2, z1 + z2));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1 + y2, z1));

					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));		m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));		m_verticesWire.push_back(glm::vec3(x1, y1, z1));

					for (Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[0]);
				}
			}
		}
	}
	// South		X-
	for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
		x = x1 + 1;
		for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
			y = y1 + 1;
			for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
				z = z1 + 1;
				if(_faces[x][y][z] & FACE_SOUTH) {
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					y2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done1 = true;
							break;
						}
						if(!(_faces[x][y + y2][z] & FACE_SOUTH) || _voxels[x][y + y2][z].interactionType != _interactionType || _voxels[x][y + y2][z].color != color)
							_done1 = true;
						if(!_done1)
							y2++;
					}
					while(!_done2) {
						if(z2 + z1 >= p_dimensions.z) {
							z2 = p_dimensions.z - z1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < y2; i++) {
							if(!(_faces[x][y + i][z + z2] & FACE_SOUTH) || _voxels[x][y + i][z + z2].interactionType != _interactionType || _voxels[x][y + i][z + z2].color != color) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							z2++;
					}

					for(Uint16 i = 0; i < y2; i++)
						for(Uint16 j = 0; j < z2; j++)
							_faces[x][y + i][z + j] -= FACE_SOUTH;

					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + 1, y1, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + 1, y1 + y2, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + 1, y1, z1 + z2));

					m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1));		m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1 + z2));		m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1));

					for (Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[1]);
				}
			}
		}
	}
	// East			Z+
	for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
		z = z1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
			x = x1 + 1;
			for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
				y = y1 + 1;
				if(_faces[x][y][z] & FACE_EAST)
				{
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					x2 = y2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_EAST) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != color)
							_done1 = true;
						if(!_done1)
							x2++;
					}
					while(!_done2) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < x2; i++) {
							if(!(_faces[x + i][y + y2][z] & FACE_EAST) || _voxels[x + i][y + y2][z].interactionType != _interactionType || _voxels[x + i][y + y2][z].color != color) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							y2++;
					}
					for(Uint16 i = 0; i < x2; i++)
						for(Uint16 j = 0; j < y2; j++)
							_faces[x + i][y + j][z] -= FACE_EAST;

					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1, z1 + 1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1 + 1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1 + y2, z1 + 1));

					m_verticesWire.push_back(glm::vec3(x1, y1, z1 + 1));			m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + 1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + 1));		m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));
					m_verticesWire.push_back(glm::vec3(x1, y1, z1 + 1));			m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));	m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + 1));
					m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + 1));		m_verticesWire.push_back(glm::vec3(x1, y1, z1 + 1));

					for (Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[2]);
				}
			}
		}
	}
	// West			Z-
	for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
		z = z1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
			x = x1 + 1;
			for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
				y = y1 + 1;
				if(_faces[x][y][z] & FACE_WEST) {
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					x2 = y2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_WEST) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != color)
							_done1 = true;
						if(!_done1)
							x2++;
					}
					while(!_done2) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < x2; i++) {
							if(!(_faces[x + i][y + y2][z] & FACE_WEST) || _voxels[x + i][y + y2][z].interactionType != _interactionType || _voxels[x + i][y + y2][z].color != color) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							y2++;
					}
					for(Uint16 i = 0; i < x2; i++)
						for(Uint16 j = 0; j < y2; j++)
							_faces[x + i][y + j][z] -= FACE_WEST;

					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1 + y2, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + x2, y1 + y2, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1));

					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));		m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1));	m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));		m_verticesWire.push_back(glm::vec3(x1, y1, z1));

					for (Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[3]);
				}
			}
		}
	}
	// Top			Y+
	for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
		y = y1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
			x = x1 + 1;
			for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
				z = z1 + 1;
				if(_faces[x][y][z] & FACE_TOP) {
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					x2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_TOP) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != color)
							_done1 = true;
						if(!_done1)
							x2++;
					}
					while(!_done2) {
						if(z2 + z1 >= p_dimensions.z) {
							z2 = p_dimensions.z - z1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < x2; i++) {
							if(!(_faces[x + i][y][z + z2] & FACE_TOP) || _voxels[x + i][y][z + z2].interactionType != _interactionType || _voxels[x + i][y][z + z2].color != color) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							z2++;
					}
					for(Uint16 i = 0; i < x2; i++)
						for(Uint16 j = 0; j < z2; j++)
							_faces[x + i][y][z + j] -= FACE_TOP;

					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1 + 1, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1 + 1, z1 + z2));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + x2, y1 + 1, z1));

					m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1));			m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1 + z2));		m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1));			m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1));		m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1));

					for (Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[4]);
				}
			}
		}
	}
	// Bottom		Y-
	for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
		y = y1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
			x = x1 + 1;
			for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
				z = z1 + 1;
				if(_faces[x][y][z] & FACE_BOTTOM) {
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					x2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_BOTTOM) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != color)
							_done1 = true;
						if(!_done1)
							x2++;
					}
					while(!_done2) {
						if(z2 + z1 >= p_dimensions.z) {
							z2 = p_dimensions.z - z1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < x2; i++) {
							if(!(_faces[x + i][y][z + z2] & FACE_BOTTOM) || _voxels[x + i][y][z + z2].interactionType != _interactionType || _voxels[x + i][y][z + z2].color != color) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							z2++;
					}
					for(Uint16 i = 0; i < x2; i++)
						for(Uint16 j = 0; j < z2; j++)
							_faces[x + i][y][z + j] -= FACE_BOTTOM;

					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1 + z2));
					m_colors.push_back(color); m_vertices.push_back(glm::vec3(x1, y1, z1 + z2));

					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));		m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));		m_verticesWire.push_back(glm::vec3(x1, y1, z1));

					for (Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[5]);
				}
			}
		}
	}
	for(x = 0; x < p_dimensions.x + 2; x++) {
		for(y = 0; y < p_dimensions.y + 2; y++) {
			delete[] _voxels[x][y];
			delete[] _faces[x][y];
		}
		delete[] _voxels[x];
		delete[] _faces[x];
	}
	delete[] _voxels;
	delete[] _faces;

	if (m_vertices.size() == 0) return;

	if (!m_vaoId) {
		glGenVertexArrays(1, &m_vaoIdWire);
		glBindVertexArray(m_vaoIdWire);
		glGenBuffers(1, m_vboIdWire);

		glGenVertexArrays(1, &m_vaoId);
		glBindVertexArray(m_vaoId);
		glGenBuffers(3, m_vboId);
	} else {
		glBindVertexArray(m_vaoId);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[0]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat) * 3, &m_vertices[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[1]);
	glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(GLubyte) * 4, &m_colors[0].r, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[2]);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLfloat) * 3, &m_normals[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(m_vaoIdWire);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboIdWire[0]);
	glBufferData(GL_ARRAY_BUFFER, m_verticesWire.size() * sizeof(GLfloat) * 3, &m_verticesWire[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Logger::logDiagnostic("Created mesh in " + std::to_string(glfwGetTime() - start) + " seconds");
	//Logger::logDiagnostic("Mesh vertices(" + std::to_string(m_vertices.size()) + "), colors(" + std::to_string(m_colors.size()) + ")");
}

void VoxelMesh::createMeshAO(Uint16*** p_voxelIds, Sint8*** p_faceData, Vector3<Sint16> p_dimensions) {
	destroyMesh();

	m_meshType = MeshType::MESHAO;
	Logger::logDiagnostic("Creating VoxelMesh (AO), size {" + p_dimensions.toString() + "}...");
	GLfloat start = glfwGetTime();
	
	glm::vec3 normals[6] = {
		glm::vec3(-1,  0,  0),
		glm::vec3( 1,  0,  0),
		glm::vec3( 0,  0,  1),
		glm::vec3( 0,  0, -1),
		glm::vec3( 0,  1,  0),
		glm::vec3( 0, -1,  0)
	};
	// Outside voxels are for continuing lighting, and are not rendered
	Voxel*** _voxels = new Voxel**[p_dimensions.x + 2];
	Sint8*** _faces = new Sint8**[p_dimensions.x + 2];
	for(Uint16 x = 0; x < p_dimensions.x + 2; x++) {
		_voxels[x] = new Voxel*[p_dimensions.y + 2];
		_faces[x] = new Sint8*[p_dimensions.y + 2];
		for(Uint16 y = 0; y < p_dimensions.y + 2; y++) {
			_voxels[x][y] = new Voxel[p_dimensions.z + 2];
			_faces[x][y] = new Sint8[p_dimensions.z + 2];
			for(Uint16 z = 0; z < p_dimensions.z + 2; z++) {
				_voxels[x][y][z] = DVoxel::getVoxel(p_voxelIds[x][y][z]);
				if(_voxels[x][y][z].interactionType)
					_faces[x][y][z] = p_faceData[x][y][z];
				else
					_faces[x][y][z] = 0;
			}
		}
	}

	Uint16 x, y, z;
	Uint16 x2, y2, z2;
	Vector4<GLfloat> ao;
	GLfloat p[4];
	Uint16 _interactionType;
	Color color;
	bool _done1, _done2;
	// North		X+
	for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
		x = x1 + 1;
		for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
			y = y1 + 1;
			for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
				z = z1 + 1;
				if(_faces[x][y][z] & FACE_NORTH) {
					ao = getAO(Vector3<Uint16>(x, y, z), _voxels, 0);
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					y2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done1 = true;
							break;
						}
						if(!(_faces[x][y + y2][z] & FACE_NORTH) || _voxels[x][y + y2][z].interactionType != _interactionType || _voxels[x][y + y2][z].color != color ||
							!(getAO(Vector3<Uint16>(x, y + y2, z), _voxels, 0) == ao))
							_done1 = true;
						if(!_done1)
							y2++;
					}
					while(!_done2) {
						if(z2 + z1 >= p_dimensions.z) {
							z2 = p_dimensions.z - z1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < y2; i++) {
							if(!(_faces[x][y + i][z + z2] & FACE_NORTH) || _voxels[x][y + i][z + z2].interactionType != _interactionType || _voxels[x][y + i][z + z2].color != color ||
								!(getAO(Vector3<Uint16>(x, y + i, z + z2), _voxels, 0) == ao)) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							z2++;
					}
					for(Uint16 i = 0; i < y2; i++)
						for(Uint16 j = 0; j < z2; j++)
							_faces[x][y + i][z + j] -= FACE_NORTH;
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors.push_back(color * ao.x1);						m_vertices.push_back(glm::vec3(x1, y1, z1));
						m_colors.push_back(color * ao.y2);						m_vertices.push_back(glm::vec3(x1, y1, z1 + z2));
						m_colors.push_back(color * ao.x2);						m_vertices.push_back(glm::vec3(x1, y1 + y2, z1 + z2));
						m_colors.push_back(color * ao.y1);						m_vertices.push_back(glm::vec3(x1, y1 + y2, z1));
						m_verticesWire.push_back(glm::vec3(x1, y1, z1));		m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + z2));
					}
					else {
						m_colors.push_back(color * ao.y2);						m_vertices.push_back(glm::vec3(x1, y1, z1 + z2));
						m_colors.push_back(color * ao.x2);						m_vertices.push_back(glm::vec3(x1, y1 + y2, z1 + z2));
						m_colors.push_back(color * ao.y1);						m_vertices.push_back(glm::vec3(x1, y1 + y2, z1));
						m_colors.push_back(color * ao.x1);						m_vertices.push_back(glm::vec3(x1, y1, z1));
						m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));
					}
					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));		m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));		m_verticesWire.push_back(glm::vec3(x1, y1, z1));

					for(Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[0]);
				}
			}
		}
	}
	// South		X-
	for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
		x = x1 + 1;
		for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
			y = y1 + 1;
			for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
				z = z1 + 1;
				if(_faces[x][y][z] & FACE_SOUTH) {
					ao = getAO(Vector3<Uint16>(x, y, z), _voxels, 1);
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					y2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done1 = true;
							break;
						}
						if(!(_faces[x][y + y2][z] & FACE_SOUTH) || _voxels[x][y + y2][z].interactionType != _interactionType || _voxels[x][y + y2][z].color != color ||
							!(getAO(Vector3<Uint16>(x, y + y2, z), _voxels, 1) == ao))
							_done1 = true;
						if(!_done1)
							y2++;
					}
					while(!_done2) {
						if(z2 + z1 >= p_dimensions.z) {
							z2 = p_dimensions.z - z1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < y2; i++) {
							if(!(_faces[x][y + i][z + z2] & FACE_SOUTH) || _voxels[x][y + i][z + z2].interactionType != _interactionType || _voxels[x][y + i][z + z2].color != color ||
								!(getAO(Vector3<Uint16>(x, y + i, z + z2), _voxels, 1) == ao)) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							z2++;
					}

					for(Uint16 i = 0; i < y2; i++)
						for(Uint16 j = 0; j < z2; j++)
							_faces[x][y + i][z + j] -= FACE_SOUTH;
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1 + 1, y1, z1));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + 1, y1 + y2, z1));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1 + 1, y1, z1 + z2));
						m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1)); m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));
					}
					else {
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1 + 1, y1, z1 + z2));
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1 + 1, y1, z1));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + 1, y1 + y2, z1));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));
						m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1 + z2)); m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1));
					}
					m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1));		m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + 1, y1 + y2, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1 + z2));		m_verticesWire.push_back(glm::vec3(x1 + 1, y1, z1));

					for(Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[1]);
				}
			}
		}
	}
	// East			Z+
	for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
		z = z1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
			x = x1 + 1;
			for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
				y = y1 + 1;
				if(_faces[x][y][z] & FACE_EAST)
				{
					ao = getAO(Vector3<Uint16>(x, y, z), _voxels, 2);
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					x2 = y2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_EAST) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != color ||
							!(getAO(Vector3<Uint16>(x + x2, y, z), _voxels, 2) == ao))
							_done1 = true;
						if(!_done1)
							x2++;
					}
					while(!_done2) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < x2; i++) {
							if(!(_faces[x + i][y + y2][z] & FACE_EAST) || _voxels[x + i][y + y2][z].interactionType != _interactionType || _voxels[x + i][y + y2][z].color != color ||
								!(getAO(Vector3<Uint16>(x + i, y + y2, z), _voxels, 2) == ao)) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							y2++;
					}
					for(Uint16 i = 0; i < x2; i++)
						for(Uint16 j = 0; j < y2; j++)
							_faces[x + i][y + j][z] -= FACE_EAST;
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1, y1, z1 + 1));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1 + 1));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1, y1 + y2, z1 + 1));
						m_verticesWire.push_back(glm::vec3(x1, y1, z1 + 1)); m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));
					}
					else {
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1, y1 + y2, z1 + 1));
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1, y1, z1 + 1));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1 + 1));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));
						m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + 1)); m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + 1));
					}
					m_verticesWire.push_back(glm::vec3(x1, y1, z1 + 1));			m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + 1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + 1));		m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1 + 1));	m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + 1));
					m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1 + 1));		m_verticesWire.push_back(glm::vec3(x1, y1, z1 + 1));

					for(Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[2]);
				}
			}
		}
	}
	// West			Z-
	for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
		z = z1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
			x = x1 + 1;
			for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
				y = y1 + 1;
				if(_faces[x][y][z] & FACE_WEST) {
					ao = getAO(Vector3<Uint16>(x, y, z), _voxels, 3);
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					x2 = y2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_WEST) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != color ||
							!(getAO(Vector3<Uint16>(x + x2, y, z), _voxels, 3) == ao))
							_done1 = true;
						if(!_done1)
							x2++;
					}
					while(!_done2) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < x2; i++) {
							if(!(_faces[x + i][y + y2][z] & FACE_WEST) || _voxels[x + i][y + y2][z].interactionType != _interactionType || _voxels[x + i][y + y2][z].color != color ||
								!(getAO(Vector3<Uint16>(x + i, y + y2, z), _voxels, 3) == ao)) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							y2++;
					}
					for(Uint16 i = 0; i < x2; i++)
						for(Uint16 j = 0; j < y2; j++)
							_faces[x + i][y + j][z] -= FACE_WEST;
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1, y1, z1));
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1, y1 + y2, z1));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + x2, y1 + y2, z1));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1));
						m_verticesWire.push_back(glm::vec3(x1, y1, z1)); m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1));
					}
					else {
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1, y1 + y2, z1));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + x2, y1 + y2, z1));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1));
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1, y1, z1));
						m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1)); m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));
					}
					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1, y1 + y2, z1));		m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + y2, z1));	m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));		m_verticesWire.push_back(glm::vec3(x1, y1, z1));

					for(Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[3]);
				}
			}
		}
	}
	// Top			Y+
	for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
		y = y1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
			x = x1 + 1;
			for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
				z = z1 + 1;
				if(_faces[x][y][z] & FACE_TOP) {
					ao = getAO(Vector3<Uint16>(x, y, z), _voxels, 4);
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					x2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_TOP) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != color ||
							!(getAO(Vector3<Uint16>(x + x2, y, z), _voxels, 4) == ao))
							_done1 = true;
						if(!_done1)
							x2++;
					}
					while(!_done2) {
						if(z2 + z1 >= p_dimensions.z) {
							z2 = p_dimensions.z - z1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < x2; i++) {
							if(!(_faces[x + i][y][z + z2] & FACE_TOP) || _voxels[x + i][y][z + z2].interactionType != _interactionType || _voxels[x + i][y][z + z2].color != color ||
								!(getAO(Vector3<Uint16>(x + i, y, z + z2), _voxels, 4) == ao)) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							z2++;
					}
					for(Uint16 i = 0; i < x2; i++)
						for(Uint16 j = 0; j < z2; j++)
							_faces[x + i][y][z + j] -= FACE_TOP;
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1, y1 + 1, z1));
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1, y1 + 1, z1 + z2));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + x2, y1 + 1, z1));
						m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1)); m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));
					}
					else {
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1, y1 + 1, z1 + z2));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + x2, y1 + 1, z1));
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1, y1 + 1, z1));
						m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1 + z2)); m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1));
					}
					m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1));			m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1 + z2));		m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1 + 1, z1));		m_verticesWire.push_back(glm::vec3(x1, y1 + 1, z1));

					for(Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[4]);
				}
			}
		}
	}
	// Bottom		Y-
	for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++) {
		y = y1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++) {
			x = x1 + 1;
			for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++) {
				z = z1 + 1;
				if(_faces[x][y][z] & FACE_BOTTOM) {
					ao = getAO(Vector3<Uint16>(x, y, z), _voxels, 5);
					_interactionType = _voxels[x][y][z].interactionType;
					color = _voxels[x][y][z].color;
					x2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_BOTTOM) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != color ||
							!(getAO(Vector3<Uint16>(x + x2, y, z), _voxels, 5) == ao))
							_done1 = true;
						if(!_done1)
							x2++;
					}
					while(!_done2) {
						if(z2 + z1 >= p_dimensions.z) {
							z2 = p_dimensions.z - z1;
							_done2 = true;
							break;
						}
						for(Uint16 i = 0; i < x2; i++) {
							if(!(_faces[x + i][y][z + z2] & FACE_BOTTOM) || _voxels[x + i][y][z + z2].interactionType != _interactionType || _voxels[x + i][y][z + z2].color != color ||
								!(getAO(Vector3<Uint16>(x + i, y, z + z2), _voxels, 5) == ao)) {
								_done2 = true;
								break;
							}
						}
						if(!_done2)
							z2++;
					}
					for(Uint16 i = 0; i < x2; i++)
						for(Uint16 j = 0; j < z2; j++)
							_faces[x + i][y][z + j] -= FACE_BOTTOM;
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1, y1, z1));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1 + z2));
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1, y1, z1 + z2));
						m_verticesWire.push_back(glm::vec3(x1, y1, z1)); m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + z2));
					}
					else {
						m_colors.push_back(color * ao.y2); m_vertices.push_back(glm::vec3(x1, y1, z1 + z2));
						m_colors.push_back(color * ao.x1); m_vertices.push_back(glm::vec3(x1, y1, z1));
						m_colors.push_back(color * ao.y1); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1));
						m_colors.push_back(color * ao.x2); m_vertices.push_back(glm::vec3(x1 + x2, y1, z1 + z2));
						m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2)); m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));
					}
					m_verticesWire.push_back(glm::vec3(x1, y1, z1));			m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1));		m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1 + x2, y1, z1 + z2));	m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));
					m_verticesWire.push_back(glm::vec3(x1, y1, z1 + z2));		m_verticesWire.push_back(glm::vec3(x1, y1, z1));

					for(Uint16 k = 0; k < 4; k++) m_normals.push_back(normals[5]);
				}
			}
		}
	}
	for(x = 0; x < p_dimensions.x + 2; x++) {
		for(y = 0; y < p_dimensions.y + 2; y++) {
			delete[] _voxels[x][y];
			delete[] _faces[x][y];
		}
		delete[] _voxels[x];
		delete[] _faces[x];
	}
	delete[] _voxels;
	delete[] _faces;

	if(m_vertices.size() == 0) return;
	
	if (!m_vaoId) {
		glGenVertexArrays(1, &m_vaoIdWire);
		glBindVertexArray(m_vaoIdWire);
		glGenBuffers(1, m_vboIdWire);

		glGenVertexArrays(1, &m_vaoId);
		glBindVertexArray(m_vaoId);
		glGenBuffers(3, m_vboId);
	}
	else {
		glBindVertexArray(m_vaoId);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[0]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat) * 3, &m_vertices[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[1]);
	glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(GLfloat) * 4, &m_colors[0].r, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId[2]);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLfloat) * 3, &m_normals[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(m_vaoIdWire);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboIdWire[0]);
	glBufferData(GL_ARRAY_BUFFER, m_verticesWire.size() * sizeof(GLfloat) * 3, &m_verticesWire[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	Logger::logDiagnostic("Created mesh in " + std::to_string(glfwGetTime() - start) + " seconds");
	Logger::logDiagnostic("Mesh vertices(" + std::to_string(m_vertices.size()) + "), colors(" + std::to_string(m_colors.size()) + ")");
}

void VoxelMesh::renderMesh() {
	if (!m_vaoId) return;
	Shader::applyModel();
	Shader::setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
	glBindVertexArray(m_vaoId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(3);
	glDrawArrays(GL_QUADS, 0, GLsizei(m_vertices.size()));
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(3);
	glBindVertexArray(0);
}

void VoxelMesh::renderMeshWireframe() {
	if (!m_vaoIdWire) return;
	Shader::setBool("wireframe", true);
	Shader::applyModel();
	Shader::setColor(glm::vec4(1.f));
	glBindVertexArray(m_vaoIdWire);
	glVertexAttrib4f(1, 1.f, 0.75f, 0.f, 0.75f);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, GLsizei(m_verticesWire.size()));
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	Shader::setBool("wireframe", false);
}

void VoxelMesh::renderMeshShadow() {
	if (!m_vaoId) return;
	Shader::applyModel();
	glBindVertexArray(m_vaoId);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_QUADS, 0, GLsizei(m_vertices.size()));
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
