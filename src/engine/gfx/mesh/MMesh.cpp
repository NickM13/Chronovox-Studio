#include "engine\gfx\mesh\MMesh.h"
#include "engine\gfx\shader\Shader.h"
#include "engine\editor\camera\Camera.h"

GLuint MMesh::m_lineVao, MMesh::m_lineVbo[2];
glm::vec3 MMesh::m_lineVertices[2];

GLuint MMesh::m_boxVao, MMesh::m_boxVbo[2];
glm::vec3 MMesh::m_boxVertices[24];

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

	m_boxVertices[0]  = p000; m_boxVertices[1]  = p100;
	m_boxVertices[2]  = p010; m_boxVertices[3]  = p110;
	m_boxVertices[4]  = p011; m_boxVertices[5]  = p111;
	m_boxVertices[6]  = p001; m_boxVertices[7]  = p101;

	m_boxVertices[8]  = p000; m_boxVertices[9]  = p010;
	m_boxVertices[10] = p001; m_boxVertices[11] = p011;
	m_boxVertices[12] = p101; m_boxVertices[13] = p111;
	m_boxVertices[14] = p100; m_boxVertices[15] = p110;

	m_boxVertices[16] = p000; m_boxVertices[17] = p001;
	m_boxVertices[18] = p100; m_boxVertices[19] = p101;
	m_boxVertices[20] = p110; m_boxVertices[21] = p111;
	m_boxVertices[22] = p010; m_boxVertices[23] = p011;

	glGenVertexArrays(1, &m_boxVao);
	glBindVertexArray(m_boxVao);
	glGenBuffers(2, m_boxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_boxVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 24, &m_boxVertices[0].x, GL_STATIC_DRAW);
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

	glBindVertexArray(m_boxVao);
	glVertexAttrib4f(1, p_color.r, p_color.g, p_color.b, p_color.a);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, 24);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	Shader::popMatrixModel();
}
