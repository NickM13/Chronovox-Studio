#include "engine\gfx\mesh\VoxelMesh.h"

#include "engine\utils\variable\manager\VoxelManager.h"

Vector4<GLfloat> getAO(Vector3<Uint16> p_pos, Voxel*** p_voxels, Uint8 p_side) {
	GLfloat intensity = 8;
	GLfloat idk = 8;
	GLfloat ao00, ao10, ao11, ao01;
	ao00 = ao10 = ao11 = ao01 = 0;
	switch(p_side)
	{
	case 0: // North
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType != 0);
		ao00 = ao00 - ((p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType == 0)) / idk;
		ao10 = ao10 - ((p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType == 0)) / idk;
		ao11 = ao11 - ((p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType == 0)) / idk;
		ao01 = ao01 - ((p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType == 0)) / idk;
		break;
	case 1: // South
		ao00 = GLfloat(p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType != 0);
		ao00 = ao00 - ((p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType == 0)) / idk;
		ao10 = ao10 - ((p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType == 0)) / idk;
		ao11 = ao11 - ((p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType == 0)) / idk;
		ao01 = ao01 - ((p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType == 0)) / idk;
		break;
	case 2: // West
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao00 = ao00 - ((p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao10 = ao10 - ((p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao11 = ao11 - ((p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao01 = ao01 - ((p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		break;
	case 3: // East
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType != 0);
		ao00 = ao00 - ((p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao10 = ao10 - ((p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao11 = ao11 - ((p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao01 = ao01 - ((p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType == 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		break;
	case 4: // Top
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y + 1][p_pos.z + 1].interactionType != 0);
		ao00 = ao00 - ((p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao10 = ao10 - ((p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao11 = ao11 - ((p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao01 = ao01 - ((p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		break;
	case 5: // Bottom
		ao00 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType != 0);
		ao10 = GLfloat(p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z - 1].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType != 0);
		ao11 = GLfloat(p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x + 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0);
		ao01 = GLfloat(p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z + 1].interactionType != 0) + (p_voxels[p_pos.x - 1][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z].interactionType != 0) + (p_voxels[p_pos.x][p_pos.y - 1][p_pos.z + 1].interactionType != 0);
		ao00 = ao00 - ((p_voxels[p_pos.x - 1][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao10 = ao10 - ((p_voxels[p_pos.x + 1][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z - 1].interactionType == 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao11 = ao11 - ((p_voxels[p_pos.x + 1][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x + 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		ao01 = ao01 - ((p_voxels[p_pos.x - 1][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x][p_pos.y][p_pos.z + 1].interactionType == 0) + (p_voxels[p_pos.x - 1][p_pos.y][p_pos.z].interactionType == 0)) / idk;
		break;
	}
	return (Vector4<GLfloat>(1.f - max(0, min(2.25f, ao00 + 0.25f)) / intensity, 1.f - max(0, min(2.25f, ao10 + 0.25f)) / intensity, 1.f - max(0, min(2.25f, ao11 + 0.25f)) / intensity, 1.f - max(0, min(2.25f, ao01 + 0.25f) / intensity)));
}

void VoxelMesh::createMesh(Uint16*** p_voxelIds, Sint8*** p_faceData, Vector3<Sint16> p_dimensions) {
	GLfloat _shading[6] = {
		0.95f,			// X-
		0.95f,			// X+
		0.975f,			// Z+
		0.925f,			// Z-
		1.f,			// Y+
		0.8f			// Y-
	};
	Voxel*** _voxels;
	Sint8*** _faces;
	// Outside voxels are for continuing lighting, and are not rendered
	_voxels = new Voxel**[p_dimensions.x + 2];
	_faces = new Sint8**[p_dimensions.x + 2];
	for(Uint16 x = 0; x < p_dimensions.x + 2; x++) {
		_voxels[x] = new Voxel*[p_dimensions.y + 2];
		_faces[x] = new Sint8*[p_dimensions.y + 2];
		for(Uint16 y = 0; y < p_dimensions.y + 2; y++) {
			_voxels[x][y] = new Voxel[p_dimensions.z + 2];
			_faces[x][y] = new Sint8[p_dimensions.z + 2];
			for(Uint16 z = 0; z < p_dimensions.z + 2; z++) {
				_voxels[x][y][z] = MVoxel::getInstance().getUnit(p_voxelIds[x][y][z]);
				if(_voxels[x][y][z].interactionType)
					_faces[x][y][z] = p_faceData[x][y][z];
				else
					_faces[x][y][z] = 0;
			}
		}
	}
	for(Uint16 i = 0; i < 6; i++) {
		m_colors[i].clear();
		m_vertices[i].clear();
	}
	Uint16 x, y, z;
	Uint16 x2, y2, z2;
	Vector4<GLfloat> ao;
	GLfloat p[4];
	Uint16 _interactionType;
	Uint16 _colorId;
	Color _color;
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
					_colorId = _voxels[x][y][z].color;
					y2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done1 = true;
							break;
						}
						if(!(_faces[x][y + y2][z] & FACE_NORTH) || _voxels[x][y + y2][z].interactionType != _interactionType || _voxels[x][y + y2][z].color != _colorId ||
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
							if(!(_faces[x][y + i][z + z2] & FACE_NORTH) || _voxels[x][y + i][z + z2].interactionType != _interactionType || _voxels[x][y + i][z + z2].color != _colorId ||
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
					_color = MColor::getInstance().getUnit(_colorId) * _shading[0];
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors[0].push_back(_color * ao.x1); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1, z1));
						m_colors[0].push_back(_color * ao.y2); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1, z1 + z2));
						m_colors[0].push_back(_color * ao.y1); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1 + y2, z1));
						m_colors[0].push_back(_color * ao.x2); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1 + y2, z1 + z2));
						m_colors[0].push_back(_color * ao.y1); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1 + y2, z1));
						m_colors[0].push_back(_color * ao.y2); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1, z1 + z2));
					} else {
						m_colors[0].push_back(_color * ao.y2); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1, z1 + z2));
						m_colors[0].push_back(_color * ao.x2); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1 + y2, z1 + z2));
						m_colors[0].push_back(_color * ao.x1); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1, z1));
						m_colors[0].push_back(_color * ao.y1); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1 + y2, z1));
						m_colors[0].push_back(_color * ao.x1); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1, z1));
						m_colors[0].push_back(_color * ao.x2); m_vertices[0].push_back(Vector3<GLfloat>(x1, y1 + y2, z1 + z2));
					}
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
					_colorId = _voxels[x][y][z].color;
					y2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(y2 + y1 >= p_dimensions.y) {
							y2 = p_dimensions.y - y1;
							_done1 = true;
							break;
						}
						if(!(_faces[x][y + y2][z] & FACE_SOUTH) || _voxels[x][y + y2][z].interactionType != _interactionType || _voxels[x][y + y2][z].color != _colorId ||
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
							if(!(_faces[x][y + i][z + z2] & FACE_SOUTH) || _voxels[x][y + i][z + z2].interactionType != _interactionType || _voxels[x][y + i][z + z2].color != _colorId ||
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
					_color = MColor::getInstance().getUnit(_colorId) * _shading[1];
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors[1].push_back(_color * ao.x1); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1, z1));
						m_colors[1].push_back(_color * ao.y1); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1 + y2, z1));
						m_colors[1].push_back(_color * ao.y2); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1, z1 + z2));
						m_colors[1].push_back(_color * ao.x2); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1 + y2, z1 + z2));
						m_colors[1].push_back(_color * ao.y2); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1, z1 + z2));
						m_colors[1].push_back(_color * ao.y1); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1 + y2, z1));
					} else {
						m_colors[1].push_back(_color * ao.y2); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1, z1 + z2));
						m_colors[1].push_back(_color * ao.x1); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1, z1));
						m_colors[1].push_back(_color * ao.x2); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1 + y2, z1 + z2));
						m_colors[1].push_back(_color * ao.y1); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1 + y2, z1));
						m_colors[1].push_back(_color * ao.x2); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1 + y2, z1 + z2));
						m_colors[1].push_back(_color * ao.x1); m_vertices[1].push_back(Vector3<GLfloat>(x1 + 1, y1, z1));
					}
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
					_colorId = _voxels[x][y][z].color;
					x2 = y2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_EAST) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != _colorId ||
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
							if(!(_faces[x + i][y + y2][z] & FACE_EAST) || _voxels[x + i][y + y2][z].interactionType != _interactionType || _voxels[x + i][y + y2][z].color != _colorId ||
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
					_color = MColor::getInstance().getUnit(_colorId) * _shading[2];
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors[2].push_back(_color * ao.x1); m_vertices[2].push_back(Vector3<GLfloat>(x1, y1, z1 + 1));
						m_colors[2].push_back(_color * ao.y1); m_vertices[2].push_back(Vector3<GLfloat>(x1 + x2, y1, z1 + 1));
						m_colors[2].push_back(_color * ao.y2); m_vertices[2].push_back(Vector3<GLfloat>(x1, y1 + y2, z1 + 1));
						m_colors[2].push_back(_color * ao.x2); m_vertices[2].push_back(Vector3<GLfloat>(x1 + x2, y1 + y2, z1 + 1));
						m_colors[2].push_back(_color * ao.y2); m_vertices[2].push_back(Vector3<GLfloat>(x1, y1 + y2, z1 + 1));
						m_colors[2].push_back(_color * ao.y1); m_vertices[2].push_back(Vector3<GLfloat>(x1 + x2, y1, z1 + 1));
					} else {
						m_colors[2].push_back(_color * ao.y2); m_vertices[2].push_back(Vector3<GLfloat>(x1, y1 + y2, z1 + 1));
						m_colors[2].push_back(_color * ao.x1); m_vertices[2].push_back(Vector3<GLfloat>(x1, y1, z1 + 1));
						m_colors[2].push_back(_color * ao.x2); m_vertices[2].push_back(Vector3<GLfloat>(x1 + x2, y1 + y2, z1 + 1));
						m_colors[2].push_back(_color * ao.y1); m_vertices[2].push_back(Vector3<GLfloat>(x1 + x2, y1, z1 + 1));
						m_colors[2].push_back(_color * ao.x2); m_vertices[2].push_back(Vector3<GLfloat>(x1 + x2, y1 + y2, z1 + 1));
						m_colors[2].push_back(_color * ao.x1); m_vertices[2].push_back(Vector3<GLfloat>(x1, y1, z1 + 1));
					}
				}
			}
		}
	}
	// West			Z-
	for(Uint16 z1 = 0; z1 < p_dimensions.z; z1++){
		z = z1 + 1;
		for(Uint16 x1 = 0; x1 < p_dimensions.x; x1++){
			x = x1 + 1;
			for(Uint16 y1 = 0; y1 < p_dimensions.y; y1++){
				y = y1 + 1;
				if(_faces[x][y][z] & FACE_WEST){
					ao = getAO(Vector3<Uint16>(x, y, z), _voxels, 3);
					_interactionType = _voxels[x][y][z].interactionType;
					_colorId = _voxels[x][y][z].color;
					x2 = y2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_WEST) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != _colorId ||
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
							if(!(_faces[x + i][y + y2][z] & FACE_WEST) || _voxels[x + i][y + y2][z].interactionType != _interactionType || _voxels[x + i][y + y2][z].color != _colorId ||
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
					_color = MColor::getInstance().getUnit(_colorId) * _shading[3];
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors[3].push_back(_color * ao.x1); m_vertices[3].push_back(Vector3<GLfloat>(x1, y1, z1));
						m_colors[3].push_back(_color * ao.y2); m_vertices[3].push_back(Vector3<GLfloat>(x1, y1 + y2, z1));
						m_colors[3].push_back(_color * ao.y1); m_vertices[3].push_back(Vector3<GLfloat>(x1 + x2, y1, z1));
						m_colors[3].push_back(_color * ao.x2); m_vertices[3].push_back(Vector3<GLfloat>(x1 + x2, y1 + y2, z1));
						m_colors[3].push_back(_color * ao.y1); m_vertices[3].push_back(Vector3<GLfloat>(x1 + x2, y1, z1));
						m_colors[3].push_back(_color * ao.y2); m_vertices[3].push_back(Vector3<GLfloat>(x1, y1 + y2, z1));
					} else {
						m_colors[3].push_back(_color * ao.y2); m_vertices[3].push_back(Vector3<GLfloat>(x1, y1 + y2, z1));
						m_colors[3].push_back(_color * ao.x2); m_vertices[3].push_back(Vector3<GLfloat>(x1 + x2, y1 + y2, z1));
						m_colors[3].push_back(_color * ao.x1); m_vertices[3].push_back(Vector3<GLfloat>(x1, y1, z1));
						m_colors[3].push_back(_color * ao.y1); m_vertices[3].push_back(Vector3<GLfloat>(x1 + x2, y1, z1));
						m_colors[3].push_back(_color * ao.x1); m_vertices[3].push_back(Vector3<GLfloat>(x1, y1, z1));
						m_colors[3].push_back(_color * ao.x2); m_vertices[3].push_back(Vector3<GLfloat>(x1 + x2, y1 + y2, z1));
					}
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
					_colorId = _voxels[x][y][z].color;
					x2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_TOP) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != _colorId ||
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
							if(!(_faces[x + i][y][z + z2] & FACE_TOP) || _voxels[x + i][y][z + z2].interactionType != _interactionType || _voxels[x + i][y][z + z2].color != _colorId ||
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
					_color = MColor::getInstance().getUnit(_colorId) * _shading[4];
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors[4].push_back(_color * ao.x1); m_vertices[4].push_back(Vector3<GLfloat>(x1, y1 + 1, z1));
						m_colors[4].push_back(_color * ao.y2); m_vertices[4].push_back(Vector3<GLfloat>(x1, y1 + 1, z1 + z2));
						m_colors[4].push_back(_color * ao.y1); m_vertices[4].push_back(Vector3<GLfloat>(x1 + x2, y1 + 1, z1));
						m_colors[4].push_back(_color * ao.x2); m_vertices[4].push_back(Vector3<GLfloat>(x1 + x2, y1 + 1, z1 + z2));
						m_colors[4].push_back(_color * ao.y1); m_vertices[4].push_back(Vector3<GLfloat>(x1 + x2, y1 + 1, z1));
						m_colors[4].push_back(_color * ao.y2); m_vertices[4].push_back(Vector3<GLfloat>(x1, y1 + 1, z1 + z2));
					} else {
						m_colors[4].push_back(_color * ao.y2); m_vertices[4].push_back(Vector3<GLfloat>(x1, y1 + 1, z1 + z2));
						m_colors[4].push_back(_color * ao.x2); m_vertices[4].push_back(Vector3<GLfloat>(x1 + x2, y1 + 1, z1 + z2));
						m_colors[4].push_back(_color * ao.x1); m_vertices[4].push_back(Vector3<GLfloat>(x1, y1 + 1, z1));
						m_colors[4].push_back(_color * ao.y1); m_vertices[4].push_back(Vector3<GLfloat>(x1 + x2, y1 + 1, z1));
						m_colors[4].push_back(_color * ao.x1); m_vertices[4].push_back(Vector3<GLfloat>(x1, y1 + 1, z1));
						m_colors[4].push_back(_color * ao.x2); m_vertices[4].push_back(Vector3<GLfloat>(x1 + x2, y1 + 1, z1 + z2));
					}
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
					_colorId = _voxels[x][y][z].color;
					x2 = z2 = 0;
					_done1 = _done2 = false;
					while(!_done1) {
						if(x2 + x1 >= p_dimensions.x) {
							x2 = p_dimensions.x - x1;
							_done1 = true;
							break;
						}
						if(!(_faces[x + x2][y][z] & FACE_BOTTOM) || _voxels[x + x2][y][z].interactionType != _interactionType || _voxels[x + x2][y][z].color != _colorId ||
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
							if(!(_faces[x + i][y][z + z2] & FACE_BOTTOM) || _voxels[x + i][y][z + z2].interactionType != _interactionType || _voxels[x + i][y][z + z2].color != _colorId ||
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
					_color = MColor::getInstance().getUnit(_colorId) * _shading[5];
					p[0] = ao.x1 + (ao.y1 + ao.y2) / 2;
					p[1] = ao.y1 + (ao.x1 + ao.x2) / 2;
					p[2] = ao.x2 + (ao.y1 + ao.y2) / 2;
					p[3] = ao.y2 + (ao.x1 + ao.x2) / 2;
					if((p[0] >= p[1] && p[0] >= p[3]) || (p[2] >= p[1] && p[2] >= p[3])) {
						m_colors[5].push_back(_color * ao.x1); m_vertices[5].push_back(Vector3<GLfloat>(x1, y1, z1));
						m_colors[5].push_back(_color * ao.y1); m_vertices[5].push_back(Vector3<GLfloat>(x1 + x2, y1, z1));
						m_colors[5].push_back(_color * ao.y2); m_vertices[5].push_back(Vector3<GLfloat>(x1, y1, z1 + z2));
						m_colors[5].push_back(_color * ao.x2); m_vertices[5].push_back(Vector3<GLfloat>(x1 + x2, y1, z1 + z2));
						m_colors[5].push_back(_color * ao.y2); m_vertices[5].push_back(Vector3<GLfloat>(x1, y1, z1 + z2));
						m_colors[5].push_back(_color * ao.y1); m_vertices[5].push_back(Vector3<GLfloat>(x1 + x2, y1, z1));
					} else {
						m_colors[5].push_back(_color * ao.y2); m_vertices[5].push_back(Vector3<GLfloat>(x1, y1, z1 + z2));
						m_colors[5].push_back(_color * ao.x1); m_vertices[5].push_back(Vector3<GLfloat>(x1, y1, z1));
						m_colors[5].push_back(_color * ao.x2); m_vertices[5].push_back(Vector3<GLfloat>(x1 + x2, y1, z1 + z2));
						m_colors[5].push_back(_color * ao.y1); m_vertices[5].push_back(Vector3<GLfloat>(x1 + x2, y1, z1));
						m_colors[5].push_back(_color * ao.x2); m_vertices[5].push_back(Vector3<GLfloat>(x1 + x2, y1, z1 + z2));
						m_colors[5].push_back(_color * ao.x1); m_vertices[5].push_back(Vector3<GLfloat>(x1, y1, z1));
					}
				}
			}
		}
	}
}

void VoxelMesh::renderMesh() {
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	for(Uint16 i = 0; i < 6; i++) {
		if(m_colors[i].empty()) continue;
		glColorPointer(4, GL_FLOAT, 0, &m_colors[i][0].r);
		glVertexPointer(3, GL_FLOAT, 0, &m_vertices[i][0].x);
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(m_vertices[i].size()));
	}
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
