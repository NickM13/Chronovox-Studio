#pragma once
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\LOpenGL.h"

#include <string>

class Texture {
private:
	GLuint m_ilTexId, m_glTexId;
	std::string m_name;
	Vector2<Sint32> m_size;

public:
	Texture(std::string p_name = "", GLuint p_ilTexId = 0, GLuint p_glTexId = 0, Vector2<Sint32> p_size = {}) {
		m_name = p_name;
		m_ilTexId = p_ilTexId;
		m_glTexId = p_glTexId;
		m_size = p_size;
	}
	~Texture() {
		glDeleteTextures(1, &m_glTexId);
	}
	void bind() {
		glBindTexture(GL_TEXTURE_2D, m_glTexId);
	}
	GLuint getIlId() const {
		return m_ilTexId;
	}
	GLuint getGlId() const {
		return m_glTexId;
	}
	std::string getName() const {
		return m_name;
	}
	Vector2<Sint32> getSize() const {
		return m_size;
	}
	void setTexture(GLuint p_glTexId, Vector2<Sint32> p_size) {
		m_glTexId = p_glTexId;
		m_size = p_size;
	}
	bool operator==(const Texture p_tex) const {
		return (m_name == p_tex.m_name);
	}
	bool operator<(const Texture p_tex) const {
		return (m_glTexId < p_tex.getGlId());
	}
};
