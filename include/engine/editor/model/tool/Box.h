#pragma once

#include "..\Matrix.h"

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Voxel.h"

class Box
{
public:
	static void start(Matrix* p_matrix, Vector3<Sint32> p_pos, Voxel p_voxel, bool p_replace);
	static void update(Vector3<Sint32> p_pos);
	static void end();
	static void render();
	static bool running() { return m_matrix != 0; };
private:
	static Matrix *m_matrix, *m_initMatrix; // *Initial state matrix
	static Voxel m_voxel;
	static Vector3<Sint32> m_startPos, m_lastPos;
	static bool m_replace;
};
