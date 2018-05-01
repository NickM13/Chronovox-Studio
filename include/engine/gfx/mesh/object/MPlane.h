#include "engine\gfx\mesh\object\TMeshObject.h"
#include <iostream>

// Cube Mesh
struct MPlane : public TMeshObject {
	MPlane()
	{
		m_objectName = "Plane";

		addQuad({{-0.5f, 0.5f, -0.5f},{-0.5f, 0.5f, 0.5f},{0.5f, 0.5f, 0.5f},{0.5f, 0.5f, -0.5f}}, glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addQuad({{-0.5f, 0.5f, -0.5f},{0.5f, 0.5f, -0.5f},{0.5f, 0.5f, 0.5f},{-0.5f, 0.5f, 0.5f}}, glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
	}
};
