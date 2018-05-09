#pragma once

#include "engine\utils\LOpenGL.h"
#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Macros.h"

#include <glm\glm.hpp>
#include <vector>

class MMesh {
protected:
	static GLuint m_lineVao, m_lineVbo[2];
	static glm::vec3 m_lineVertices[2];

	static GLuint m_boxVao, m_boxVbo[2];
	static glm::vec3 m_boxVertices[24];
public:
	static void init();
	static void terminate();
	static void renderLine(glm::vec3 p_pos1, glm::vec3 p_pos2);
	static void renderLine(glm::vec3 p_pos1, glm::vec3 p_pos2, glm::vec4 p_color);
	static void renderBoxOutline(glm::vec3 p_pos, glm::vec3 p_size, glm::vec4 p_color = glm::vec4(1.f, 1.f, 1.f, 1.f));
};
