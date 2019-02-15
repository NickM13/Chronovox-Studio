#pragma once

#include "engine\gfx\texture\MTexture.h"

#include "engine\utils\LOpenGL.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"
#include "engine\utils\variable\manager\ColorManager.h"
#include "engine\utils\variable\manager\VoxelManager.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <math.h>
#include <map>

// Global implementation of GUI VBOs
class GBuffer {
protected:
	struct TextureBuffer {
		Sint32 textureId;
		Sint32 quadCount, lineCount;
		bool font;
		Rect scissor;
		TextureBuffer(Sint32 p_textureId, bool p_font, Rect p_scissor) {
			textureId = p_textureId;
			quadCount = lineCount = 0;
			font = p_font;
			scissor = p_scissor;
		};
	};
private:
	static Color m_currColor;
	static GLuint m_currTexture;
	static bool m_font;
	static glm::vec2 m_currUV;

	static bool m_scissorEnabled;
	static std::vector<Rect> m_scissorList;
	static Rect m_currScissor;
	
	static GLuint m_quadVaoId, m_quadVboId[3];

	// How many vertices go to current bound texture/scissor
	static std::vector<TextureBuffer> m_textureBuffers;

	static std::vector<glm::vec2> m_quadVertices;
	static std::vector<Color> m_quadColors;
	static std::vector<glm::vec2> m_quadUVs;

public:
	enum class TextureAnchor : Sint8 {
		NONE = 0,
		HALF = 1,
		FULL = 2
	};

	static void init();
	static void terminate();

	static void clean();
	static void rasterize();

	static void setTexture(GLuint p_textureId, bool p_font = false);
	static void setColor(Color p_color);
	static void setUV(GLfloat u, GLfloat v);

	static void renderTexture(Texture* p_texture, TextureAnchor p_xAnchor = TextureAnchor::NONE, TextureAnchor p_yAnchor = TextureAnchor::NONE);

	static void pushScissor(Rect p_scissor);
	static void popScissor();
	static void setScissorActive(bool p_isActive);
	static bool isScissorActive();

	static void addVertexQuad(Sint32 x, Sint32 y);

	static void renderMesh();
};
