#pragma once
#include "engine\utils\OpenGL.h"
#include "engine\utils\variable\manager\TextureManager.h"

#include <string>

class LTexture
{
public:
	static void init();
	static GLuint getPrevId();
	static GLuint getTextureId(std::string src);
	static Texture getTexture(std::string src);
	static void freeTex(GLuint id = -1);
private:
	static bool m_isInit;
	static GLuint m_imgId;
};
