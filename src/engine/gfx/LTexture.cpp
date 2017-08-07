#include "engine\gfx\LTexture.h"

#include "engine\utils\OpenGL.h"
#include "engine\utils\Utilities.h"

#include <IL\il.h>
#include <IL\ilut.h>

GLuint LTexture::m_imgId = 0;
bool LTexture::m_isInit = false;

void LTexture::init()
{
	if(!m_isInit)
	{
		ilInit();
		iluInit();
		ilutInit();
		m_isInit = true;
	}
}

GLuint LTexture::getPrevId()
{
	return m_imgId;
}

GLuint LTexture::getTextureId(std::string src)
{
	if(!m_isInit) return 0;
	std::string _src = "res\\texture\\" + src;
	ilGenImages(1, &m_imgId);
	ilBindImage(m_imgId);
	ilLoadImage(_src.c_str());
	Texture p_temp(src, 0, Vector2<Sint32>(Sint32(ilGetInteger(IL_IMAGE_WIDTH)), Sint32(ilGetInteger(IL_IMAGE_HEIGHT))));
	if(MTexture::getInstance().contains(p_temp))
	{
		ilDeleteImage(m_imgId);
		return MTexture::getInstance().getUnitID(p_temp);
	}
	else
	{
		ilutRenderer(ILUT_OPENGL);
		m_imgId = ilutGLBindTexImage();
		return MTexture::getInstance().addUnit(Texture(src, m_imgId, Vector2<Sint32>(Sint32(ilGetInteger(IL_IMAGE_WIDTH)), Sint32(ilGetInteger(IL_IMAGE_HEIGHT)))));
	}
}

Texture LTexture::getTexture(std::string src)
{
	if(!m_isInit) return Texture();
	std::string _src = "res\\texture\\" + src;
	ilGenImages(1, &m_imgId);
	ilBindImage(m_imgId);
	ilLoadImage(_src.c_str());
	Texture p_temp(src, 0, Vector2<Sint32>(Sint32(ilGetInteger(IL_IMAGE_WIDTH)), Sint32(ilGetInteger(IL_IMAGE_HEIGHT))));
	if(MTexture::getInstance().contains(p_temp))
	{
		ilDeleteImage(m_imgId);
		return p_temp;
	}
	else
	{
		ilutRenderer(ILUT_OPENGL);
		m_imgId = ilutGLBindTexImage();
		return Texture(src, m_imgId, Vector2<Sint32>(Sint32(ilGetInteger(IL_IMAGE_WIDTH)), Sint32(ilGetInteger(IL_IMAGE_HEIGHT))));
	}
}

void LTexture::freeTex(GLuint id)
{
	if(!m_isInit) return;
	if(id == -1)
	{
		id = m_imgId;
		m_imgId = 0;
	}
	ilDeleteImage(id);
}
