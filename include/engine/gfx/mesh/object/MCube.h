#include "engine\gfx\mesh\object\TMeshObject.h"
#include <iostream>

// Cube Mesh
struct MCube : public TMeshObject
{
	MCube()
	{
		m_objectName = "Cube";
		
		addQuad({	{-0.5f, -0.5f, -0.5f},	{-0.5f, -0.5f, 0.5f},	{-0.5f, 0.5f, 0.5f},	{-0.5f, 0.5f, -0.5f}	}, glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
		addQuad({	{0.5f, -0.5f, -0.5f},	{0.5f, 0.5f, -0.5f},	{0.5f, 0.5f, 0.5f},		{0.5f, -0.5f, 0.5f}		}, glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));
		addQuad({	{-0.5f, -0.5f, -0.5f},	{0.5f, -0.5f, -0.5f},	{0.5f, -0.5f, 0.5f},	{-0.5f, -0.5f, 0.5f}	}, glm::vec4(0.80f, 0.80f, 0.80f, 1.0f));
		addQuad({	{-0.5f, 0.5f, -0.5f},	{-0.5f, 0.5f, 0.5f},	{0.5f, 0.5f, 0.5f},		{0.5f, 0.5f, -0.5f}		}, glm::vec4(1.00f, 1.00f, 1.00f, 1.0f));
		addQuad({	{-0.5f, -0.5f, -0.5f},	{-0.5f, 0.5f, -0.5f},	{0.5f, 0.5f, -0.5f},	{0.5f, -0.5f, -0.5f}	}, glm::vec4(0.90f, 0.90f, 0.90f, 1.0f));
		addQuad({	{-0.5f, -0.5f, 0.5f},	{0.5f, -0.5f, 0.5f},	{0.5f, 0.5f, 0.5f},		{-0.5f, 0.5f, 0.5f}		}, glm::vec4(0.90f, 0.90f, 0.90f, 1.0f));
	}
};
