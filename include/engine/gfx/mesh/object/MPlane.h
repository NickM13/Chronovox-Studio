#include "engine\gfx\mesh\object\TMeshObject.h"
#include <iostream>

// Cube Mesh
struct MPlane : public TMeshObject {
	MPlane()
	{
		m_objectName = "Plane";

		addQuad({{-0.5f, 0.5f, -0.5f},{-0.5f, 0.5f, 0.5f},{0.5f, 0.5f, 0.5f},{0.5f, 0.5f, -0.5f}}, 1.00f);
	}
};
