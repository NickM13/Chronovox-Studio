#include "engine\gfx\gui\buffer\GBuffer.h"

#include "engine\gfx\shader\Shader.h"
#include "engine\editor\camera\Camera.h"

Color GBuffer::m_currColor = Color();
GLuint GBuffer::m_currTexture = 0;
bool GBuffer::m_font = false;
glm::vec2 GBuffer::m_currUV = {};
glm::vec4 GBuffer::m_currSubtexUV = {};
Texture* GBuffer::m_texShadow = 0;

bool GBuffer::m_scissorEnabled = false;
std::vector<Rect> GBuffer::m_scissorList;
Rect GBuffer::m_currScissor = Rect();

GLuint GBuffer::m_quadVaoId, GBuffer::m_quadVboId[4];

std::vector<GBuffer::TextureBuffer> GBuffer::m_textureBuffers;

std::vector<glm::vec2> GBuffer::m_quadVertices;
std::vector<Color> GBuffer::m_quadColors;
std::vector<glm::vec2> GBuffer::m_quadUVs;

void GBuffer::init() {
	m_texShadow = MTexture::getTexture("gui\\UIShadow.png");
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
	glDeleteBuffers(4, m_quadVboId);
}

void GBuffer::clean() {
	m_currSubtexUV = glm::vec4(0, 0, 1, 1);
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
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId[1]);
		glBufferData(GL_ARRAY_BUFFER, m_quadColors.size() * sizeof(GLfloat) * 4, &m_quadColors[0].r, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVboId[2]);
		glBufferData(GL_ARRAY_BUFFER, m_quadUVs.size() * sizeof(GLfloat) * 2, &m_quadUVs[0].x, GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void GBuffer::setTexture(GLuint p_textureId, bool p_font) {
	m_currTexture = p_textureId;
	m_font = p_font;
	// Check if the back texture buffer is empty to prevent unused buffers
	if (!m_textureBuffers.empty()
		&& m_textureBuffers.back().quadCount == 0
		&& m_textureBuffers.back().lineCount == 0) {
		m_textureBuffers.back().textureId = p_textureId;
		m_textureBuffers.back().font = p_font;
		m_textureBuffers.back().scissor = m_currScissor;
	}
	else {
		m_textureBuffers.push_back(TextureBuffer(m_currTexture, m_currSubtexUV, m_font, m_currScissor));
	}
}

void GBuffer::setColor(Color p_color) {
	m_currColor = p_color;
}
void GBuffer::setUV(GLfloat u, GLfloat v) {
	// Flips vertically after switching from 
	// DevIL to lodpng, way too many calls 
	// to this function to fix
	m_currUV = glm::vec2(u, v);
}
void GBuffer::setSubtexUV(GLfloat s, GLfloat t, GLfloat p, GLfloat q) {
	m_currSubtexUV = glm::vec4(s, t, p, q);
	// Check if the back texture buffer is empty to prevent unused buffers
	if (!m_textureBuffers.empty()
		&& m_textureBuffers.back().quadCount == 0
		&& m_textureBuffers.back().lineCount == 0) {
		m_textureBuffers.back().subtexCoords = m_currSubtexUV;
		m_textureBuffers.back().scissor = m_currScissor;
	} else {
		m_textureBuffers.push_back(TextureBuffer(m_currTexture, m_currSubtexUV, m_font, m_currScissor));
	}
}
void GBuffer::renderTexture(Texture* p_texture, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, TextureStyle p_texStyle) {
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(p_pos.x, p_pos.y, 0));
	setColor(Color(1.f, 1.f, 1.f, 1.f));
	Vector2<Sint32> _texSize;
	if (p_texture != 0) {
		setTexture(p_texture->getTexId());
		_texSize = p_texture->getSize();
	}
	GLfloat _height, _width;
	switch (p_texStyle) {
	case TextureStyle::NONE:
	case TextureStyle::STRETCH:
		setUV(0, 0); addVertexQuad(0, 0);
		setUV(1, 0); addVertexQuad((GLfloat)p_size.x, 0);
		setUV(1, 1); addVertexQuad((GLfloat)p_size.x, (GLfloat)p_size.y);
		setUV(0, 1); addVertexQuad(0, (GLfloat)p_size.y);
		break;
	case TextureStyle::WRAP:
		setUV(0, 0);															addVertexQuad(0, 0);
		setUV((GLfloat)p_size.x / _texSize.x, 0);								addVertexQuad((GLfloat)p_size.x, 0);
		setUV((GLfloat)p_size.x / _texSize.x, (GLfloat)p_size.y / _texSize.y);	addVertexQuad((GLfloat)p_size.x, (GLfloat)p_size.y);
		setUV(0, (GLfloat)p_size.y / _texSize.y);								addVertexQuad(0, (GLfloat)p_size.y);
		break;
	case TextureStyle::SCALE:
		// Top Left corner
		setSubtexUV(0, 0, 0.25f, 0.25f);
		setUV(0, 0);		addVertexQuad(0, 0);
		setUV(1, 0);		addVertexQuad(_texSize.x / 4.f, 0);
		setUV(1, 1);		addVertexQuad(_texSize.x / 4.f, _texSize.y / 4.f);
		setUV(0, 1);		addVertexQuad(0, _texSize.y / 4.f);

		// Top Right corner
		setSubtexUV(0.75f, 0, 1, 0.25f);
		setUV(0, 0);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, 0);
		setUV(1, 0);		addVertexQuad((GLfloat)p_size.x, 0);
		setUV(1, 1);		addVertexQuad((GLfloat)p_size.x, _texSize.y / 4.f);
		setUV(0, 1);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, _texSize.y / 4.f);

		// Bottom Left corner
		setSubtexUV(0, 0.75f, 0.25f, 1);
		setUV(0, 0);		addVertexQuad(0, (GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(1, 0);		addVertexQuad(_texSize.x / 4.f, (GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(1, 1);		addVertexQuad(_texSize.x / 4.f, (GLfloat)p_size.y);
		setUV(0, 1);		addVertexQuad(0, (GLfloat)p_size.y);

		// Bottom Right corner
		setSubtexUV(0.75f, 0.75f, 1, 1);
		setUV(0, 0);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, (GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(1, 0);		addVertexQuad((GLfloat)p_size.x, (GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(1, 1);		addVertexQuad((GLfloat)p_size.x, (GLfloat)p_size.y);
		setUV(0, 1);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, (GLfloat)p_size.y);

		_width = ((GLfloat)p_size.x - _texSize.x / 2.f) / (_texSize.x / 2.f);
		_height = (((GLfloat)p_size.y - _texSize.y / 2.f) / (_texSize.y / 2.f));
		// Top side
		setSubtexUV(0.25f, 0, 0.75f, 0.25f);
		setUV(0, 0);			addVertexQuad(_texSize.x / 4.f,						0);
		setUV(_width, 0);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, 0);
		setUV(_width, 1);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, _texSize.y / 4.f);
		setUV(0, 1);			addVertexQuad(_texSize.x / 4.f,						_texSize.y / 4.f);

		// Bottom side
		setSubtexUV(0.25f, 0.75f, 0.75f, 1);
		setUV(0, 0);			addVertexQuad(_texSize.x / 4.f,						(GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(_width, 0);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, (GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(_width, 1);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, (GLfloat)p_size.y);
		setUV(0, 1);			addVertexQuad(_texSize.x / 4.f,						(GLfloat)p_size.y);

		// Left side
		setSubtexUV(0, 0.25f, 0.25f, 0.75f);
		setUV(0, 0);			addVertexQuad(0,									_texSize.y / 4.f);
		setUV(1, 0);			addVertexQuad(_texSize.x / 4.f,						_texSize.y / 4.f);
		setUV(1, _height);		addVertexQuad(_texSize.x / 4.f,						(GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(0, _height);		addVertexQuad(0,									(GLfloat)p_size.y - _texSize.y / 4.f);

		// Right side
		setSubtexUV(0.75f, 0.25f, 1, 0.75f);
		setUV(0, 0);			addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f,	_texSize.y / 4.f);
		setUV(1, 0);			addVertexQuad((GLfloat)p_size.x,					_texSize.y / 4.f);
		setUV(1, _height);		addVertexQuad((GLfloat)p_size.x,					(GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(0, _height);		addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f,	(GLfloat)p_size.y - _texSize.y / 4.f);

		setSubtexUV(0, 0, 1, 1);

		/*
		_width = floor(((GLfloat)p_size.x - _texSize.x / 2.f) / (_texSize.x / 2.f));
		_widthF = (((GLfloat)p_size.x - _texSize.x / 2.f) / (_texSize.x / 2.f));

		// Top and Bottom side
		for (Uint16 i = 0; i < _width; i++) {
			setUV(0.25f, 1);		addVertexQuad(_texSize.x / 4.f + (i * _texSize.x / 2.f), 0);
			setUV(0.75f, 1);		addVertexQuad(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), 0);
			setUV(0.75f, 0.75f);	addVertexQuad(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), _texSize.y / 4.f);
			setUV(0.25f, 0.75f);	addVertexQuad(_texSize.x / 4.f + (i * _texSize.x / 2.f), _texSize.y / 4.f);

			setUV(0.25f, 0.25f);	addVertexQuad(_texSize.x / 4.f + (i * _texSize.x / 2.f), (GLfloat)p_size.y - _texSize.y / 4.f);
			setUV(0.75f, 0.25f);	addVertexQuad(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), (GLfloat)p_size.y - _texSize.y / 4.f);
			setUV(0.75f, 0);		addVertexQuad(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), (GLfloat)p_size.y);
			setUV(0.25f, 0);		addVertexQuad(_texSize.x / 4.f + (i * _texSize.x / 2.f), (GLfloat)p_size.y);
		}
		setUV(0.25f, 1);									addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), 0);
		setUV(0.25f + 0.5f * (_widthF - _width), 1);		addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), 0);
		setUV(0.25f + 0.5f * (_widthF - _width), 0.75f);	addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f);
		setUV(0.25f, 0.75f);								addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f);

		setUV(0.25f, 0.25f);								addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), (GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(0.25f + 0.5f * (_widthF - _width), 0.25f);	addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), (GLfloat)p_size.y - _texSize.y / 4.f);
		setUV(0.25f + 0.5f * (_widthF - _width), 0);		addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), (GLfloat)p_size.y);
		setUV(0.25f, 0);									addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), (GLfloat)p_size.y);

		_height = floor(((GLfloat)p_size.y - _texSize.y / 2.f) / (_texSize.y / 2.f));
		_heightF = (((GLfloat)p_size.y - _texSize.y / 2.f) / (_texSize.y / 2.f));

		// Left and Right side
		for (Uint16 i = 0; i < _height; i++) {
			setUV(0, 0.75f);		addVertexQuad(0, _texSize.y / 4.f + (i * _texSize.y / 2.f));
			setUV(0, 0.25f);		addVertexQuad(0, (_texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f)));
			setUV(0.25f, 0.25f);	addVertexQuad(_texSize.x / 4.f, (_texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f)));
			setUV(0.25f, 0.75f);	addVertexQuad(_texSize.x / 4.f, _texSize.y / 4.f + (i * _texSize.y / 2.f));

			setUV(0.75f, 0.75f);	addVertexQuad((GLfloat)p_size.x - _texSize.y / 4.f, _texSize.y / 4.f + (i * _texSize.y / 2.f));
			setUV(0.75f, 0.25f);	addVertexQuad((GLfloat)p_size.x - _texSize.y / 4.f, _texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f));
			setUV(1, 0.25f);		addVertexQuad((GLfloat)p_size.x, _texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f));
			setUV(1, 0.75f);		addVertexQuad((GLfloat)p_size.x, _texSize.y / 4.f + (i * _texSize.y / 2.f));
		}
		setUV(0, 0.75f);									addVertexQuad(0, _texSize.y / 4.f + (_height * _texSize.y / 2.f));
		setUV(0, 0.75f - 0.5f * (_heightF - _height));		addVertexQuad(0, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
		setUV(0.25f, 0.75f - 0.5f * (_heightF - _height));	addVertexQuad(_texSize.x / 4.f, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
		setUV(0.25f, 0.75f);								addVertexQuad(_texSize.x / 4.f, _texSize.y / 4.f + (_height * _texSize.y / 2.f));

		setUV(0.75f, 0.75f);								addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, _texSize.y / 4.f + (_height * _texSize.y / 2.f));
		setUV(0.75f, 0.75f - 0.5f * (_heightF - _height));	addVertexQuad((GLfloat)p_size.x - _texSize.x / 4.f, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
		setUV(1, 0.75f - 0.5f * (_heightF - _height));		addVertexQuad((GLfloat)p_size.x, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
		setUV(1, 0.75f);									addVertexQuad((GLfloat)p_size.x, _texSize.y / 4.f + (_height * _texSize.y / 2.f));

		// Center
		for (Uint16 x = 0; x < _width; x++) {
			for (Uint16 y = 0; y < _height; y++) {
				setUV(0.25f, 0.75f);	addVertexQuad(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
				setUV(0.75f, 0.75f);	addVertexQuad(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
				setUV(0.75f, 0.25f);	addVertexQuad(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
				setUV(0.25f, 0.25f);	addVertexQuad(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
			}
		}
		for (Uint16 x = 0; x < _width; x++) {
			setUV(0.25f, 0.75f);								addVertexQuad(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
			setUV(0.75f, 0.75f);								addVertexQuad(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
			setUV(0.75f, 0.75f - 0.5f * (_heightF - _height));	addVertexQuad(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
			setUV(0.25f, 0.75f - 0.5f * (_heightF - _height));	addVertexQuad(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
		}
		for (Uint16 y = 0; y < _height; y++) {
			setUV(0.25f, 0.75f);								addVertexQuad(_texSize.x / 4.f + (_width * _texSize.y / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
			setUV(0.25f + 0.5f * (_widthF - _width), 0.75f);	addVertexQuad(_texSize.x / 4.f + (_width * _texSize.y / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
			setUV(0.25f + 0.5f * (_widthF - _width), 0.25f);	addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.y / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
			setUV(0.25f, 0.25f);								addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.y / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
		}
		setUV(0.25f, 0.75f);															addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
		setUV(0.25f + 0.5f * (_widthF - _width), 0.75f);								addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
		setUV(0.25f + 0.5f * (_widthF - _width), 0.75f - 0.5f * (_heightF - _height));	addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
		setUV(0.25f, 0.75f - 0.5f * (_heightF - _height));								addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
		*/

		break;
	case TextureStyle::CENTERED:
	default:
		setUV(0, 0); addVertexQuad(std::floor(_texSize.x / -2.f) + p_size.x / 2, std::floor(_texSize.y / -2.f) + p_size.y / 2);
		setUV(1, 0); addVertexQuad(std::floor(_texSize.x / 2.f) + p_size.x / 2, std::floor(_texSize.y / -2.f) + p_size.y / 2);
		setUV(1, 1); addVertexQuad(std::floor(_texSize.x / 2.f) + p_size.x / 2, std::floor(_texSize.y / 2.f) + p_size.y / 2);
		setUV(0, 1); addVertexQuad(std::floor(_texSize.x / -2.f) + p_size.x / 2, std::floor(_texSize.y / 2.f) + p_size.y / 2);
		break;
	}
	Shader::popMatrixModel();
}
void GBuffer::renderShadow(Vector2<Sint32> p_pos, Vector2<Sint32> p_size) {
	renderTexture(m_texShadow, p_pos - m_texShadow->getSize() / 8, p_size + m_texShadow->getSize() / 4, TextureStyle::SCALE);
}
void GBuffer::addQuadFilled(Vector2<Sint32> p_pos, Vector2<Sint32> p_size) {
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(p_pos.x, p_pos.y, 0));
	addVertexQuad(0, 0);
	addVertexQuad(p_size.x, 0);
	addVertexQuad(p_size.x, p_size.y);
	addVertexQuad(0, p_size.y);
	Shader::popMatrixModel();
}
void GBuffer::addQuadOutlined(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32 b) {
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(p_pos.x, p_pos.y, 0));
	addVertexQuad(0, 0);
	addVertexQuad(p_size.x, 0);
	addVertexQuad(p_size.x, b);
	addVertexQuad(0, b);

	addVertexQuad(0, p_size.y);
	addVertexQuad(p_size.x, p_size.y);
	addVertexQuad(p_size.x, p_size.y - b);
	addVertexQuad(0, p_size.y - b);

	addVertexQuad(0, 0);
	addVertexQuad(b, 0);
	addVertexQuad(b, p_size.y);
	addVertexQuad(0, p_size.y);

	addVertexQuad(p_size.x, 0);
	addVertexQuad(p_size.x - b, 0);
	addVertexQuad(p_size.x - b, p_size.y);
	addVertexQuad(p_size.x, p_size.y);
	Shader::popMatrixModel();
}

void GBuffer::pushScissor(Rect p_scissor) {
	Rect _rect;
	glm::mat4 modelMatrix = Shader::getMatrixModel();

	if (m_scissorList.empty()) {
		_rect = p_scissor;
		_rect.x += modelMatrix[3][0];
		_rect.y = GScreen::getScreenSize().y - (p_scissor.y + modelMatrix[3][1] + p_scissor.h);
		m_scissorEnabled = true;
	}
	else {
		Rect cScissor = m_scissorList.back();
		_rect.x = GLfloat(std::fmaxf(GLint(modelMatrix[3][0] + p_scissor.x), cScissor.x));
		_rect.y = GLfloat(std::fmaxf(GLint(GScreen::getScreenSize().y - (p_scissor.y + modelMatrix[3][1] + p_scissor.h)), cScissor.y));
		if ((modelMatrix[3][0] + p_scissor.x) + p_scissor.w <= cScissor.x + cScissor.w) {
			_rect.w = p_scissor.w;
		}
		else {
			_rect.w = std::fmaxf(0, p_scissor.w - (((modelMatrix[3][0] + p_scissor.x) + p_scissor.w) - (cScissor.x + cScissor.w)));
		}
		if ((GScreen::getScreenSize().y - (p_scissor.y + modelMatrix[3][1] + p_scissor.h)) + p_scissor.h < cScissor.y + cScissor.h) {
			_rect.h = p_scissor.h;
		}
		else {
			_rect.h = std::fmaxf(0, p_scissor.h - (((GScreen::getScreenSize().y - (p_scissor.y + modelMatrix[3][1] + p_scissor.h)) + p_scissor.h) - (cScissor.y + cScissor.h)));
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
	Shader::setVec4("colorScalar", 1, 1, 1, 1);
	for (TextureBuffer tb : m_textureBuffers) {
		Shader::setTexture(0, tb.textureId);
		Shader::setBool("isFont", tb.font);
		Shader::setVec4("scissor", tb.scissor.x, tb.scissor.y, tb.scissor.w, tb.scissor.h);
		Shader::setTextureCoords(tb.subtexCoords);

		glBindVertexArray(m_quadVaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glDrawArrays(GL_QUADS, quadIndex, GLsizei(tb.quadCount));
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);

		quadIndex += tb.quadCount;
		lineIndex += tb.lineCount;
	}
}
