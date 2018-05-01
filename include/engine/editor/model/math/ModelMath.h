#pragma once

#include "..\EditMatrix.h"
#include "..\Matrix.h"

namespace ModelMath {
	bool castRayMatrix(glm::vec3 p_start, glm::vec3 p_direction, Matrix* p_matrix, GLfloat &p_near, GLfloat &p_far);
	bool castRayMatrices(glm::vec3 p_start, glm::vec3 p_direction, std::vector<Matrix*> p_matrices, Sint16 &p_selectedMatrix, GLfloat &p_near, GLfloat &p_far);
	bool castRayVoxel(glm::vec3 p_start, glm::vec3 p_direction, Matrix *p_matrix, GLfloat &p_near, GLfloat &p_far, glm::ivec3 &p_selectedVoxel, Sint8 &p_side, glm::ivec3 &p_selectedVoxelOffset);
	bool castRayScale(glm::vec3 p_start, glm::vec3 p_direction, Matrix *p_matrix, GLfloat &p_near, GLfloat &p_far, glm::vec3 &p_scalePos, Sint8 &p_scale);
};
