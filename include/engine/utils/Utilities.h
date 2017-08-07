#pragma once

#pragma warning (disable : 4244)

#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\event\KeyStates.h"
#include "engine\utils\event\MouseStates.h"
#include "engine\utils\Globals.h"
#include "engine\utils\OpenGL.h"
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

class Math
{
public:
	static GLfloat round2f(GLfloat p_f, Sint32 p_decimalPlaces)
	{
		return GLfloat(roundf(p_f * GLfloat(pow(10, p_decimalPlaces))) / pow(10, p_decimalPlaces));
	}

	static GLfloat sind(GLfloat p_f) /* sin of decimal number */
	{
		return GLfloat(sin((double)(p_f * 3.14159265359 / 180)));
	}

	static GLfloat cosd(GLfloat p_f) /* cos of decimal number */
	{
		return GLfloat(cos((double)(p_f * 3.14159265359 / 180)));
	}

	static GLfloat tand(GLfloat p_f) /* tan of decimal number */
	{
		return GLfloat(tan((double)(p_f * 3.14159265359 / 180)));
	}

	// Does not use z-axis rotation
	static Vector3<GLfloat> computeDirection(Vector3<GLfloat> p_rotation)
	{
		GLfloat ydist = GLfloat(2 * sind(p_rotation.x));
		GLfloat xdist = GLfloat(sqrt(pow(2, 2) - pow(ydist, 2)));
		GLfloat zdist = GLfloat(xdist * sind(p_rotation.y));
		xdist = xdist * sind((p_rotation.y - 90));
		return Vector3< GLfloat >(zdist, -ydist, xdist).getNormal();
	}

	static Vector3<GLfloat> flipOverPoint(Vector3<GLfloat> p_start, Vector3<GLfloat> p_len, Vector3<GLfloat> p_point, bool p_flip[3])
	{
		if(p_flip[0]) p_start.x = (p_start.x - (p_start.x - p_point.x) * 2) - p_len.x;
		if(p_flip[1]) p_start.y = (p_start.y - (p_start.y - p_point.y) * 2) - p_len.y;
		if(p_flip[2]) p_start.z = (p_start.z - (p_start.z - p_point.z) * 2) - p_len.z;
		return p_start;
	}

	static void initPerlin(Uint32 p_seed);
	static GLfloat perlinNoise(GLfloat x, GLfloat y, GLfloat z, Sint16 p_octaves, GLfloat p_persistence);

	static void castRay3d(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction, Vector3<GLfloat> p_boxPosition, Vector3<GLfloat> p_boxDimension, GLfloat &p_near, GLfloat &p_far, Sint8 &p_side);
	static void castBox3d(Vector3<GLfloat> p_start, Vector3<GLfloat> p_dimension, Vector3<GLfloat> p_direction, Vector3<GLfloat> p_boxPosition, Vector3<GLfloat> p_boxDimension, GLfloat &p_near, GLfloat &p_far, Sint8 &p_side);
private:
	static GLfloat perlin(GLfloat x, GLfloat y, GLfloat z);

	static const Uint8 m_permutation[256];
	static Uint8* p;
};

class Util
{
public:
	template<class T>
	static std::string numToString(T n, Uint16 p_decimalPlaces = 0)
	{
		try
		{
			return std::string(std::to_string(n)).substr(0, std::string(std::to_string(n)).find('.', 0) + p_decimalPlaces + (p_decimalPlaces > 0 ? 1 : 0));
		}
		catch(...) {
			std::cout << "Warning: Arg passed to Util::numToString( T n ) was not a number." << std::endl;;
		}
		return "0";
	}

	template<class T>
	static std::string stringToNum(T s)
	{
		std::string save = "";
		std::ostringstream convert;
		convert.str(std::string());
		try {
			convert << s;
			save = convert.str();
			return save;
		}
		catch(std::exception e) {
			return std::string("Incorrect Variable Type!");
		}
	}

	static std::string subchar(char* chararray, Uint32 start, Uint32 end)
	{
		std::string subbed;
		for(Uint32 i = start; i < end + 1; i++)
		{
			subbed += chararray[i];
		}
		return subbed;
	}

};

class FileExt
{
public:
	static void writeInt(std::ofstream& p_fileStream, Sint32 p_uint)
	{
		p_fileStream << Uint8((p_uint & 0xFF000000) >> 24);
		p_fileStream << Uint8((p_uint & 0xFF0000) >> 16);
		p_fileStream << Uint8((p_uint & 0xFF00) >> 8);
		p_fileStream << Uint8((p_uint & 0xFF));
	}
	static void writeShort(std::ofstream& p_fileStream, Sint16 p_ushort)
	{
		p_fileStream << Uint8((p_ushort & 0xFF00) >> 8);
		p_fileStream << Uint8((p_ushort & 0xFF));
	}
	static void writeChar(std::ofstream& p_fileStream, Uint8 p_uchar)
	{
		p_fileStream << p_uchar;
	}
	static void writeString(std::ofstream& p_fileStream, std::string p_string)
	{
		writeChar(p_fileStream, (Uint8)p_string.length());
		for(Uint16 i = 0; i < p_string.length(); i++)
			p_fileStream << p_string[i];
	}

	static Sint32 readInt(char* p_fileStream, Uint32& p_index)
	{
		Sint32 _value = 0;
		_value += readChar(p_fileStream, p_index) << 24;
		_value += readChar(p_fileStream, p_index) << 16;
		_value += readChar(p_fileStream, p_index) << 8;
		_value += readChar(p_fileStream, p_index);
		return _value;
	}
	static Sint16 readShort(char* p_fileStream, Uint32& p_index)
	{
		Sint16 _value = 0;
		_value += readChar(p_fileStream, p_index) << 8;
		_value += readChar(p_fileStream, p_index);
		return _value;
	}
	static Uint8 readChar(char* p_fileStream, Uint32& p_index)
	{
		p_index = p_index + 1;
		return Uint8(p_fileStream[p_index - 1]);
	}
	static std::string readString(char* p_fileStream, Uint32& p_index)
	{
		std::string str = "";
		Uint8 size = readChar(p_fileStream, p_index);
		for(Uint8 i = 0; i < size; i++)
			str += ((char)readChar(p_fileStream, p_index));
		return str;
	}
};
