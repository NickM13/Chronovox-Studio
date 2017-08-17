#include "engine\gfx\mesh\object\TMeshObject.h"
#include <iostream>

// Cube Mesh
struct MCube : public TMeshObject
{
	MCube()
	{
		m_objectName = "Cube";
		
		addQuad({{0, 0, 0},{0, 0, 1},{0, 1, 1},{0, 1, 0}}, 0.85f);
		addQuad({{1, 0, 0},{1, 1, 0},{1, 1, 1},{1, 0, 1}}, 0.95f);
		addQuad({{0, 0, 0},{1, 0, 0},{1, 0, 1},{0, 0, 1}}, 0.80f);
		addQuad({{0, 1, 0},{0, 1, 1},{1, 1, 1},{1, 1, 0}}, 1.00f);
		addQuad({{0, 0, 0},{0, 1, 0},{1, 1, 0},{1, 0, 0}}, 0.90f);
		addQuad({{0, 0, 1},{1, 0, 1},{1, 1, 1},{0, 1, 1}}, 0.90f);
	}
};
