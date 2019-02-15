#include "engine\gfx\gui\buffer\GBuffer.h"

#include "engine\gfx\shader\Shader.h"
#include "engine\editor\camera\Camera.h"

Color GBuffer::m_currColor = Color();
GLuint GBuffer::m_currTexture = 0;
bool GBuffer::m_font = false;
glm::vec2 GBuffer::m_currUV = {};

bool GBuffer::m_scissorEnabled = false;
std::vector<Rect> GBuffer::m_scissorList;
Rect GBuffer::m_currScissor = Rect();

GLuint GBuffer::m_quadVaoId, GBuffer::m_quadVboId[3];

std::vector<GBuffer::TextureBuffer> GBuffer::m_textureBuffers;

std::vector<glm::vec2> GBuffer::m_quadVertices;
std::vector<Color> GBuffer::m_quadColors;
std::vector<glm::vec2> GBuffer::m_quadUVs;

void GBuffer::init() {
	setTexture(0, false);

	glGenVertexArrays(1, &m_quadVaoId);
	glBindVertexArray(m_quadVaoId);
	glGenBuffers(3, m_quadVboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId[1]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GBuffer::terminate() {
	clean();
	glDeleteVertexArrays(1, &m_quadVaoId);
	glDeleteBuffers(3, m_quadVboId);
}

void GBuffer::clean() {
	m_quadVertices.clear();
	m_quadColors.clear();
	m_quadUVs.clear();
	m_textureBuffers.clear();
	setTexture(0, false);
}

void GBuffer::rasterize() {
	if (!m_quadVertices.empty()) {
		glBindVertexArray(m_quadVaoId);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId[0]);
		glBufferData(GL_ARRAY_BUFFER, m_quadVertices.size() * sizeof(GLfloat) * 2, &m_quadVertices[0].x, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId[1]);
		glBufferData(GL_ARRAY_BUFFER, m_quadColors.size() * sizeof(GLfloat) * 4, &m_quadColors[0].r, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId[2]);
		glBufferData(GL_ARRAY_BUFFER, m_quadUVs.size() * sizeof(GLfloat) * 2, &m_quadUVs[0].x, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void GBuffer::setTexture(GLuint p_textureId, bool p_font) {
	m_currTexture = p_textureId;
	m_font = p_font;
	if (!m_textureBuffers.empty()
		&& m_textureBuffers.back().quadCount == 0
		&& m_textureBuffers.back().lineCount == 0) {
		m_textureBuffers.back().textureId = p_textureId;
		m_textureBuffers.back().font = p_font;
		m_textureBuffers.back().scissor = m_currScissor;
	}
	else {
		m_textureBuffers.push_back(TextureBuffer(m_currTexture, p_font, m_currScissor));
	}
}

void GBuffer::setColor(Color p_color) {
	m_currColor = p_color;
}
void GBuffer::setUV(GLfloat u, GLfloat v) {
	m_currUV = glm::vec2(u, v);
}
void GBuffer::renderTexture(Texture* p_texture, TextureAnchor p_xAnchor, TextureAnchor p_yAnchor) {
	Shader::pushMatrixModel();
	switch (p_xAnchor) {
	case TextureAnchor::NONE: break;
	case TextureAnchor::HALF: Shader::translate(glm::vec3((GLfloat)-p_texture->getSize().x / 2, 0, 0));	break;
	case TextureAnchor::FULL: Shader::translate(glm::vec3((GLfloat)-p_texture->getSize().x, 0, 0));		break;
	}
	switch (p_yAnchor) {
	case TextureAnchor::NONE: break;
	case TextureAnchor::HALF: Shader::translate(glm::vec3(0, (GLfloat)-p_texture->getSize().y / 2, 0));	break;
	case TextureAnchor::FULL: Shader::translate(glm::vec3(0, (GLfloat)-p_texture->getSize().y, 0));		break;
	}
	setTexture(p_texture->getGlId());
	setUV(0, 0);	addVertexQuad(0, 0);
	setUV(1, 0);	addVertexQuad(p_texture->getSize().x, 0);
	setUV(1, 1);	addVertexQuad(p_texture->getSize().x, p_texture->getSize().y);
	setUV(0, 1);	addVertexQuad(0, p_texture->getSize().y);
	Shader::popMatrixModel();
}

void GBuffer::pushScissor(Rect p_scissor) {
	Rect _rect;
	glm::mat4 modelMatrix = Shader::getMatrixModel();

	if (m_scissorList.empty()) {
		_rect = p_scissor;
		_rect.x += modelMatrix[3][0];
		_rect.y = GScreen::m_screenSize.y - (p_scissor.y + modelMatrix[3][1] + p_scissor.h);
		m_scissorEnabled = true;
	}
	else {
		Rect cScissor = m_scissorList.back();
		_rect.x = GLfloat(std::fmaxf(GLint(modelMatrix[3][0] + p_scissor.x), cScissor.x));
		_rect.y = GLfloat(std::fmaxf(GLint(GScreen::m_screenSize.y - (p_scissor.y + modelMatrix[3][1] + p_scissor.h)), cScissor.y));
		if ((modelMatrix[3][0] + p_scissor.x) + p_scissor.w <= cScissor.x + cScissor.w) {
			_rect.w = p_scissor.w;
		}
		else {
			_rect.w = std::fmaxf(0, p_scissor.w - (((modelMatrix[3][0] + p_scissor.x) + p_scissor.w) - (cScissor.x + cScissor.w)));
		}
		if ((GScreen::m_screenSize.y - (p_scissor.y + modelMatrix[3][1] + p_scissor.h)) + p_scissor.h < cScissor.y + cScissor.h) {
			_rect.h = p_scissor.h;
		}
		else {
			_rect.h = std::fmaxf(0, p_scissor.h - (((GScreen::m_screenSize.y - (p_scissor.y + modelMatrix[3][1] + p_scissor.h)) + p_scissor.h) - (cScissor.y + cScissor.h)));
		}
	}
	m_currScissor = _rect;
	m_scissorList.push_back(_rect);
}
void GBuffer::popScissor() {
	if (!m_scissorList.empty()) {
		m_scissorList.pop_back();
		if (!m_scissorList.empty()) {
			m_scissorEnabled = true;
			m_currScissor = m_scissorList.back();
			return;
		}
	}
	m_scissorEnabled = false;
	m_currScissor = Rect();
}
void GBuffer::setScissorActive(bool p_isActive) {
	m_scissorEnabled = p_isActive && !m_scissorList.empty();
	if (m_scissorEnabled) {
		m_currScissor = m_scissorList.back();
	}
	else {
		m_currScissor = Rect();
	}
}
bool GBuffer::isScissorActive() {
	return m_scissorEnabled;
}

void GBuffer::addVertexQuad(Sint32 x, Sint32 y) {
	m_quadVertices.push_back(Shader::getMatrixModel() * glm::vec4((GLfloat)x, (GLfloat)y, 0.f, 1.f));
	m_quadColors.push_back(m_currColor);
	m_quadUVs.push_back(m_currUV);
	m_textureBuffers.back().quadCount++;
}

void GBuffer::renderMesh() {
	Sint32 quadIndex = 0, lineIndex = 0;
	for (TextureBuffer tb : m_textureBuffers) {
		if (tb.scissor.w > 1) {
			glEnable(GL_SCISSOR_TEST);
			glScissor((GLsizei)tb.scissor.x,
					  (GLsizei)tb.scissor.y,
					  (GLsizei)tb.scissor.w,
					  (GLsizei)tb.scissor.h);
		}
		else {
			glDisable(GL_SCISSOR_TEST);
		}
		Shader::setTexture(0, tb.textureId);
		glUniform4f(3, 1, 1, 1, 1);
		glUniform1i(5, tb.font ? 1 : 0);

		glBindVertexArray(m_quadVaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glDrawArrays(GL_QUADS, quadIndex, GLsizei(tb.quadCount));
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);

		quadIndex += tb.quadCount;
		lineIndex += tb.lineCount;
	}
	glDisable(GL_SCISSOR_TEST);
}
