#include "engine\gfx\mesh\object\TMeshObject.h"
#include <iostream>

// Cube Skeleton Mesh
struct MSCube : public TMeshObject
{
	MSCube()
	{
		m_objectName = "SCube";

		addLine({	{-0.5f, -0.5f, -0.5f},	{0.5f, -0.5f, -0.5f}},	1);
		addLine({	{0.5f, -0.5f, -0.5f},	{0.5f, -0.5f, 0.5f}},	1);
		addLine({	{0.5f, -0.5f, 0.5f},	{-0.5f, -0.5f, 0.5f}},	1);
		addLine({	{-0.5f, -0.5f, 0.5f},	{-0.5f, -0.5f, -0.5f}}, 1);

		addLine({	{-0.5f, 0.5f, -0.5f},	{0.5f, 0.5f, -0.5f}},	1);
		addLine({	{0.5f, 0.5f, -0.5f},	{0.5f, 0.5f, 0.5f}},	1);
		addLine({	{0.5f, 0.5f, 0.5f},		{-0.5f, 0.5f, 0.5f}},	1);
		addLine({	{-0.5f, 0.5f, 0.5f},	{-0.5f, 0.5f, -0.5f}},	1);
		
		addLine({	{-0.5f, 0.5f, -0.5f},	{-0.5f, -0.5f, -0.5f}}, 1);
		addLine({	{0.5f, 0.5f, -0.5f},	{0.5f, -0.5f, -0.5f}},	1);
		addLine({	{0.5f, 0.5f, 0.5f},		{0.5f, -0.5f, 0.5f}},	1);
		addLine({	{-0.5f, 0.5f, 0.5f},	{-0.5f, -0.5f, 0.5f}},	1);
	}
};
