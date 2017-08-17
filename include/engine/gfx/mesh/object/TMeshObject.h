#pragma once

#include "engine\utils\OpenGL.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"
#include <vector>

struct TMeshObject
{
	std::string m_objectName;

	std::vector<Vector3<Vector3<GLfloat>>> m_triVertices;
	std::vector<Vector4<Vector3<GLfloat>>> m_quadVertices;

	std::vector<GLfloat> m_triShading;
	std::vector<GLfloat> m_quadShading;

	void addTriangle(Vector3<Vector3<GLfloat>> p_vertices, GLfloat p_shading)
	{
		m_triVertices.push_back(p_vertices);
		m_triShading.push_back(p_shading);
	}
	void addQuad(Vector4<Vector3<GLfloat>> p_vertices, GLfloat p_shading)
	{
		m_quadVertices.push_back(p_vertices);
		m_quadShading.push_back(p_shading);
	}
};
