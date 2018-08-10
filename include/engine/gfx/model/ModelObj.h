#pragma once
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\LOpenGL.h"

#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <glm\common.hpp>

// 3d model class
class ModelObj {
private:
	std::string m_name;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec4> m_colors;
	std::vector<glm::vec2> m_texCoords;
	std::vector<glm::vec3> m_normals;
	GLuint m_vao;
	GLuint m_vbo[4];
public:
	ModelObj(std::string p_name = "") 
		: m_name(p_name) {}
	~ModelObj() {}
	bool loadObjFile(std::string p_fileName);
	void renderObj();
	std::string getName() const					{ return m_name; }
	bool operator==(const ModelObj p_tex) const { return (m_name == p_tex.m_name); }
	bool operator<(const ModelObj p_tex) const	{ return (m_name < p_tex.getName()); }
};
