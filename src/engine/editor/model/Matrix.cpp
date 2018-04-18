#include "engine\editor\model\Matrix.h"
#include "engine\utils\variable\manager\ColorManager.h"
#include "engine\gfx\mesh\MMesh.h"

Matrix::Matrix(Sint16 p_id, std::string p_name, std::string p_parent, Vector3<GLfloat> p_pos, Vector3<Sint16> p_size) {
	m_id = p_id;
	m_name = p_name;
	m_parent = p_parent;
	m_size = p_size;
	m_pos = p_pos;

	Uint16 noVoxel = MVoxel::getInstance().getUnitID(Voxel(0, MColor::getInstance().getUnitID(Color())));

	m_voxelData = new Uint16**[m_size.x + 2];
	m_faceData = new Sint8**[m_size.x + 2];
	for(Uint16 x = 0; x < m_size.x + 2; x++) {
		m_voxelData[x] = new Uint16*[m_size.y + 2];
		m_faceData[x] = new Sint8*[m_size.y + 2];
		for(Uint16 y = 0; y < m_size.y + 2; y++) {
			m_voxelData[x][y] = new Uint16[m_size.z + 2];
			m_faceData[x][y] = new Sint8[m_size.z + 2];
			for(Uint16 z = 0; z < m_size.z + 2; z++) {
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

	Uint16 noVoxel = MVoxel::getInstance().getUnitID(Voxel(0, MColor::getInstance().getUnitID(Color())));

	m_voxelData = new Uint16**[m_size.x + 2];
	m_faceData = new Sint8**[m_size.x + 2];
	for(Uint16 x = 0; x < m_size.x + 2; x++) {
		m_voxelData[x] = new Uint16*[m_size.y + 2];
		m_faceData[x] = new Sint8*[m_size.y + 2];
		for(Uint16 y = 0; y < m_size.y + 2; y++) {
			m_voxelData[x][y] = new Uint16[m_size.z + 2];
			m_faceData[x][y] = new Sint8[m_size.z + 2];
			for(Uint16 z = 0; z < m_size.z + 2; z++) {
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

	delete m_mesh;
}
void Matrix::operator=(Matrix& m) {
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

	delete m_mesh;



	m_name = m.getName();
	m_size = m.getSize();
	m_pos = m.getPos();

	Uint16 noVoxel = MVoxel::getInstance().getUnitID(Voxel(0, MColor::getInstance().getUnitID(Color())));

	m_voxelData = new Uint16**[m_size.x + 2];
	m_faceData = new Sint8**[m_size.x + 2];
	for(Uint16 x = 0; x < m_size.x + 2; x++) {
		m_voxelData[x] = new Uint16*[m_size.y + 2];
		m_faceData[x] = new Sint8*[m_size.y + 2];
		for(Uint16 y = 0; y < m_size.y + 2; y++) {
			m_voxelData[x][y] = new Uint16[m_size.z + 2];
			m_faceData[x][y] = new Sint8[m_size.z + 2];
			for(Uint16 z = 0; z < m_size.z + 2; z++) {
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

void Matrix::setSize(Vector3<Sint32> p_size) {
	p_size = Vector3<Sint32>(max(p_size.x, 1), max(p_size.y, 1), max(p_size.z, 1));
	Vector3<Sint32> _size = Vector3<Sint32>(min(p_size.x, m_size.x), min(p_size.y, m_size.y), min(p_size.z, m_size.z));

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

	Uint16 noVoxel = MVoxel::getInstance().getUnitID(Voxel(0, MColor::getInstance().getUnitID(Color())));

	m_voxelData = new Uint16**[p_size.x + 2];
	m_faceData = new Sint8**[p_size.x + 2];
	for(Uint16 x = 0; x < p_size.x + 2; x++) {
		m_voxelData[x] = new Uint16*[p_size.y + 2];
		m_faceData[x] = new Sint8*[p_size.y + 2];
		for(Uint16 y = 0; y < p_size.y + 2; y++) {
			m_voxelData[x][y] = new Uint16[p_size.z + 2];
			m_faceData[x][y] = new Sint8[p_size.z + 2];
			for(Uint16 z = 0; z < p_size.z + 2; z++) {
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
void Matrix::setPosition(Vector3<GLfloat> p_pos) {
	m_pos = p_pos;
	m_pos = (m_pos * 2).round() / 2;
}
void Matrix::addPosition(Vector3<GLfloat> p_pos) {
	m_pos = m_pos + p_pos;
	m_pos = (m_pos * 2).round() / 2;
}

bool Matrix::setVoxel(Vector3<Sint32> p_pos, Voxel p_voxel) {
	if(p_pos.x < 0 || p_pos.y < 0 || p_pos.z < 0 || p_pos.x >= m_size.x || p_pos.y >= m_size.y || p_pos.z >= m_size.z) return false;
	Vector3<Sint32> _pos = p_pos + 1;
	if(m_voxelData[_pos.x][_pos.y][_pos.z] == MVoxel::getInstance().getUnitID(p_voxel)) return false;
	if(p_voxel.interactionType != MVoxel::getInstance().getUnit(m_voxelData[_pos.x][_pos.y][_pos.z]).interactionType) {
		// Set face data of surrounding voxels
		if(p_pos.x < m_size.x) {
			if(p_voxel.interactionType != 0 && (m_faceData[_pos.x + 1][_pos.y][_pos.z] & FACE_NORTH))
				m_faceData[_pos.x + 1][_pos.y][_pos.z] -= FACE_NORTH;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x + 1][_pos.y][_pos.z] & FACE_NORTH))
				m_faceData[_pos.x + 1][_pos.y][_pos.z] += FACE_NORTH;
		}

		if(p_pos.x >= 0) {
			if(p_voxel.interactionType != 0 && (m_faceData[_pos.x - 1][_pos.y][_pos.z] & FACE_SOUTH))
				m_faceData[_pos.x - 1][_pos.y][_pos.z] -= FACE_SOUTH;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x - 1][_pos.y][_pos.z] & FACE_SOUTH))
				m_faceData[_pos.x - 1][_pos.y][_pos.z] += FACE_SOUTH;
		}

		if(p_pos.z < m_size.z) {
			if(p_voxel.interactionType != 0 && (m_faceData[_pos.x][_pos.y][_pos.z + 1] & FACE_WEST))
				m_faceData[_pos.x][_pos.y][_pos.z + 1] -= FACE_WEST;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x][_pos.y][_pos.z + 1] & FACE_WEST))
				m_faceData[_pos.x][_pos.y][_pos.z + 1] += FACE_WEST;
		}

		if(p_pos.z >= 0) {
			if(p_voxel.interactionType != 0 && (m_faceData[_pos.x][_pos.y][_pos.z - 1] & FACE_EAST))
				m_faceData[_pos.x][_pos.y][_pos.z - 1] -= FACE_EAST;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x][_pos.y][_pos.z - 1] & FACE_EAST))
				m_faceData[_pos.x][_pos.y][_pos.z - 1] += FACE_EAST;
		}

		if(p_pos.y < m_size.y) {
			if(p_voxel.interactionType != 0 && (m_faceData[_pos.x][_pos.y + 1][_pos.z] & FACE_BOTTOM))
				m_faceData[_pos.x][_pos.y + 1][_pos.z] -= FACE_BOTTOM;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x][_pos.y + 1][_pos.z] & FACE_BOTTOM))
				m_faceData[_pos.x][_pos.y + 1][_pos.z] += FACE_BOTTOM;
		}

		if(p_pos.y >= 0) {
			if(p_voxel.interactionType != 0 && (m_faceData[_pos.x][_pos.y - 1][_pos.z] & FACE_TOP))
				m_faceData[_pos.x][_pos.y - 1][_pos.z] -= FACE_TOP;
			else if(p_voxel.interactionType == 0 && !(m_faceData[_pos.x][_pos.y - 1][_pos.z] & FACE_TOP))
				m_faceData[_pos.x][_pos.y - 1][_pos.z] += FACE_TOP;
		}
	}
	m_voxelData[_pos.x][_pos.y][_pos.z] = MVoxel::getInstance().getUnitID(p_voxel);
	m_needsRasterize = true;
	return true;
}
Voxel Matrix::getVoxel(Vector3<Sint32> p_pos) {
	p_pos = p_pos + 1;
	return MVoxel::getInstance().getUnit(m_voxelData[p_pos.x][p_pos.y][p_pos.z]);
}
Uint16 Matrix::getVoxelId(Vector3<Sint32> p_pos) {
	p_pos = p_pos + 1;
	return m_voxelData[p_pos.x][p_pos.y][p_pos.z];
}
void Matrix::shiftVoxels(Vector3<Sint32> d) {
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

	Vector3<Sint32> _offset;
	for(Sint32 x = 0; x < m_size.x; x++)
		for(Sint32 y = 0; y < m_size.y; y++)
			for(Sint32 z = 0; z < m_size.z; z++) {
				_offset = Vector3<Sint32>(fmod(x + d.x, m_size.x), fmod(y + d.y, m_size.y), fmod(z + d.z, m_size.z));
				if(_offset.x < 0) _offset.x += m_size.x;
				if(_offset.y < 0) _offset.y += m_size.y;
				if(_offset.z < 0) _offset.z += m_size.z;
				setVoxel(_offset, _voxelData[x][y][z]);
			}


}
void Matrix::flip(Sint8 p_axes, Vector3<GLfloat> p_focus) {
	Voxel ***_data = new Voxel**[m_size.x];
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
	}

	for(Uint16 x = 0; x < m_size.x; x++) {
		for(Uint16 y = 0; y < m_size.y; y++) {
			delete[] _data[x][y];
		}
		delete[] _data[x];
	}
	delete[] _data;
}
void Matrix::rotate(Sint8 p_axes, Vector3<GLfloat> p_focus) {
	Vector3<Sint16> _size;
	Voxel ***_data = new Voxel**[m_size.x];
	Vector3<GLfloat> center;
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
		addPosition(Vector3<GLfloat>(0, (m_size.z - m_size.y) / 2.f, (m_size.y - m_size.z) / 2.f));

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

		center = p_focus - getCenter();
		center.print();
		addPosition(Vector3<GLfloat>(0, center.y - center.z, center.z + center.y));
	}
	if(p_axes & AXIS_Y) {
		_size = m_size;
		for(Uint16 x = 0; x < _size.x; x++)
			for(Uint16 y = 0; y < _size.y; y++)
				for(Uint16 z = 0; z < _size.z; z++)
					_data[x][y][z] = getVoxel({x, y, z});

		setSize({m_size.z, m_size.y, m_size.x});
		addPosition(Vector3<GLfloat>((m_size.z - m_size.x) / 2.f, 0, (m_size.x - m_size.z) / 2.f));

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

		center = p_focus - getCenter();
		center.print();
		addPosition(Vector3<GLfloat>(center.x + center.z, 0, center.z - center.x));
	}
	if(p_axes & AXIS_Z) {
		_size = m_size;
		for(Uint16 x = 0; x < _size.x; x++)
			for(Uint16 y = 0; y < _size.y; y++)
				for(Uint16 z = 0; z < _size.z; z++)
					_data[x][y][z] = getVoxel({x, y, z});

		setSize({m_size.y, m_size.x, m_size.z});
		addPosition(Vector3<GLfloat>((m_size.y - m_size.x) / 2.f, (m_size.x - m_size.y) / 2.f, 0));

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

		center = p_focus - getCenter();
		center.print();
		addPosition(Vector3<GLfloat>(center.x - center.y, center.y + center.x, 0));
	}

}
void Matrix::addKeyframe(Keyframe* p_keyframe) {
	m_keyframes.push_back(p_keyframe);
}
void Matrix::applyAnimation(GLfloat p_time) {
	GLfloat _usage;
	Vector3<GLfloat> _focus, _value;
	for(Keyframe* k : m_keyframes) {
		_usage = max(0, min(1, GLfloat(p_time - k->m_start) / k->m_length));
		if(_usage > 0) {
			_focus = k->m_focus + m_pos + (m_size / 2);
			_value = k->m_value * _usage;
			glTranslatef(_focus.x, _focus.y, _focus.z);
			switch(k->m_transformationType) {
			case Keyframe::TransformationType::TRANSLATION:
				glTranslatef(_value.x, _value.y, _value.z);
				break;
			case Keyframe::TransformationType::ROTATION:
				glRotatef(_value.x, 1, 0, 0);
				glRotatef(_value.y, 0, 1, 0);
				glRotatef(_value.z, 0, 0, 1);
				break;
			case Keyframe::TransformationType::SCALE:
				glScalef(powf(k->m_value.x, _usage), powf(k->m_value.y, _usage), powf(k->m_value.z, _usage));
				break;
			default: break;
			}
			glTranslatef(-_focus.x, -_focus.y, -_focus.z);
		}
	}
}

bool Matrix::containsPoint(Vector3<Sint32> p_point) {
	return (p_point.x >= 0 && p_point.x < m_size.x
		&& p_point.y >= 0 && p_point.y < m_size.y
		&& p_point.z >= 0 && p_point.z < m_size.z);
}

void Matrix::update() {

}
void Matrix::renderMatrix() {
	glPushMatrix();
	{
		if(m_needsRasterize) {
			rasterize();
			m_needsRasterize = false;
		}
		glTranslatef(m_pos.x, m_pos.y, m_pos.z);

		m_mesh->renderMesh();
	}
	glPopMatrix();
}
void Matrix::renderOutline(OutlineType p_outline) {
	GLfloat w1 = 0.05f, w2 = w1 / 2.5f;
	Vector3<GLfloat> s1 = Vector3<GLfloat>(m_size), s2 = s1 / 2;
	Color outlineColor;
	switch(p_outline) {
	case NONE: return;
	case OUTLINE: outlineColor = Color(0.6f, 0.6f, 0.6f); break;
	case HOVERED: outlineColor = Color(0xAADDFF);  break;
	case SELECTED: outlineColor = Color(1, 1, 1); break;
	}
	glPushMatrix();
	{
		glTranslatef(m_pos.x, m_pos.y, m_pos.z);
		MMesh::render("Cube", {s2.x,      w2,        w2       }, {s1.x, w1, w1}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {s2.x,      s1.y - w2, w2       }, {s1.x, w1, w1}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {s2.x,      s1.y - w2, s1.z - w2}, {s1.x, w1, w1}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {s2.x,      w2,        s1.z - w2}, {s1.x, w1, w1}, {0, 0, 0}, outlineColor);

		MMesh::render("Cube", {w2,        s2.y,      w2       }, {w1, s1.y, w1}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {s1.x - w2, s2.y,      w2       }, {w1, s1.y, w1}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {s1.x - w2, s2.y,      s1.z - w2}, {w1, s1.y, w1}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {w2,        s2.y,      s1.z - w2}, {w1, s1.y, w1}, {0, 0, 0}, outlineColor);

		MMesh::render("Cube", {w2,        w2,        s2.z      }, {w1, w1, s1.z}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {s1.x - w2, w2,        s2.z      }, {w1, w1, s1.z}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {s1.x - w2, s1.y - w2, s2.z      }, {w1, w1, s1.z}, {0, 0, 0}, outlineColor);
		MMesh::render("Cube", {w2,        s1.y - w2, s2.z      }, {w1, w1, s1.z}, {0, 0, 0}, outlineColor);
	}
	glPopMatrix();
}

void Matrix::rasterize() {
	m_mesh->createMesh(m_voxelData, m_faceData, m_size);
}
