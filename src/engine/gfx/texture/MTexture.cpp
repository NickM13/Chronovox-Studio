#include "engine\gfx\texture\MTexture.h"
#include "engine\utils\LOpenGL.h"
#include "engine\utils\directory\LDirectory.h"

#include <lodepng.h>

std::map<std::string, Texture*> MTexture::m_textures;

Texture* MTexture::loadTexture(std::string p_texturePath) {
	std::string path = LDirectory::getProjectPath() + "res\\texture\\" + p_texturePath;
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, path.c_str());

	if (error != 0) {
		Logger::logError("LodePNG %i: ", lodepng_error_text(error));
		return new Texture();
	}

	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	Logger::logNormal("Loaded texture \"" + p_texturePath + "\"");
	return new Texture(p_texturePath, texId, Vector2<Sint32>(width, height), &image);
}

void MTexture::init() {
	m_textures.insert({"NULL", new Texture()});
	size_t rootLen = std::string(LDirectory::getProjectPath() + "res\\texture\\").length();
	for(std::string path : LDirectory::getFilesInDirectory(LDirectory::getProjectPath() + "res\\texture\\", ".png")) {
		m_textures.insert({ path.substr(rootLen), loadTexture(path.substr(rootLen)) });
	}
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

Texture* MTexture::getTexture(std::string p_texturePath) {
	if(m_textures.find(p_texturePath) == m_textures.end()) {
		Logger::logWarning("Texture file could not be found: \"" + p_texturePath + "\"");
		return m_textures.at("NULL");
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

void MTexture::saveTexturePNG(std::string p_filePath, Texture* p_tex) {
	//ilBindImage(p_tex->getIlId());
	//ilSave(IL_PNG, p_filePath.c_str());
}
