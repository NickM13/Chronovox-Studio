#pragma once

#include "engine\utils\Utilities.h"
#include "engine\editor\model\Model.h"

// Shadows for voxel models
class Shader {
private:
	GLfloat m_lightHeight;								// Light shaft spawn height
	GLfloat m_lightShaftFrequency;						// Distance between light shafts
	Vector3<GLfloat> m_lightDirection;					// Direction of light
	Vector2<Uint16> m_lightMapSize;						// Smallest size of current lightmap
	std::vector<std::vector<GLfloat>> m_lightMap;		// Distance between light shaft creation point and first contact of a surface
public:
	Shader();
	void createShadows(Model p_model);
	void render();
};
