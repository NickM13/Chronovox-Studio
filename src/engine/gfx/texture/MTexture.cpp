#include "engine\gfx\texture\MTexture.h"
#include "engine\utils\LOpenGL.h"
#include "engine\utils\directory\LDirectory.h"

#include <lodepng.h>

std::map<std::string, Texture*> MTexture::m_textures;

Texture* MTexture::loadTexture(std::string p_texturePath, bool p_includeGlfw) {
	Texture* tex;
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, p_texturePath.c_str());

	if (error != 0) {
		Logger::logError("LodePNG %i: %s", lodepng_error_text(error), p_texturePath);
		return new Texture();
	}

	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	Logger::logNormal("Loaded texture \"%s\"", p_texturePath);
	if (p_includeGlfw)	tex = new Texture(p_texturePath, texId, Vector2<Sint32>(width, height), image);
	else				tex = new Texture(p_texturePath, texId, Vector2<Sint32>(width, height));
	image.clear();
	return tex;
}

void MTexture::init() {
	m_textures.insert({"NULL", new Texture()});
}

void MTexture::reload() {
	/*
	std::string path;
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error;
	for (std::pair<std::string, Texture*> t : m_textures) {
		if (t.first != "NULL") {
			path = LDirectory::getProjectPath() + "res\\texture\\" + t.second->getName();
			Logger::logQuiet(path);

			error = lodepng::decode(image, width, height, path.c_str());
			if (error != 0) {
				Logger::logError("LodePNG %i: ", lodepng_error_text(error));
				continue;
			}

			glBindTexture(GL_TEXTURE_2D, t.second->getTexId());

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

			t.second->setTexture(t.second->getTexId(), Vector2<Sint32>(width, height));
		}
	}
	*/
}

void MTexture::terminate() {
	for(std::pair<std::string, Texture*> t : m_textures) {
		delete t.second;
	}
	m_textures.clear();
}

Texture* MTexture::getTexture(std::string p_texturePath, bool p_includeGlfw) {
	if(m_textures.find(p_texturePath) == m_textures.end()) {
		Texture* tex = loadTexture(LDirectory::getProjectPath() + "res\\texture\\" + p_texturePath, p_includeGlfw);
		m_textures.insert({ p_texturePath, tex });
		//Logger::logWarning("Texture file could not be found: \"" + p_texturePath + "\"");
	}
	return m_textures.at(p_texturePath);
}
Texture* MTexture::getTextureExternal(std::string p_texturePath, bool p_includeGlfw) {
	if (m_textures.find(p_texturePath) == m_textures.end()) {
		m_textures.insert({ p_texturePath, loadTexture(p_texturePath, p_includeGlfw) });
		Logger::logWarning("Texture file could not be found: \"" + p_texturePath + "\"");
	}
	return m_textures.at(p_texturePath);
}

Texture* MTexture::getTextureById(GLuint p_texId) {
	for(std::pair<std::string, Texture*> t : m_textures)
		if(t.second->getTexId() == p_texId)
			return m_textures.at(t.first);
	Logger::logWarning("Texture ID not found: " + p_texId);
	return m_textures.at("NULL");
}

void MTexture::saveTexturePNG(const std::string& p_filename, const std::vector<unsigned char>& p_pixels, Uint32 p_width, Uint32 p_height) {
	lodepng::encode(p_filename, p_pixels, p_width, p_height);
}
