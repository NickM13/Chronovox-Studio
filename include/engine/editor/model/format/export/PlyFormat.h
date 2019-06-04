#pragma once

#include "..\..\Matrix.h"

class PlyFormat  {
private:
	static Uint32 m_index;
	static char* m_data;

	static glm::vec3 m_scale;
	static glm::vec3 m_offset;
	enum class PlyMode {
		ASCII,
		BIN_L,
		BIN_B
	};
	static PlyMode m_plyMode;

	struct Vertex {
		Sint32 x, y, z;
		Uint8 r, g, b;
	};

	static Sint32 find(std::vector<Vertex>& vertexList, Vertex& vertex);
public:
	static void init(glm::vec3 p_scale, glm::vec3 p_offset);

	static bool save(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
};
