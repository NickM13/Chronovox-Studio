#include "engine\gfx\mesh\object\TMeshObject.h"
#include <iostream>

// Cube Skeleton Mesh
struct MSCube : public TMeshObject
{
	MSCube()
	{
		m_objectName = "SCube";

		/*
		addLine(Vector2<glm::vec3>(	glm::vec3(-0.5f, -0.5f, -0.5f}, glm::vec3(0.5f, -0.5f, -0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(	glm::vec3(0.5f, -0.5f, -0.5f}, glm::vec3(0.5f, -0.5f, 0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(	glm::vec3(0.5f, -0.5f, 0.5f}, glm::vec3(-0.5f, -0.5f, 0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(	glm::vec3(-0.5f, -0.5f, 0.5f}, glm::vec3(-0.5f, -0.5f, -0.5f}), glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));

		addLine(Vector2<glm::vec3>(glm::vec3(-0.5f, 0.5f, -0.5f}, glm::vec3(0.5f, 0.5f, -0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(glm::vec3(0.5f, 0.5f, -0.5f}, glm::vec3(0.5f, 0.5f, 0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(glm::vec3(0.5f, 0.5f, 0.5f}, glm::vec3(-0.5f, 0.5f, 0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(glm::vec3(-0.5f, 0.5f, 0.5f}, glm::vec3(-0.5f, 0.5f, -0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		
		addLine(Vector2<glm::vec3>(	{-0.5f, 0.5f, -0.5f},	{-0.5f, -0.5f, -0.5f}), glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(	{0.5f, 0.5f, -0.5f},	{0.5f, -0.5f, -0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(	{0.5f, 0.5f, 0.5f},		{0.5f, -0.5f, 0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addLine(Vector2<glm::vec3>(	{-0.5f, 0.5f, 0.5f},	{-0.5f, -0.5f, 0.5f}),	glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		*/
	}
};
