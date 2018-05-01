#pragma once

#include "engine\utils\LOpenGL.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"
#include <vector>

#include <glm\glm.hpp>

struct TMeshObject {
	std::string m_objectName;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec4> m_colors;

	std::vector<Vector2<glm::vec3>> m_lineVertices;
	std::vector<Vector3<glm::vec3>> m_triVertices;
	std::vector<Vector4<glm::vec3>> m_quadVertices;

	std::vector<Vector2<glm::vec4>> m_lineColors;
	std::vector<Vector3<glm::vec4>> m_triColors;
	std::vector<Vector4<glm::vec4>> m_quadColors;

	GLuint m_vaoId, m_vboId[2];
	size_t m_lineCount, m_triCount, m_quadCount;

	void addLine(Vector2<glm::vec3> p_vertices, glm::vec4 p_color);
	void addTriangle(Vector3<glm::vec3> p_vertices, glm::vec4 p_color);
	void addQuad(Vector4<glm::vec3> p_vertices, glm::vec4 p_color);

	void rasterize();
};
