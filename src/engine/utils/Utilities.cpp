#include "engine\utils\Utilities.h"
#include "engine\utils\variable\datatype\Voxel.h"

const Uint8 Math::m_permutation[256] = {151, 160, 137, 91, 90, 15,												// Hash lookup table as defined by Ken Perlin.  This is a randomly
131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,				// arranged array of all numbers from 0-255 inclusive.
190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

Uint8* Math::p = new Uint8[512];

void Math::initPerlin(Uint32 p_seed)
{
	srand(p_seed);
	for(Uint16 i = 0; i < 512; i++)
		p[i] = m_permutation[i%256];
}

GLfloat repeat = 0;

static GLfloat fade(GLfloat t)
{
	return (6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3));
}

Sint16 inc(Sint16 num)
{
	num++;
	if(repeat > 0)
		num = Sint16(fmod(num, repeat));

	return num;
}

GLfloat grad(Sint16 hash, GLfloat x, GLfloat y, GLfloat z)
{
	switch(hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0;
	}
}

GLfloat lerp(GLfloat a, GLfloat b, GLfloat x)
{
	return a + x * (b - a);
}

GLfloat Math::perlin(GLfloat x, GLfloat y, GLfloat z)
{
	if(repeat > 0)
	{
		x = fmod(x, repeat);
		y = fmod(y, repeat);
		z = fmod(z, repeat);
	}

	Sint16 xi = Sint16(x) & 255;
	Sint16 yi = Sint16(y) & 255;
	Sint16 zi = Sint16(z) & 255;

	GLfloat xf = x - Sint16(x);
	GLfloat yf = y - Sint16(y);
	GLfloat zf = z - Sint16(z);

	GLfloat u = fade(xf);
	GLfloat v = fade(yf);
	GLfloat w = fade(zf);

	Sint16 aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = p[p[p[xi] +		yi] +		zi];
	aba = p[p[p[xi] +		inc(yi)] +	zi];
	aab = p[p[p[xi] +		yi] +		inc(zi)];
	abb = p[p[p[xi] +		inc(yi)] +	inc(zi)];
	baa = p[p[p[inc(xi)] +	yi] +		zi];
	bba = p[p[p[inc(xi)] +	inc(yi)] +	zi];
	bab = p[p[p[inc(xi)] +	yi] +		inc(zi)];
	bbb = p[p[p[inc(xi)] +	inc(yi)] +	inc(zi)];

	GLfloat x1, x2, y1, y2;
	x1 = lerp(grad(aaa, xf, yf, zf),		grad(baa, xf - 1, yf, zf), u);
	x2 = lerp(grad(aba, xf, yf - 1, zf),	grad(bba, xf - 1, yf - 1, zf), u);
	y1 = lerp(x1, x2, v);
	x1 = lerp(grad(aab, xf, yf, zf - 1),	grad(bab, xf - 1, yf, zf - 1), u);
	x1 = lerp(grad(abb, xf, yf - 1, zf - 1),grad(bbb, xf - 1, yf - 1, zf - 1), u);
	y2 = lerp(x1, x2, v);

	return (lerp(y1, y2, w) + 1) / 2;
}

GLfloat Math::perlinNoise(GLfloat x, GLfloat y, GLfloat z, Sint16 p_octaves, GLfloat p_persistence)
{
	GLfloat _total = 0;
	GLfloat _frequency = 1;
	GLfloat _amplitude = 1;
	GLfloat _maxValue = 0;
	for(Sint16 i = 0; i < p_octaves; i++)
	{
		_total += perlin(x * _frequency, y * _frequency, z * _frequency) * _amplitude;

		_maxValue += _amplitude;

		_amplitude *= p_persistence;
		_frequency *= 2;
	}
	return _total / _maxValue;
}

void Math::castRay3d(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction, Vector3<GLfloat> p_boxPosition, Vector3<GLfloat> p_boxDimension, GLfloat &p_near, GLfloat &p_far, Sint8 &p_side)
{
	bool flip[3] = {p_direction.x < 0, p_direction.y < 0, p_direction.z < 0};
	p_boxPosition = flipOverPoint(p_boxPosition, p_boxDimension, p_start, flip);

	if(p_direction.x < 0)
		p_direction.x = -p_direction.x;
	if(p_direction.y < 0)
		p_direction.y = -p_direction.y;
	if(p_direction.z < 0)
		p_direction.z = -p_direction.z;

	GLfloat _near = p_near, _far = p_far;
	p_near = 0;
	Sint8 _side = 0;
	if(p_direction.x != 0)
	{
		if(p_start.x > p_boxPosition.x + p_boxDimension.x)
		{
			_near = 1;
			_far = 0;
		}
		else
		{
			_near = max(_near, (p_boxPosition.x - p_start.x) / (p_direction.x));
			_far = min(_far, (p_boxPosition.x + p_boxDimension.x - p_start.x) / (p_direction.x));
			if(_near > p_near)
			{
				_side = flip[0] ? FACE_NORTH : FACE_SOUTH;
				p_near = _near;
			}
		}
	}
	else if(p_start.x < p_boxPosition.x || p_start.x > p_boxPosition.x + p_boxDimension.x)
	{
		_near = 1;
		_far = 0;
	}
	else
	{
		_near = max(_near, 0);
		_far = min(_far, 1);
		if(_near > p_near)
		{
			_side = flip[0] ? FACE_NORTH : FACE_SOUTH;
			p_near = _near;
		}
	}

	if(p_direction.y != 0)
	{
		if(p_start.y > p_boxPosition.y + p_boxDimension.y)
		{
			_near = 1;
			_far = 0;
		}
		else
		{
			_near = max(_near, (p_boxPosition.y - p_start.y) / (p_direction.y));
			_far = min(_far, (p_boxPosition.y + p_boxDimension.y - p_start.y) / (p_direction.y));
			if(_near > p_near)
			{
				_side = flip[1] ? FACE_TOP : FACE_BOTTOM;
				p_near = _near;
			}
		}
	}
	else if(p_start.y < p_boxPosition.y || p_start.y > p_boxPosition.y + p_boxDimension.y)
	{
		_near = 1;
		_far = 0;
	}
	else
	{
		_near = max(_near, 0);
		_far = min(_far, 1);
		if(_near > p_near)
		{
			_side = flip[1] ? FACE_TOP : FACE_BOTTOM;
			p_near = _near;
		}
	}

	if(p_direction.z != 0)
	{
		if(p_start.z > p_boxPosition.z + p_boxDimension.z)
		{
			_near = 1;
			_far = 0;
		}
		else
		{
			_near = max(_near, (p_boxPosition.z - p_start.z) / (p_direction.z));
			_far = min(_far, (p_boxPosition.z + p_boxDimension.z - p_start.z) / (p_direction.z));
			if(_near > p_near)
			{
				_side = flip[2] ? FACE_EAST : FACE_WEST;
				p_near = _near;
			}
		}
	}
	else if(p_start.z < p_boxPosition.z || p_start.z > p_boxPosition.z + p_boxDimension.z)
	{
		_near = 1;
		_far = 0;
	}
	else
	{
		_near = max(_near, 0);
		_far = min(_far, 1);
		if(_near > p_near)
		{
			_side = flip[2] ? FACE_EAST : FACE_WEST;
			p_near = _near;
		}
	}
	
	if(_near < _far)
	{
		p_side = _side;
		p_far = _far;
	}
	else
	{
		p_side = 0;
		p_near = 1;
		p_far = 0;
	}
}


void Math::castBox3d(Vector3<GLfloat> p_start, Vector3<GLfloat> p_dimension, Vector3<GLfloat> p_direction, Vector3<GLfloat> p_boxPosition, Vector3<GLfloat> p_boxDimension, GLfloat &p_near, GLfloat &p_far, Sint8 &p_side)
{
	GLfloat _near = 1, _nearCast;
	GLfloat _far = 1;
	Sint8 _side = 0, _sideCast;

	GLfloat _c = 0.0001f; // Slight offset for corners to cast from
	Vector3<GLfloat> _dCount = (p_dimension / (p_boxDimension)).ceil();
	Vector3<GLfloat> _dOffset = p_dimension / (_dCount) - _c * 2;

	p_direction = p_direction.getNormal();

	if(p_direction.x < 0)
	{
		for(Uint16 y = 0; y <= _dCount.y; y++)
		{
			for(Uint16 z = 0; z <= _dCount.z; z++)
			{
				_nearCast = 0;
				_sideCast = 0;
				_far = 1;
				castRay3d(p_start + Vector3<GLfloat>(-_c, y * _dOffset.y + _c, z * _dOffset.z + _c), p_direction, p_boxPosition, p_boxDimension, _nearCast, _far, _sideCast);
				if((_nearCast < _near) && (_sideCast & 1))
				{
					_side = 1;
					_near = _nearCast;
				}
			}
		}
	}
	else
	{
		for(Uint16 y = 0; y <= _dCount.y; y++)
		{
			for(Uint16 z = 0; z <= _dCount.z; z++)
			{
				_nearCast = 0;
				_sideCast = 0;
				_far = 1;
				castRay3d(p_start + Vector3<GLfloat>(p_dimension.x + _c, y * _dOffset.y + _c, z * _dOffset.z + _c), p_direction, p_boxPosition, p_boxDimension, _nearCast, _far, _sideCast);
				if((_nearCast < _near) && (_sideCast & 1))
				{
					_side = 1;
					_near = _nearCast;
				}
			}
		}
	}


	if(p_direction.y < 0)
	{
		for(Uint16 x = 0; x <= _dCount.x; x++)
		{
			for(Uint16 z = 0; z <= _dCount.z; z++)
			{
				_nearCast = 0;
				_sideCast = 0;
				_far = 1;
				castRay3d(p_start + Vector3<GLfloat>(x * _dOffset.x + _c, -_c, z * _dOffset.z + _c), p_direction, p_boxPosition, p_boxDimension, _nearCast, _far, _sideCast);
				if((_nearCast < _near) && (_sideCast & 2))
				{
					_side = 2;
					_near = _nearCast;
				}
			}
		}
	}
	else
	{
		for(Uint16 x = 0; x <= _dCount.x; x++)
		{
			for(Uint16 z = 0; z <= _dCount.z; z++)
			{
				_nearCast = 0;
				_sideCast = 0;
				_far = 1;
				castRay3d(p_start + Vector3<GLfloat>(x * _dOffset.x + _c, p_dimension.y + _c, z * _dOffset.z + _c), p_direction, p_boxPosition, p_boxDimension, _nearCast, _far, _sideCast);
				if((_nearCast < _near) && (_sideCast & 2))
				{
					_side = 2;
					_near = _nearCast;
				}
			}
		}
	}


	if(p_direction.z < 0)
	{
		for(Uint16 x = 0; x <= _dCount.x; x++)
		{
			for(Uint16 y = 0; y <= _dCount.y; y++)
			{
				_nearCast = 0;
				_sideCast = 0;
				_far = 1;
				castRay3d(p_start + Vector3<GLfloat>(x * _dOffset.x + _c, y * _dOffset.y + _c, -_c), p_direction, p_boxPosition, p_boxDimension, _nearCast, _far, _sideCast);
				if((_nearCast < _near) && (_sideCast & 4))
				{
					_side = 4;
					_near = _nearCast;
				}
			}
		}
	}
	else
	{
		for(Uint16 x = 0; x <= _dCount.x; x++)
		{
			for(Uint16 y = 0; y <= _dCount.y; y++)
			{
				_nearCast = 0;
				_sideCast = 0;
				_far = 1;
				castRay3d(p_start + Vector3<GLfloat>(x * _dOffset.x + _c, y * _dOffset.y + _c, p_dimension.z + _c), p_direction, p_boxPosition, p_boxDimension, _nearCast, _far, _sideCast);
				if((_nearCast < _near) && (_sideCast & 4))
				{
					_side = 4;
					_near = _nearCast;
				}
			}
		}
	}

	if(_side == 0)
	{
		for(Uint16 x = 0; x <= _dCount.x; x++)
		{
			for(Uint16 y = 0; y <= _dCount.y; y++)
			{
				for(Uint16 z = 0; z <= _dCount.z; z++)
				{
					_nearCast = 0;
					_sideCast = 0;
					_far = 1;
					castRay3d(p_start + Vector3<GLfloat>(x * _dOffset.x + _c, y * _dOffset.y + _c, z * _dOffset.z + _c), p_direction, p_boxPosition, p_boxDimension, _nearCast, _far, _sideCast);
					if((_nearCast < _near) && (_sideCast & 4))
					{
						_side = 4 + 2 + 1;
						_near = _nearCast;
					}
				}
			}
		}
	}


	if(_side == 0)
		_near = 1;
	else
		_near = _near * 0.95f;
	p_near = _near;
	p_side = _side;
}
