#pragma once
#include "Texture.h"
#include <map>

// Texture manager
class MTexture {
private:
	static std::map<std::string, Texture*> m_textures;

	static Texture* loadTexture(std::string p_texturePath, bool p_includeGlfw);
public:
	static void init();
	static void reload();
	static void terminate();
	static Texture* getTexture(std::string p_texturePath, bool p_includeGlfw = false);
	static Texture* getTextureExternal(std::string p_texturePath, bool p_includeGlfw = false);
	static Texture* getTextureById(GLuint p_texId);
	static void saveTexturePNG(const std::string& p_filename, const std::vector<unsigned char>& p_pixels, Uint32 p_width, Uint32 p_height);
};
