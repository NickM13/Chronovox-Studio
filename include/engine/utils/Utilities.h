#pragma once

#pragma warning (disable : 4244)

#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\global\event\GKey.h"
#include "engine\utils\global\event\GMouse.h"
#include "engine\utils\global\GScreen.h"
#include "engine\utils\logger\Logger.h"
#include "engine\utils\LOpenGL.h"
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm\glm.hpp>

class Math
{
public:
	static GLfloat round2f(GLfloat p_f, Sint32 p_decimalPlaces) {
		return GLfloat(roundf(p_f * GLfloat(pow(10, p_decimalPlaces))) / pow(10, p_decimalPlaces));
	}
	// sin of decimal number
	static GLfloat sind(GLfloat p_f) {
		return GLfloat(sin((double)(p_f * 3.14159265359 / 180)));
	}
	// cos of decimal number
	static GLfloat cosd(GLfloat p_f)  {
		return GLfloat(cos((double)(p_f * 3.14159265359 / 180)));
	}
	// tan of decimal number
	static GLfloat tand(GLfloat p_f) {
		return GLfloat(tan((double)(p_f * 3.14159265359 / 180)));
	}
	// Does not use z-axis rotation
	static glm::vec3 computeDirection(glm::vec3 p_rotation) {
		GLfloat ydist = GLfloat(2 * sind(p_rotation.x));
		GLfloat xdist = GLfloat(sqrt(pow(2, 2) - pow(ydist, 2)));
		GLfloat zdist = GLfloat(xdist * sind(p_rotation.y));
		xdist = xdist * sind((p_rotation.y - 90));
		return glm::normalize(glm::vec3(zdist, -ydist, xdist));
	}

	static glm::vec3 flipOverPoint(glm::vec3 p_start, glm::vec3 p_len, glm::vec3 p_point, bool p_flip[3]) {
		if(p_flip[0]) p_start.x = (p_start.x - (p_start.x - p_point.x) * 2) - p_len.x;
		if(p_flip[1]) p_start.y = (p_start.y - (p_start.y - p_point.y) * 2) - p_len.y;
		if(p_flip[2]) p_start.z = (p_start.z - (p_start.z - p_point.z) * 2) - p_len.z;
		return p_start;
	}

	static void initPerlin(Uint32 p_seed);
	static GLfloat perlinNoise(GLfloat x, GLfloat y, GLfloat z, Sint16 p_octaves, GLfloat p_persistence);

	static void castRay3d(glm::vec3 p_start, glm::vec3 p_direction, glm::vec3 p_boxPosition, glm::vec3 p_boxDimension, GLfloat &p_near, GLfloat &p_far, Sint8 &p_side);
	static void castBox3d(glm::vec3 p_start, glm::vec3 p_dimension, glm::vec3 p_direction, glm::vec3 p_boxPosition, glm::vec3 p_boxDimension, GLfloat &p_near, GLfloat &p_far, Sint8 &p_side);
private:
	static GLfloat perlin(GLfloat x, GLfloat y, GLfloat z);

	static const Uint8 m_permutation[256];
	static Uint8 p[512];
};

class Util {
public:
	static std::string numToStringInt(Sint32 n);
	static std::string numToStringFloat(GLfloat n);
	static std::string numToStringFloat(GLfloat n, Uint16 p_decimalPlaces);

	static std::string subchar(char* chararray, Uint32 start, Uint32 end);
};

class FileExt
{
public:
	static void writeInt(std::ofstream& p_fileStream, Sint32 p_uint);
	static void writeShort(std::ofstream& p_fileStream, Sint16 p_ushort);
	static void writeChar(std::ofstream& p_fileStream, Uint8 p_uchar);
	static void writeString(std::ofstream& p_fileStream, std::string p_string);

	static Sint32 readInt(char* p_fileStream, Uint32& p_index);
	static Sint16 readShort(char* p_fileStream, Uint32& p_index);
	static Uint8 readChar(char* p_fileStream, Uint32& p_index);
	static std::string readString(char* p_fileStream, Uint32& p_index);
};
