#include "engine\gfx\shader\Shader.h"

Shader::Shader() {
	m_lightHeight = 100.f;
	m_lightShaftFrequency = 0.25f;
	m_lightDirection = Vector3<GLfloat>(1, -1, 1).getNormal();
}

void Shader::createShadows(Model p_model) {
	m_lightMapSize = Vector2<Uint16>(p_model.getSize().x, p_model.getSize().z);
	for(Uint16 x = 0; x < p_model.getSize().x / m_lightShaftFrequency; x++) {
		m_lightMap.push_back(std::vector<GLfloat>());
		for(Uint16 z = 0; z < p_model.getSize().z / m_lightShaftFrequency; z++) {
			m_lightMap[x].push_back(0);
			
		}
	}

}

void Shader::render() {

}
