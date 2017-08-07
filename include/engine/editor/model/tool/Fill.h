#pragma once

#include "engine\editor\model\Matrix.h"

#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Voxel.h"

class Fill
{
public:
	static void append(Matrix* p_matrix, Vector3<Sint32> p_pos, Sint8 p_side, Voxel p_fill);
	static void insert(Matrix* p_matrix, Vector3<Sint32> p_pos, Sint8 p_side, Voxel p_fill);
};
