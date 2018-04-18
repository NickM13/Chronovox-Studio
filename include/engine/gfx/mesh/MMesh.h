#pragma once

#include "engine\utils\LOpenGL.h"
#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Macros.h"

#include "object\TMeshObject.h"
#include "object\MArrow.h"
#include "object\MCube.h"
#include "object\MPlane.h"
#include "object\MScale.h"
#include "object\MSCube.h"

#include <vector>

class MMesh {
protected:
	static std::vector<TMeshObject*> m_objectList;
public:
	static void init();
	static void terminate();
	static void render(std::string p_objectName, Vector3<GLfloat> p_pos, Vector3<GLfloat> p_size, Vector3<GLfloat> p_rotation, Color p_color);
};
