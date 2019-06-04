#pragma once

#include "..\..\Matrix.h"

class ObjFormat {
private:
	static Uint32 m_index;
	static char* m_data;

	static glm::vec3 m_scale;
	static glm::vec3 m_offset;
	static GLfloat m_texelSize;
	static std::string m_texSubfolder;
public:
	static void init(glm::vec3 p_scale, glm::vec3 p_offset, GLfloat p_texelSize, std::string p_texSubfolder);

	static bool save(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
};
