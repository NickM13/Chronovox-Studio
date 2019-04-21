#include "engine\gfx\texture\MTexture.h"
#include "engine\utils\LOpenGL.h"
#include "engine\utils\directory\LDirectory.h"

#include <IL\il.h>
#include <IL\ilut.h>

std::map<std::string, Texture*> MTexture::m_textures;

Texture* MTexture::loadTexture(std::string p_texturePath) {
	std::string path = LDirectory::getProjectPath() + "res\\texture\\" + p_texturePath;
	ILuint imgId;
	ilGenImages(1, &imgId);
	ilBindImage(imgId);
	ilLoadImage(path.c_str());
	ILenum Error;
	Error = ilGetError();
	if(Error != IL_NO_ERROR) {
		Logger::logError("Could not load texture: \"" + path + "\"");
		return new Texture();
	}
	else {
		Logger::logNormal("Loaded texture: \"" + p_texturePath + "\"");
		return new Texture(p_texturePath, imgId, ilutGLBindTexImage(), Vector2<Sint32>(Sint32(ilGetInteger(IL_IMAGE_WIDTH)), Sint32(ilGetInteger(IL_IMAGE_HEIGHT))));
	}
}

void MTexture::init() {
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	//ilutRenderer(ILUT_ALLEGRO);
	//ilutRenderer(ILUT_WIN32); // For DirectX and Windows GDI
	m_textures.insert({"NULL", new Texture()});
	size_t rootLen = std::string(LDirectory::getProjectPath() + "res\\texture\\").length();
	for(std::string path : LDirectory::getFilesInDirectory(LDirectory::getProjectPath() + "res\\texture\\", ".png")) {
		m_textures.insert({ path.substr(rootLen), loadTexture(path.substr(rootLen)) });
	}
}

void MTexture::reload() {
	std::string path;
	for (std::pair<std::string, Texture*> t : m_textures) {
		if (t.first != "NULL") {
			path = LDirectory::getProjectPath() + "res\\texture\\" + t.second->getName();
			ilBindImage(t.second->getIlId());
			ilLoadImage(path.c_str());
			t.second->setTexture(ilutGLBindTexImage(), Vector2<Sint32>(Sint32(ilGetInteger(IL_IMAGE_WIDTH)), Sint32(ilGetInteger(IL_IMAGE_HEIGHT))));
		}
	}
}

void MTexture::terminate() {
	for(std::pair<std::string, Texture*> t : m_textures) {
		ilDeleteImage(t.second->getIlId());
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
		if(t.second->getGlId() == p_texId)
			return m_textures.at(t.first);
	Logger::logWarning("Texture ID not found: " + p_texId);
	return m_textures.at("NULL");
}

void MTexture::saveTexturePNG(std::string p_filePath, Texture* p_tex) {
	ilBindImage(p_tex->getIlId());
	ilSave(IL_PNG, p_filePath.c_str());
}
