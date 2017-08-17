#include "engine\editor\model\tool\MatrixCast.h"

bool MatrixCast::castRayMatrix(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction, std::vector<Matrix*> p_matrices, Sint16 &p_selectedMatrix, GLfloat &p_near, GLfloat &p_far)
{
	GLfloat _mNear, _mFar, _vNear, _vFar, _cNear = 1, _cFar = 0;
	GLfloat _near, _far;
	GLfloat _iNear = p_near, _iFar = p_far;
	Sint8 _side = 0;
	Sint32 _cMatrix = -1;
	Vector3<Sint32> m_selectedVoxel, m_selectedVoxelOffset;

	for(Uint16 i = 0; i < p_matrices.size(); i++)
	{
		
		_mNear = _iNear;
		_mFar = _iFar;
		Math::castRay3d(p_start, p_direction, p_matrices[i]->getPos(), p_matrices[i]->getSize(), _mNear, _mFar, _side);
		if(_mNear < _cNear)
		{
			_vNear = _mNear, _vFar = _mFar;
			castRayVoxel(p_start, p_direction, p_matrices[i], _vNear, _vFar, m_selectedVoxel, _side, m_selectedVoxelOffset);
			if(_vFar == 1)
				_near = 0.999f;
			else
				_near = _mNear + ((_mNear - _mFar) * _vNear);
			_far = _mFar;
			if(_near < _cNear)
			{
				_cMatrix = i;
				_cNear = _near;
				_cFar = _far;
				p_near = _mNear + ((_mNear - _mFar) * _vNear);
				p_far = _cFar;
			}
		}
	}
	p_selectedMatrix = _cMatrix;
	return (p_selectedMatrix != -1);
}

bool MatrixCast::castRayVoxel(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction, Matrix *p_matrix, GLfloat &p_near, GLfloat &p_far, Vector3<Sint32> &p_selectedVoxel, Sint8 &p_side, Vector3<Sint32> &p_selectedVoxelOffset)
{
	GLfloat _near = 0, _far = 1;
	GLfloat _cNear = 0, _cFar = 1;
	Sint32 _close = -1;
	Vector2<GLfloat> _closest = {1, 1};
	Vector3<Sint32> _size = p_matrix->getSize(), _pos = p_matrix->getPos();
	p_side = 0;

	p_selectedVoxel = p_selectedVoxelOffset = {-1, -1, -1};

	p_near -= 0.000001f;
	p_far += 0.000001f;
	p_start = p_start - p_matrix->getPos() + p_direction * p_near;
	p_direction = p_direction * (p_far - p_near);

	Math::castRay3d(p_start, p_direction, p_start.floor(), {1, 1, 1}, _cNear, _cFar, p_side);

	Vector3<GLfloat> _floor = (p_start).floor();
	int i = 500;
	do
	{
		if(_floor.x >= 0 && _floor.y >= 0 && _floor.z >= 0 &&
			_floor.x < _size.x && _floor.y < _size.y && _floor.z < _size.z)
		{
			if(p_matrix->getVoxel(_floor).interactionType != 0)
			{
				p_near = _near;
				p_far = _near;
				p_selectedVoxelOffset = p_selectedVoxel;
				p_selectedVoxel = _floor;
				return true;
			}
		}
		p_selectedVoxel = _floor;
		_near = _cFar + 0.0001f;
		_cNear = 0, _cFar = 1;
		Math::castRay3d(p_start, p_direction, (p_start + p_direction * (_near)).floor(), {1, 1, 1}, _cNear, _cFar, p_side);
		_floor = (p_start + p_direction * _near).floor();
		i--;
	} while(_cNear < 1 && _near < _far && i > 0);
	p_selectedVoxelOffset = p_selectedVoxel;
	p_selectedVoxel = _floor;
	switch(p_side)
	{
	case FACE_NORTH:	p_selectedVoxelOffset.x++; break;
	case FACE_SOUTH:	p_selectedVoxelOffset.x--; break;
	case FACE_TOP:	p_selectedVoxelOffset.y++; break;
	case FACE_BOTTOM:		p_selectedVoxelOffset.y--; break;
	case FACE_EAST:		p_selectedVoxelOffset.z++; break;
	case FACE_WEST:		p_selectedVoxelOffset.z--; break;
	}
	p_near = p_far = 1;
	return false;
}
bool MatrixCast::castRayScale(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction, Matrix* p_matrix, GLfloat &p_near, GLfloat &p_far, Vector3<GLfloat> &p_scalePos, Sint8 &p_scale)
{
	if(!(GMouse::m_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_DOWN))
	{
		GLfloat _near = 0, _far = 1;
		Sint8 _side = 0;
		Sint32 _close = -1;
		Vector2<GLfloat> _closest = {1, 1};
		p_scale = 0;
		GLfloat size = 0.2f;

		Vector3<GLfloat> s = Vector3<GLfloat>(p_matrix->getSize()) / 2;
		Vector3<GLfloat> _offset = p_matrix->getPos() + s;

		Math::castRay3d(p_start, p_direction, _offset + Vector3<GLfloat>{-(s.x + 2), -size, -size}, {2, size * 2, size * 2}, _near, _far, _side);
		if(_near < _closest.x)
		{
			p_scale = FACE_SOUTH;
			_closest = {_near, _far};
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + Vector3<GLfloat>{(s.x), -size, -size}, {2, size * 2, size * 2}, _near, _far, _side);
		if(_near < _closest.x)
		{
			p_scale = FACE_NORTH;
			_closest = {_near, _far};
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + Vector3<GLfloat>{-size, -(s.y + 2), -size}, {size * 2, 2, size * 2}, _near, _far, _side);
		if(_near < _closest.x)
		{
			p_scale = FACE_BOTTOM;
			_closest = {_near, _far};
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + Vector3<GLfloat>{-size, (s.y), -size}, {size * 2, 2, size * 2}, _near, _far, _side);
		if(_near < _closest.x)
		{
			p_scale = FACE_TOP;
			_closest = {_near, _far};
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + Vector3<GLfloat>{-size, -size, -(s.z + 2)}, {size * 2, size * 2, 2}, _near, _far, _side);
		if(_near < _closest.x)
		{
			p_scale = FACE_WEST;
			_closest = {_near, _far};
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + Vector3<GLfloat>{-size, -size, (s.z)}, {size * 2, size * 2, 2}, _near, _far, _side);
		if(_near < _closest.x)
		{
			p_scale = FACE_EAST;
			_closest = {_near, _far};
		}

		if(p_scale != 0)
			p_scalePos = p_start + p_direction * _closest.x;
		return (p_scale != 0);
	}
	return false;
}
