#pragma once

#include "..\EditMatrix.h"
#include "..\Matrix.h"

class MatrixCast
{
public:
	static bool castRayMatrix(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction, std::vector<Matrix*> p_matrices, Sint16 &p_selectedMatrix, GLfloat &p_near, GLfloat &p_far);
	static bool castRayVoxel(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction, Matrix *p_matrix, GLfloat &p_near, GLfloat &p_far, Vector3<Sint32> &p_selectedVoxel, Sint8 &p_side, Vector3<Sint32> &p_selectedVoxelOffset);
	static bool castRayScale(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction, Matrix *p_matrix, GLfloat &p_near, GLfloat &p_far, Vector3<GLfloat> &p_scalePos, Sint8 &p_scale);
};
