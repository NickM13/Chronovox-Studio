#pragma once
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\LOpenGL.h"

#include <string>
#include <vector>

class Texture {
private:
	GLuint m_texId;
	std::string m_name;
	Vector2<Sint32> m_size;
	GLFWimage* m_image;

public:
	Texture(std::string p_name = "", GLuint p_texId = 0, Vector2<Sint32> p_size = {}, std::vector<unsigned char>* pixels = 0) {
		m_name = p_name;
		m_texId = p_texId;
		m_size = p_size;
		m_image = new GLFWimage();
		m_image->width = p_size.x;
		m_image->height = p_size.y;
		if (pixels != 0) {
			m_image->pixels = new unsigned char[pixels->size()];
			for (size_t i = 0; i < pixels->size(); i++) {
				m_image->pixels[i] = pixels->at(i);
			}
		}
		else {
			m_image->pixels = 0;
		}
	}
	~Texture() {
		glDeleteTextures(1, &m_texId);
	}
	void bind() {
		glBindTexture(GL_TEXTURE_2D, m_texId);
	}
	GLuint getTexId() const {
		return m_texId;
	}
	std::string getName() const {
		return m_name;
	}
	Vector2<Sint32> getSize() const {
		return m_size;
	}
	void setTexture(GLuint p_texId, Vector2<Sint32> p_size) {
		m_texId = p_texId;
		m_size = p_size;
	}
	void setGlfwImage(GLFWimage* p_image) {
		m_image = p_image;
	}
	GLFWimage* getGlfwImage() const {
		return m_image;
	}
	bool operator==(const Texture p_tex) const {
		return (m_name == p_tex.m_name);
	}
	bool operator<(const Texture p_tex) const {
		return (m_texId < p_tex.getTexId());
	}
};
