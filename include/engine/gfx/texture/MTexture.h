#pragma once
#include "Texture.h"
#include <map>

// Texture manager
class MTexture {
private:
	static std::map<std::string, Texture*> m_textures;

	static Texture* loadTexture(std::string p_texturePath);
public:
	static void init();
	static void reload();
	static void terminate();
	static Texture* getTexture(std::string p_texturePath);
	static Texture* getTextureById(GLuint p_texId);
};
