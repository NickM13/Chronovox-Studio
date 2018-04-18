#include "engine\gfx\texture\MTexture.h"
#include "engine\utils\LOpenGL.h"
#include "engine\utils\directory\LDirectory.h"

#include <IL\il.h>
#include <IL\ilut.h>

std::map<std::string, Texture*> MTexture::m_textures;

void MTexture::loadTexture(std::string p_texturePath) {
	std::string path = LDirectory::getProjectPath().append("res\\texture\\").append(p_texturePath);
	ILuint imgId;
	ilGenImages(1, &imgId);
	ilBindImage(imgId);
	ilLoadImage(path.c_str());
	ILenum Error;
	Error = ilGetError();
	if(Error != IL_NO_ERROR) {
		std::cout << "Error loading texture " << p_texturePath << std::endl;
		m_textures.insert({p_texturePath, new Texture()});
	}
	else {
		m_textures.insert({p_texturePath, new Texture(p_texturePath, ilutGLBindTexImage(), Vector2<Sint32>(Sint32(ilGetInteger(IL_IMAGE_WIDTH)), Sint32(ilGetInteger(IL_IMAGE_HEIGHT))))});
	}
}

void MTexture::init() {
	ilInit();
	iluInit();
	ilutInit();
	m_textures.insert({"NULL", new Texture()});
	size_t rootLen = std::string(LDirectory::getProjectPath() + "res\\texture\\").length();
	for(std::string path : LDirectory::getFilesInDirectory(LDirectory::getProjectPath() + "res\\texture\\", ".png")) {
		loadTexture(path.substr(rootLen));
	}
}

void MTexture::terminate() {
	for(std::pair<std::string, Texture*> t : m_textures) {
		ilDeleteImage(t.second->getId());
		delete t.second;
	}
	m_textures.clear();
}

Texture* MTexture::getTexture(std::string p_texturePath) {
	if(m_textures.find(p_texturePath) == m_textures.end()) {
		std::cout << "Texture not found: " << p_texturePath << std::endl;
		return m_textures.at("NULL");
	}
	return m_textures.at(p_texturePath);
}

Texture* MTexture::getTextureById(Sint32 p_texId) {
	for(std::pair<std::string, Texture*> t : m_textures)
		if(t.second->getId() == p_texId)
			return m_textures.at(t.first);
	std::cout << "Texture id not found: " << p_texId << std::endl;
	return m_textures.at("NULL");
}
