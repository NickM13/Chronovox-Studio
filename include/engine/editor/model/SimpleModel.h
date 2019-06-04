#pragma once

#include "Matrix.h"

#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\editor\model\tool\MTool.h"

#include "format\LFormat.h"

class SimpleModel {
private:
	// Matrices
	std::vector<Matrix*> m_matrices;

	glm::vec3 dir, pos, norm;
	GLfloat dist, denom, p;
public:
	SimpleModel();
	~SimpleModel();
	void init();

	void setVoxel(Uint16 p_matrix, glm::ivec3 p_pos, Voxel p_voxel);
	Voxel getVoxel(Uint16 p_matrix, glm::ivec3 p_pos);
	Uint16 getVoxelId(Uint16 p_matrix, glm::ivec3 p_pos);

	void input(Sint8 p_guiFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
	void renderShadow();

	std::vector<std::string> getMatrixNames();
	Matrix* getMatrix(Sint16 id);
	Matrix* getMatrix(std::string p_name);
	std::vector<Matrix*>* getMatrixList() { return &m_matrices; }

	void open();

	bool loadOpen(std::string p_fileName);
};
