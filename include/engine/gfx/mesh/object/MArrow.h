#pragma once

#include "TMeshObject.h"

// Arrow Mesh
struct MArrow : public TMeshObject
{
	MArrow()
	{
		m_objectName = "Arrow";

		addQuad({{-0.5f, 0, -0.5f},{-0.5f, 0, 0.5f},{-0.5f, 0.75f, 0.5f},{-0.5f, 0.75f, -0.5f}}, 0.85f);
		addQuad({{0.5f, 0, -0.5f},{0.5f, 0.75f, -0.5f},{0.5f, 0.75f, 0.5f},{0.5f, 0, 0.5f}}, 0.95f);
		addQuad({{-0.5f, 0, -0.5f},{0.5f, 0, -0.5f},{0.5f, 0, 0.5f},{-0.5f, 0, 0.5f}}, 0.80f);
		addQuad({{-1, 0.75f, -1},{1, 0.75f, -1},{1, 0.75f, 1},{-1, 0.75f, 1}}, 0.80f);
		addQuad({{-0.5f, 0, -0.5f},{-0.5f, 0.75f, -0.5f},{0.5f, 0.75f, -0.5f},{0.5f, 0, -0.5f}}, 0.90f);
		addQuad({{-0.5f, 0, 0.5f},{0.5f, 0, 0.5f},{0.5f, 0.75f, 0.5f},{-0.5f, 0.75f, 0.5f}}, 0.90f);

		addTriangle({{1, 0.75f, -1},{-1, 0.75f, -1},{0, 1, 0}}, 0.9f);
		addTriangle({{1, 0.75f, 1},{1, 0.75f, -1},{0, 1, 0}}, 0.85f);
		addTriangle({{-1, 0.75f, 1},{1, 0.75f, 1},{0, 1, 0}}, 0.9f);
		addTriangle({{-1, 0.75f, -1},{-1, 0.75f, 1},{0, 1, 0}}, 0.95f);
	}
};
