#include "engine\editor\model\Matrix.h"
#include "engine\gfx\mesh\MMesh.h"
#include "engine\gfx\model\MModelObj.h"
#include "engine\gfx\gui\component\Component.h"
#include "engine\editor\GPreferences.h"

Matrix::Matrix(Sint16 p_id, std::string p_name, std::string p_parent, glm::vec3 p_pos, glm::ivec3 p_size) {
	m_id = p_id;
	m_name = p_name;
	m_parent = p_parent;
	m_size = p_size;
	m_pos = p_pos;
	m_visible = true;

	Uint16 noVoxel = static_cast<Uint16>(DVoxel::getVoxelId(Voxel(0, Color())));

	m_voxelData = new Uint16**[static_cast<size_t>(m_size.x) + 2];
	m_faceData = new Sint8**[static_cast<size_t>(m_size.x) + 2];
	for(Uint16 x = 0; x < static_cast<size_t>(m_size.x) + 2; x++) {
		m_voxelData[x] = new Uint16*[static_cast<size_t>(m_size.y) + 2];
		m_faceData[x] = new Sint8*[static_cast<size_t>(m_size.y) + 2];
		for(Uint16 y = 0; y < static_cast<size_t>(m_size.y) + 2; y++) {
			m_voxelData[x][y] = new Uint16[static_cast<size_t>(m_size.z) + 2];
			m_faceData[x][y] = new Sint8[static_cast<size_t>(m_size.z) + 2];
			for(Uint16 z = 0; z < static_cast<size_t>(m_size.z) + 2; z++) {
				m_voxelData[x][y][z] = noVoxel;
				m_faceData[x][y][z] = FACE_ALL;
			}
		}
	}

	m_mesh = new VoxelMesh();
}
Matrix::Matrix(Matrix& m) {
	m_id = m.getId();
	m_name = m.getName();
	m_parent = m.getParent();
	m_size = m.getSize();
	m_pos = m.getPos();
	m_visible = true;

	Uint16 noVoxel = static_cast<Uint16>(DVoxel::getVoxelId(Voxel(0, Color())));

	m_voxelData = new Uint16**[static_cast<size_t>(m_size.x) + 2];
	m_faceData = new Sint8**[static_cast<size_t>(m_size.x) + 2];
	for(Uint16 x = 0; x < static_cast<size_t>(m_size.x) + 2; x++) {
		m_voxelData[x] = new Uint16*[static_cast<size_t>(m_size.y) + 2];
		m_faceData[x] = new Sint8*[static_cast<size_t>(m_size.y) + 2];
		for(Uint16 y = 0; y < static_cast<size_t>(m_size.y) + 2; y++) {
			m_voxelData[x][y] = new Uint16[static_cast<size_t>(m_size.z) + 2];
			m_faceData[x][y] = new Sint8[static_cast<size_t>(m_size.z) + 2];
			for(Uint16 z = 0; z < static_cast<size_t>(m_size.z) + 2; z++) {
				m_voxelData[x][y][z] = noVoxel;
				m_faceData[x][y][z] = FACE_ALL;
			}
		}
	}

	for(Uint16 x = 0; x < m_size.x; x++)
		for(Uint16 y = 0; y < m_size.y; y++)
			for(Uint16 z = 0; z < m_size.z; z++)
				setVoxel({x, y, z}, m.getVoxel({x, y, z}));

	m_mesh = new VoxelMesh();
}
Matrix::~Matrix() {
	for(Uint16 x = 0; x < static_cast<size_t>(m_size.x) + 2; x++) {
		for(Uint16 y = 0; y < static_cast<size_t>(m_size.y) + 2; y++) {
			delete[] m_voxelData[x][y];
			delete[] m_faceData[x][y];
		}
		delete[] m_voxelData[x];
		delete[] m_faceData[x];
	}
	delete[] m_voxelData;
	delete[] m_faceData;

	if (m_mesh) {
		delete m_mesh;
		m_mesh = 0;
	}
}
void Matrix::operator=(Matrix& m) {
	for(Uint16 x = 0; x < static_cast<size_t>(m_size.x) + 2; x++) {
		for(Uint16 y = 0; y < static_cast<size_t>(m_size.y) + 2; y++) {
			delete[] m_voxelData[x][y];
			delete[] m_faceData[x][y];
		}
		delete[] m_voxelData[x];
		delete[] m_faceData[x];
	}
	delete[] m_voxelData;
	delete[] m_faceData;

	if (m_mesh) delete m_mesh;

	m_name = m.getName();
	m_size = m.getSize();
	m_pos = m.getPos();
	m_visible = true;

	Uint16 noVoxel = static_cast<Uint16>(DVoxel::getVoxelId(Voxel(0, Color())));

	m_voxelData = new Uint16**[static_cast<size_t>(m_size.x) + 2];
	m_faceData = new Sint8**[static_cast<size_t>(m_size.x) + 2];
	for(Uint16 x = 0; x < static_cast<size_t>(m_size.x) + 2; x++) {
		m_voxelData[x] = new Uint16*[static_cast<size_t>(m_size.y) + 2];
		m_faceData[x] = new Sint8*[static_cast<size_t>(m_size.y) + 2];
		for(Uint16 y = 0; y < static_cast<size_t>(m_size.y) + 2; y++) {
			m_voxelData[x][y] = new Uint16[static_cast<size_t>(m_size.z) + 2];
			m_faceData[x][y] = new Sint8[static_cast<size_t>(m_size.z) + 2];
			for(Uint16 z = 0; z < static_cast<size_t>(m_size.z) + 2; z++) {
				m_voxelData[x][y][z] = noVoxel;
				m_faceData[x][y][z] = FACE_ALL;
			}
		}
	}

	for(Uint16 x = 0; x < m_size.x; x++)
		for(Uint16 y = 0; y < m_size.y; y++)
			for(Uint16 z = 0; z < m_size.z; z++)
				setVoxel({x, y, z}, m.getVoxel({x, y, z}));

	m_mesh = new VoxelMesh();
}

void Matrix::merge(Matrix& m) {
	glm::vec3 pos1 = (glm::min(m_pos, m.getPos()));
	glm::vec3 pos2 = (glm::max(m_pos + glm::vec3(m_size), m.getPos() + glm::vec3(m.getSize())));
	glm::vec3 size = pos2 - pos1;

	setSize(size);
	shiftVoxels(m_pos - pos1);
	setPosition(pos1);
	
	for (Sint32 x = 0; x < m.getSize().x; x++) {
		for (Sint32 y = 0; y < m.getSize().y; y++) {
			for (Sint32 z = 0; z < m.getSize().z; z++) {
				if (m.getVoxel(glm::ivec3(x, y, z)).interactionType != 0)
					setVoxel(glm::ivec3(m.getPos() - pos1) + glm::ivec3(x, y, z), m.getVoxel(glm::ivec3(x, y, z)));
			}
		}
	}
}

void Matrix::setSize(glm::ivec3 p_size) {
	p_size = glm::ivec3(std::fmaxf(p_size.x, 1), std::fmaxf(p_size.y, 1), std::fmaxf(p_size.z, 1));
	glm::ivec3 _size = glm::ivec3(std::fminf(p_size.x, m_size.x), std::fminf(p_size.y, m_size.y), std::fminf(p_size.z, m_size.z));

	Voxel*** _voxelData = new Voxel**[_size.x];
	for(Uint16 x = 0; x < _size.x; x++) {
		_voxelData[x] = new Voxel*[_size.y];
		for(Uint16 y = 0; y < _size.y; y++) {
			_voxelData[x][y] = new Voxel[_size.z];
			for(Uint16 z = 0; z < _size.z; z++) {
				_voxelData[x][y][z] = getVoxel({x, y, z});
			}
		}
	}

	for(Uint16 x = 0; x < m_size.x + 2; x++) {
		for(Uint16 y = 0; y < m_size.y + 2; y++) {
			delete[] m_voxelData[x][y];
			delete[] m_faceData[x][y];
		}
		delete[] m_voxelData[x];
		delete[] m_faceData[x];
	}
	delete[] m_voxelData;
	delete[] m_faceData;

	m_size = p_size;

	Uint16 noVoxel = static_cast<Uint16>(DVoxel::getVoxelId(Voxel(0, Color())));

	m_voxelData = new Uint16**[static_cast<size_t>(p_size.x) + 2];
	m_faceData = new Sint8**[static_cast<size_t>(p_size.x) + 2];
	for(Uint16 x = 0; x < static_cast<size_t>(p_size.x) + 2; x++) {
		m_voxelData[x] = new Uint16*[static_cast<size_t>(p_size.y) + 2];
		m_faceData[x] = new Sint8*[static_cast<size_t>(p_size.y) + 2];
		for(Uint16 y = 0; y < static_cast<size_t>(p_size.y) + 2; y++) {
			m_voxelData[x][y] = new Uint16[static_cast<size_t>(p_size.z) + 2];
			m_faceData[x][y] = new Sint8[static_cast<size_t>(p_size.z) + 2];
			for(Uint16 z = 0; z < static_cast<size_t>(p_size.z) + 2; z++) {
				m_voxelData[x][y][z] = noVoxel;
				m_faceData[x][y][z] = FACE_ALL;
			}
		}
	}

	for(Uint16 x = 0; x < p_size.x + 2; x++)
		for(Uint16 y = 0; y < p_size.y + 2; y++)
			for(Uint16 z = 0; z < p_size.z + 2; z++)
				if(x != 0 && x <= _size.x && y != 0 && y <= _size.y && z != 0 && z <= _size.z)
					setVoxel({x - 1, y - 1, z - 1}, _voxelData[x - 1][y - 1][z - 1]);

}
void Matrix::setPosition(glm::vec3 p_pos) {
	m_pos = p_pos;
	m_pos = glm::round(m_pos * glm::vec3(10)) / glm::vec3(10);
}
void Matrix::addPosition(glm::vec3 p_pos) {
	m_pos = m_pos + p_pos;
	m_pos = glm::round(m_pos * glm::vec3(10)) / glm::vec3(10);
}

bool Matrix::setVoxel(glm::ivec3 p_pos, Voxel p_voxel) {
	if(p_pos.x < 0 || p_pos.y < 0 || p_pos.z < 0 || p_pos.x >= m_size.x || p_pos.y >= m_size.y || p_pos.z >= m_size.z) return false;
	if (m_name == "Body" && p_pos.x == 1 && p_pos.y == 0 && p_pos.z == 2) {
		int a = 0;
	}
	glm::ivec3 _pos = p_pos + 1;
	Uint16 voxId = static_cast<Uint16>(DVoxel::getVoxelId(p_voxel));
	if(m_voxelData[_pos.x][_pos.y][_pos.z] == voxId) return false;
	if(p_voxel.interactionType != DVoxel::getVoxel(m_voxelData[_pos.x][_pos.y][_pos.z]).interactionType) {
		// Set face data of surrounding voxels
		if(p_pos.x < m_size.x) {
			if (p_voxel.interactionType)	m_faceData[_pos.x + 1][_pos.y][_pos.z] &= ~FACE_NORTH;
			else							m_faceData[_pos.x + 1][_pos.y][_pos.z] |= FACE_NORTH;
			/*if(p_voxel.interactionType != 0 && (m_faceData[_pos.x + 1][_pos.y][_pos.z] & FACE_NORTH))
				m_faceData[_pos.x + 1][_pos.y][_pos.z] -= FACE_NORTH;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x + 1][_pos.y][_pos.z] & FACE_NORTH))
				m_faceData[_pos.x + 1][_pos.y][_pos.z] += FACE_NORTH;*/
		}

		if(p_pos.x >= 0) {
			if (p_voxel.interactionType)	m_faceData[_pos.x - 1][_pos.y][_pos.z] &= ~FACE_SOUTH;
			else							m_faceData[_pos.x - 1][_pos.y][_pos.z] |= FACE_SOUTH;
			/*if(p_voxel.interactionType != 0 && (m_faceData[_pos.x - 1][_pos.y][_pos.z] & FACE_SOUTH))
				m_faceData[_pos.x - 1][_pos.y][_pos.z] -= FACE_SOUTH;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x - 1][_pos.y][_pos.z] & FACE_SOUTH))
				m_faceData[_pos.x - 1][_pos.y][_pos.z] += FACE_SOUTH;*/
		}

		if(p_pos.z < m_size.z) {
			if (p_voxel.interactionType)	m_faceData[_pos.x][_pos.y][_pos.z + 1] &= ~FACE_WEST;
			else							m_faceData[_pos.x][_pos.y][_pos.z + 1] |= FACE_WEST;
			/*if(p_voxel.interactionType != 0 && (m_faceData[_pos.x][_pos.y][_pos.z + 1] & FACE_WEST))
				m_faceData[_pos.x][_pos.y][_pos.z + 1] -= FACE_WEST;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x][_pos.y][_pos.z + 1] & FACE_WEST))
				m_faceData[_pos.x][_pos.y][_pos.z + 1] += FACE_WEST;*/
		}

		if(p_pos.z >= 0) {
			if (p_voxel.interactionType)	m_faceData[_pos.x][_pos.y][_pos.z - 1] &= ~FACE_EAST;
			else							m_faceData[_pos.x][_pos.y][_pos.z - 1] |= FACE_EAST;
			/*if(p_voxel.interactionType != 0 && (m_faceData[_pos.x][_pos.y][_pos.z - 1] & FACE_EAST))
				m_faceData[_pos.x][_pos.y][_pos.z - 1] -= FACE_EAST;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x][_pos.y][_pos.z - 1] & FACE_EAST))
				m_faceData[_pos.x][_pos.y][_pos.z - 1] += FACE_EAST;*/
		}

		if(p_pos.y < m_size.y) {
			if (p_voxel.interactionType)	m_faceData[_pos.x][_pos.y + 1][_pos.z] &= ~FACE_BOTTOM;
			else							m_faceData[_pos.x][_pos.y + 1][_pos.z] |= FACE_BOTTOM;
			/*if(p_voxel.interactionType != 0 && (m_faceData[_pos.x][_pos.y + 1][_pos.z] & FACE_BOTTOM))
				m_faceData[_pos.x][_pos.y + 1][_pos.z] -= FACE_BOTTOM;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x][_pos.y + 1][_pos.z] & FACE_BOTTOM))
				m_faceData[_pos.x][_pos.y + 1][_pos.z] += FACE_BOTTOM;*/
		}

		if(p_pos.y >= 0) {
			if (p_voxel.interactionType)	m_faceData[_pos.x][_pos.y - 1][_pos.z] &= ~FACE_TOP;
			else							m_faceData[_pos.x][_pos.y - 1][_pos.z] |= FACE_TOP;
			/*if(p_voxel.interactionType != 0 && (m_faceData[_pos.x][_pos.y - 1][_pos.z] & FACE_TOP))
				m_faceData[_pos.x][_pos.y - 1][_pos.z] -= FACE_TOP;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x][_pos.y - 1][_pos.z] & FACE_TOP))
				m_faceData[_pos.x][_pos.y - 1][_pos.z] += FACE_TOP;*/
		}
	}
	m_voxelData[_pos.x][_pos.y][_pos.z] = voxId;
	m_needsRasterize = true;
	return true;
}
Voxel Matrix::getVoxel(glm::ivec3 p_pos) {
	return DVoxel::getVoxel(m_voxelData[p_pos.x + 1][p_pos.y + 1][p_pos.z + 1]);
}
Uint16*** Matrix::getVoxelData() {
	return m_voxelData;
}
Uint16 Matrix::getVoxelId(glm::ivec3 p_pos) {
	return m_voxelData[p_pos.x + 1][p_pos.y + 1][p_pos.z + 1];
}
void Matrix::shiftVoxels(glm::ivec3 d) {
	Voxel*** _voxelData = new Voxel**[m_size.x];
	for(Sint32 x = 0; x < m_size.x; x++) {
		_voxelData[x] = new Voxel*[m_size.y];
		for(Sint32 y = 0; y < m_size.y; y++) {
			_voxelData[x][y] = new Voxel[m_size.z];
			for(Sint32 z = 0; z < m_size.z; z++) {
				_voxelData[x][y][z] = getVoxel({x, y, z});
			}
		}
	}

	glm::ivec3 _offset;
	for(Sint32 x = 0; x < m_size.x; x++)
		for(Sint32 y = 0; y < m_size.y; y++)
			for(Sint32 z = 0; z < m_size.z; z++) {
				_offset = glm::ivec3(fmod(x + d.x, m_size.x), fmod(y + d.y, m_size.y), fmod(z + d.z, m_size.z));
				if(_offset.x < 0) _offset.x += m_size.x;
				if(_offset.y < 0) _offset.y += m_size.y;
				if(_offset.z < 0) _offset.z += m_size.z;
				setVoxel(_offset, _voxelData[x][y][z]);
			}


}
void Matrix::flip(Sint8 p_axes, glm::vec3 p_focus) {
	Voxel ***_data = new Voxel**[m_size.x];
	glm::vec3 center = p_focus - getCenter();
	for(Uint16 x = 0; x < m_size.x; x++) {
		_data[x] = new Voxel*[m_size.y];
		for(Uint16 y = 0; y < m_size.y; y++) {
			_data[x][y] = new Voxel[m_size.z];
		}
	}
	if(p_axes & AXIS_X) {
		for(Uint16 x = 0; x < m_size.x; x++)
			for(Uint16 y = 0; y < m_size.y; y++)
				for(Uint16 z = 0; z < m_size.z; z++)
					_data[x][y][z] = getVoxel({x, y, z});

		for(Uint16 x = 0; x < m_size.x; x++)
			for(Uint16 y = 0; y < m_size.y; y++)
				for(Uint16 z = 0; z < m_size.z; z++)
					setVoxel({x, y, z}, _data[m_size.x - x - 1][y][z]);

		addPosition(glm::vec3(center.x * 2, 0, 0));
	}
	if(p_axes & AXIS_Y) {
		for(Uint16 x = 0; x < m_size.x; x++)
			for(Uint16 y = 0; y < m_size.y; y++)
				for(Uint16 z = 0; z < m_size.z; z++)
					_data[x][y][z] = getVoxel({x, y, z});

		for(Uint16 x = 0; x < m_size.x; x++)
			for(Uint16 y = 0; y < m_size.y; y++)
				for(Uint16 z = 0; z < m_size.z; z++)
					setVoxel({x, y, z}, _data[x][m_size.y - y - 1][z]);

		addPosition(glm::vec3(0, center.y * 2, 0));
	}
	if(p_axes & AXIS_Z) {
		for(Uint16 x = 0; x < m_size.x; x++)
			for(Uint16 y = 0; y < m_size.y; y++)
				for(Uint16 z = 0; z < m_size.z; z++)
					_data[x][y][z] = getVoxel({x, y, z});

		for(Uint16 x = 0; x < m_size.x; x++)
			for(Uint16 y = 0; y < m_size.y; y++)
				for(Uint16 z = 0; z < m_size.z; z++)
					setVoxel({x, y, z}, _data[x][y][m_size.z - z - 1]);

		addPosition(glm::vec3(0, 0, center.z * 2));
	}

	for(Uint16 x = 0; x < m_size.x; x++) {
		for(Uint16 y = 0; y < m_size.y; y++) {
			delete[] _data[x][y];
		}
		delete[] _data[x];
	}
	delete[] _data;
}
void Matrix::rotate(Sint8 p_axes, glm::vec3 p_focus) {
	glm::ivec3 _size;
	Voxel ***_data = new Voxel**[m_size.x];
	glm::vec3 center = p_focus - getCenter();
	for(Uint16 x = 0; x < m_size.x; x++) {
		_data[x] = new Voxel*[m_size.y];
		for(Uint16 y = 0; y < m_size.y; y++) {
			_data[x][y] = new Voxel[m_size.z];
		}
	}
	if(p_axes & AXIS_X) {
		_size = m_size;
		for(Uint16 x = 0; x < _size.x; x++)
			for(Uint16 y = 0; y < _size.y; y++)
				for(Uint16 z = 0; z < _size.z; z++)
					_data[x][y][z] = getVoxel({x, y, z});

		setSize({m_size.x, m_size.z, m_size.y});
		addPosition(glm::vec3(0, (m_size.z - m_size.y) / 2.f, (m_size.y - m_size.z) / 2.f));

		for(Uint16 x = 0; x < _size.x; x++)
			for(Uint16 y = 0; y < _size.y; y++)
				for(Uint16 z = 0; z < _size.z; z++)
					setVoxel({x, z, y}, _data[x][_size.y - y - 1][z]);

		for(Uint16 x = 0; x < _size.x; x++) {
			for(Uint16 y = 0; y < _size.y; y++) {
				delete[] _data[x][y];
			}
			delete[] _data[x];
		}
		delete[] _data;

		addPosition(glm::vec3(0, center.y - center.z, center.z + center.y));
	}
	else if(p_axes & AXIS_Y) {
		_size = m_size;
		for(Uint16 x = 0; x < _size.x; x++)
			for(Uint16 y = 0; y < _size.y; y++)
				for(Uint16 z = 0; z < _size.z; z++)
					_data[x][y][z] = getVoxel({x, y, z});

		setSize({m_size.z, m_size.y, m_size.x});
		addPosition(glm::vec3((m_size.z - m_size.x) / 2.f, 0, (m_size.x - m_size.z) / 2.f));

		for(Uint16 x = 0; x < _size.x; x++)
			for(Uint16 y = 0; y < _size.y; y++)
				for(Uint16 z = 0; z < _size.z; z++)
					setVoxel({z, y, x}, _data[x][y][_size.z - z - 1]);

		for(Uint16 x = 0; x < _size.x; x++) {
			for(Uint16 y = 0; y < _size.y; y++) {
				delete[] _data[x][y];
			}
			delete[] _data[x];
		}
		delete[] _data;

		addPosition(glm::vec3(center.x + center.z, 0, center.z - center.x));
	}
	else if(p_axes & AXIS_Z) {
		_size = m_size;
		for(Uint16 x = 0; x < _size.x; x++)
			for(Uint16 y = 0; y < _size.y; y++)
				for(Uint16 z = 0; z < _size.z; z++)
					_data[x][y][z] = getVoxel({x, y, z});

		setSize({m_size.y, m_size.x, m_size.z});
		addPosition(glm::vec3((m_size.y - m_size.x) / 2.f, (m_size.x - m_size.y) / 2.f, 0));

		for(Uint16 x = 0; x < _size.x; x++)
			for(Uint16 y = 0; y < _size.y; y++)
				for(Uint16 z = 0; z < _size.z; z++)
					setVoxel({y, x, z}, _data[_size.x - x - 1][y][z]);

		for(Uint16 x = 0; x < _size.x; x++) {
			for(Uint16 y = 0; y < _size.y; y++) {
				delete[] _data[x][y];
			}
			delete[] _data[x];
		}
		delete[] _data;

		addPosition(glm::vec3(center.x - center.y, center.y + center.x, 0));
	}
}
void Matrix::scale(glm::vec3 p_scalar, glm::vec3 p_focus) {
	// Initialize variables
	glm::ivec3 _size = m_size;
	Voxel*** _data;
	glm::vec3 center;

	// Set data to current matrix voxels
	// to be scaled to expanded or
	// contrasted size
	_data = new Voxel * *[_size.x];
	center = p_focus - getCenter();
	for (Sint32 x = 0; x < _size.x; x++) {
		_data[x] = new Voxel * [_size.y];
		for (Sint32 y = 0; y < _size.y; y++) {
			_data[x][y] = new Voxel[_size.z];
			for (Sint32 z = 0; z < _size.z; z++) {
				_data[x][y][z] = getVoxel({ x, y, z });
			}
		}
	}

	// Set size to scaled size and
	// position to maintain center of voxel
	setSize(glm::vec3(m_size) * p_scalar);
	addPosition((_size - m_size) / 2);
	
	// Loop through new scale size and
	// set those voxels to corresponding
	// voxels from pre-scale matrix
	for (Sint32 x = 0; x < m_size.x; x++)
		for (Sint32 y = 0; y < m_size.y; y++)
			for (Sint32 z = 0; z < m_size.z; z++)
				setVoxel({ x, y, z },
					_data	[static_cast<Sint32>(std::floorf(x / p_scalar.x))]
							[static_cast<Sint32>(std::floorf(y / p_scalar.y))]
							[static_cast<Sint32>(std::floorf(z / p_scalar.z))]);

	// Cleanup
	for (Uint16 x = 0; x < _size.x; x++) {
		for (Uint16 y = 0; y < _size.y; y++) {
			delete[] _data[x][y];
		}
		delete[] _data[x];
	}
	delete[] _data;

	// Add position to maintain focus'd center
	addPosition(glm::vec3(center - center * p_scalar));
}
void Matrix::autoResize(glm::ivec3* inner, glm::ivec3* ext) {
	if (GPreferences::getAutoResize() == GPreferences::AutoResize::OFF) return;
	glm::ivec3 offset = *ext - *inner;

	// Check that the offset is one voxel from inner
	if ((offset.y == 0 && offset.z == 0)) {
		if (offset.x == -1) {
			if (inner->x == 0) {
				setSize(m_size + glm::ivec3(1, 0, 0));
				shiftVoxels(glm::ivec3(1, 0, 0));
				addPosition(glm::ivec3(-1, 0, 0));
				inner->x++;
				ext->x++;
			}
		}
		else if (offset.x == 1) {
			if (inner->x == m_size.x - 1) {
				setSize(m_size + glm::ivec3(1, 0, 0));
			}
		}
	}
	else if (offset.z == 0 && offset.x == 0) {
		if (offset.y == -1) {
			if (inner->y == 0) {
				setSize(m_size + glm::ivec3(0, 1, 0));
				shiftVoxels(glm::ivec3(0, 1, 0));
				addPosition(glm::ivec3(0, -1, 0));
				inner->y++;
				ext->y++;
			}
		}
		else if (offset.y == 1) {
			if (inner->y == m_size.y - 1) {
				setSize(m_size + glm::ivec3(0, 1, 0));
			}
		}
	}
	else if (offset.x == 0 && offset.y == 0) {
		if (offset.z == -1) {
			if (inner->z == 0) {
				setSize(m_size + glm::ivec3(0, 0, 1));
				shiftVoxels(glm::ivec3(0, 0, 1));
				addPosition(glm::ivec3(0, 0, -1));
				inner->z++;
				ext->z++;
			}
		}
		else if (offset.z == 1) {
			if (inner->z == m_size.z - 1) {
				setSize(m_size + glm::ivec3(0, 0, 1));
			}
		}
	}
}

bool Matrix::containsPoint(glm::ivec3 p_point) {
	return (p_point.x >= 0 && p_point.x < m_size.x
		&& p_point.y >= 0 && p_point.y < m_size.y
		&& p_point.z >= 0 && p_point.z < m_size.z);
}

void Matrix::update() {
	if(m_needsRasterize) {
		rasterize();
		m_needsRasterize = false;
	}
}
void Matrix::renderMatrix() {
	Shader::pushMatrixModel();
	Shader::transformModel(glm::translate(glm::mat4(), glm::vec3(m_pos.x, m_pos.y, m_pos.z)));
	m_mesh->renderMesh();
	Shader::popMatrixModel();
}
void Matrix::renderMatrixWireframe() {
	Shader::pushMatrixModel();
	Shader::transformModel(glm::translate(glm::mat4(), glm::vec3(m_pos.x, m_pos.y, m_pos.z)));
	m_mesh->renderMeshWireframe();
	Shader::popMatrixModel();
}
void Matrix::renderShadow() {
	Shader::pushMatrixModel();
	Shader::transformModel(glm::translate(glm::mat4(), glm::vec3(m_pos.x, m_pos.y, m_pos.z)));
	m_mesh->renderMeshShadow();
	Shader::popMatrixModel();
}
void Matrix::renderOutline(OutlineType p_outline) {
	GLfloat w1 = 0.05f, w2 = w1 / 2.5f;
	glm::vec3 s = glm::vec3(m_size);
	Color outline;
	switch(p_outline) {
	case OutlineType::NONE: return;
	case OutlineType::OUTLINE:	outline = Component::getElementColor("matrixOutline"); break;
	case OutlineType::HOVERED:	outline = Component::getElementColor("matrixHovered"); break;
	case OutlineType::SELECTED:	outline = Component::getElementColor("matrixSelected"); break;
	}
	Shader::setColor(glm::vec4(1.f));
	MMesh::renderBoxOutline(m_pos, s, glm::vec4(outline.r, outline.g, outline.b, outline.a));
}

VoxelMesh* Matrix::getMeshSimple() {
	VoxelMesh* mesh = new VoxelMesh();
	mesh->createMesh(m_voxelData, m_faceData, Vector3<Sint32>(m_size.x, m_size.y, m_size.z));
	return mesh;
}

void Matrix::rasterize() {
	//m_mesh->createMesh(m_voxelData, m_faceData, Vector3<Sint32>(m_size.x, m_size.y, m_size.z));
	m_mesh->createMeshAO(m_voxelData, m_faceData, Vector3<Sint32>(m_size.x, m_size.y, m_size.z));
}
