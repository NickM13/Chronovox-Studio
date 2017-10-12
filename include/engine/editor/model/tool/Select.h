#pragma once

#include "..\Matrix.h"

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Voxel.h"

class Select {
public:
	static void start(Matrix* p_matrix, Vector3<Sint32> p_pos);
	static void update(Vector3<Sint32> p_pos);
	static void end();
	static void render();
private:
	static Matrix *m_matrix;
	static Vector3<Sint32> m_startPos, m_lastPos;
};
