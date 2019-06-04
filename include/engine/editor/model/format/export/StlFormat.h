#pragma once

#include "..\..\Matrix.h"

class StlFormat {
private:
	static Uint32 m_index;
	static char* m_data;

	static glm::vec3 m_scale;
	static glm::vec3 m_offset;

	static void printHeader(std::ofstream& p_file, std::string p_header);
public:
	static void init(glm::vec3 p_scale, glm::vec3 p_offset);

	static bool save(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
};
