#include "engine\gfx\mesh\MMesh.h"
#include "engine\gfx\shader\Shader.h"
#include "engine\editor\camera\Camera.h"

GLuint MMesh::m_lineVao, MMesh::m_lineVbo[2];
glm::vec3 MMesh::m_lineVertices[2];

GLuint MMesh::m_lineBoxVao, MMesh::m_lineBoxVbo[2];
glm::vec3 MMesh::m_lineBoxVertices[24];

GLuint MMesh::m_solidBoxVao, MMesh::m_solidBoxVbo[2];
glm::vec3 MMesh::m_solidBoxVertices[24];

void MMesh::init() {
	// Line Vertex Array Object Initialization
	glGenVertexArrays(1, &m_lineVao);
	glBindVertexArray(m_lineVao);
	glGenBuffers(2, m_lineVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2, NULL, GL_STREAM_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Box Vertex Array Object Initialization
	glm::vec3 p000 = glm::vec3(0,   0,   0),
			  p100 = glm::vec3(1.f, 0,   0),
			  p010 = glm::vec3(0,   1.f, 0),
			  p110 = glm::vec3(1.f, 1.f, 0),
			  p001 = glm::vec3(0,   0,   1.f),
			  p101 = glm::vec3(1.f, 0,   1.f),
			  p011 = glm::vec3(0,   1.f, 1.f),
			  p111 = glm::vec3(1.f, 1.f, 1.f);

	m_lineBoxVertices[0]  = p000; m_lineBoxVertices[1]  = p100;
	m_lineBoxVertices[2]  = p010; m_lineBoxVertices[3]  = p110;
	m_lineBoxVertices[4]  = p011; m_lineBoxVertices[5]  = p111;
	m_lineBoxVertices[6]  = p001; m_lineBoxVertices[7]  = p101;

	m_lineBoxVertices[8]  = p000; m_lineBoxVertices[9]  = p010;
	m_lineBoxVertices[10] = p001; m_lineBoxVertices[11] = p011;
	m_lineBoxVertices[12] = p101; m_lineBoxVertices[13] = p111;
	m_lineBoxVertices[14] = p100; m_lineBoxVertices[15] = p110;

	m_lineBoxVertices[16] = p000; m_lineBoxVertices[17] = p001;
	m_lineBoxVertices[18] = p100; m_lineBoxVertices[19] = p101;
	m_lineBoxVertices[20] = p110; m_lineBoxVertices[21] = p111;
	m_lineBoxVertices[22] = p010; m_lineBoxVertices[23] = p011;

	glGenVertexArrays(1, &m_lineBoxVao);
	glBindVertexArray(m_lineBoxVao);
	glGenBuffers(2, m_lineBoxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineBoxVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 24, &m_lineBoxVertices[0].x, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_solidBoxVertices[0] = p000; m_solidBoxVertices[3] = p100; m_solidBoxVertices[2] = p110; m_solidBoxVertices[1] = p010;
	m_solidBoxVertices[4] = p001; m_solidBoxVertices[5] = p101; m_solidBoxVertices[6] = p111; m_solidBoxVertices[7] = p011;

	m_solidBoxVertices[8] = p000; m_solidBoxVertices[11] = p001; m_solidBoxVertices[10] = p101; m_solidBoxVertices[9] = p100;
	m_solidBoxVertices[12] = p010; m_solidBoxVertices[13] = p011; m_solidBoxVertices[14] = p111; m_solidBoxVertices[15] = p110;
	
	m_solidBoxVertices[16] = p000; m_solidBoxVertices[17] = p001; m_solidBoxVertices[18] = p011; m_solidBoxVertices[19] = p010;
	m_solidBoxVertices[20] = p100; m_solidBoxVertices[23] = p101; m_solidBoxVertices[22] = p111; m_solidBoxVertices[21] = p110;

	glGenVertexArrays(1, &m_solidBoxVao);
	glBindVertexArray(m_solidBoxVao);
	glGenBuffers(2, m_solidBoxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_solidBoxVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 24, &m_solidBoxVertices[0].x, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void MMesh::terminate() {

}

void MMesh::renderLine(glm::vec3 p_pos1, glm::vec3 p_pos2) {
	renderLine(p_pos1, p_pos2, glm::vec4(1.f));
}

void MMesh::renderLine(glm::vec3 p_pos1, glm::vec3 p_pos2, glm::vec4 p_color) {
	m_lineVertices[0] = p_pos1;
	m_lineVertices[1] = p_pos2;
	glBindVertexArray(m_lineVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * 2, &m_lineVertices[0].x);
	glVertexAttrib4f(1, p_color.r, p_color.g, p_color.b, p_color.a);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void MMesh::renderBoxOutline(glm::vec3 p, glm::vec3 s, glm::vec4 p_color) {
	Shader::pushMatrixModel();
	Shader::transformModel(glm::translate(glm::mat4(), p));
	Shader::transformModel(glm::scale(glm::mat4(), s));
	Shader::applyModel();

	glBindVertexArray(m_lineBoxVao);
	glVertexAttrib4f(1, p_color.r, p_color.g, p_color.b, p_color.a);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, 24);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	Shader::popMatrixModel();
}

void MMesh::renderBoxSolid(glm::vec3 p, glm::vec3 s, glm::vec4 p_color) {
	Shader::pushMatrixModel();
	Shader::transformModel(glm::translate(glm::mat4(), p));
	Shader::transformModel(glm::scale(glm::mat4(), s));
	Shader::applyModel();

	glBindVertexArray(m_solidBoxVao);
	glVertexAttrib4f(1, p_color.r, p_color.g, p_color.b, p_color.a);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	Shader::popMatrixModel();
}
