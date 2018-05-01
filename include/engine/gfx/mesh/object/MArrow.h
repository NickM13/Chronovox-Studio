#pragma once

#include "TMeshObject.h"

// Arrow Mesh
struct MArrow : public TMeshObject
{
	MArrow()
	{
		m_objectName = "Arrow";

		addQuad({{-0.5f, 0, -0.5f},{-0.5f, 0, 0.5f},{-0.5f, 0.75f, 0.5f},{-0.5f, 0.75f, -0.5f}}, glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
		addQuad({{0.5f, 0, -0.5f},{0.5f, 0.75f, -0.5f},{0.5f, 0.75f, 0.5f},{0.5f, 0, 0.5f}}, glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));
		addQuad({{-0.5f, 0, -0.5f},{0.5f, 0, -0.5f},{0.5f, 0, 0.5f},{-0.5f, 0, 0.5f}}, glm::vec4(0.80f, 0.80f, 0.80f, 1.0f));
		addQuad({{-1, 0.75f, -1},{1, 0.75f, -1},{1, 0.75f, 1},{-1, 0.75f, 1}}, glm::vec4(0.80f, 0.80f, 0.80f, 1.0f));
		addQuad({{-0.5f, 0, -0.5f},{-0.5f, 0.75f, -0.5f},{0.5f, 0.75f, -0.5f},{0.5f, 0, -0.5f}}, glm::vec4(0.90f, 0.90f, 0.90f, 1.0f));
		addQuad({{-0.5f, 0, 0.5f},{0.5f, 0, 0.5f},{0.5f, 0.75f, 0.5f},{-0.5f, 0.75f, 0.5f}},  glm::vec4(0.90f, 0.90f, 0.90f, 1.0f));

		addTriangle({{1, 0.75f, -1},{-1, 0.75f, -1},{0, 1, 0}}, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
		addTriangle({{1, 0.75f, 1},{1, 0.75f, -1},{0, 1, 0}}, glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
		addTriangle({{-1, 0.75f, 1},{1, 0.75f, 1},{0, 1, 0}}, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
		addTriangle({{-1, 0.75f, -1},{-1, 0.75f, 1},{0, 1, 0}}, glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));
	}
};
