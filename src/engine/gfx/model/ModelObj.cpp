#include "engine\gfx\model\ModelObj.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\shader\Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <tiny_obj_loader.h>

bool ModelObj::loadObjFile(std::string p_fileName) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, p_fileName.c_str())) {
		throw std::runtime_error(err);
	}

	for(const auto& shape : shapes) {
		for(const auto& index : shape.mesh.indices) {
			m_vertices.push_back(glm::vec3(		attrib.vertices[3 * index.vertex_index + 0],
												attrib.vertices[3 * index.vertex_index + 1],
												attrib.vertices[3 * index.vertex_index + 2]));
			if(index.texcoord_index != -1)
				m_texCoords.push_back(glm::vec2(attrib.texcoords[2 * index.texcoord_index + 0],
												attrib.texcoords[2 * index.texcoord_index + 1]));
			else
				m_texCoords.push_back(glm::vec2(0, 0));
			m_normals.push_back(glm::vec3(	attrib.normals[3 * index.normal_index + 0],
											attrib.normals[3 * index.normal_index + 1],
											attrib.normals[3 * index.normal_index + 2]));
			m_colors.push_back(glm::vec4(	1.f, 1.f, 1.f, 1.f));
		}
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(4, m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), &m_vertices[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * m_colors.size(), &m_colors[0].r, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_texCoords.size(), &m_texCoords[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_normals.size(), &m_normals[0].r, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

void ModelObj::renderObj() {
	Shader::applyModel();
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}
