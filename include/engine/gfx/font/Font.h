#pragma once

#include "engine\gfx\texture\MTexture.h"

#include "engine\utils\Utilities.h"
#include "engine\utils\Singleton.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

enum Alignment
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2
};

class Font {
private:
	struct FontType
	{
		std::string m_fontName;
		Sint16 m_height;
		GLuint* m_textures;
		GLuint* m_charWidth;
		GLuint m_listBase;

		GLfloat m_spacing;
	};
	static std::vector<FontType*> m_fontList;
	static FontType* m_font;

	static Alignment m_alignment;

	static FontType* init(std::string p_src, Uint32 p_fontSize);
public:
	static void setAlignment(Alignment p_alignment);

	static void loadFont(std::string p_fontName, std::string p_src, Uint32 p_fontSize);
	static void setFont(std::string p_fontName);
	static void clean();

	static Sint16 getHeight() { return m_font->m_height; };
	static Vector2<Sint32> getMessageWidth(std::string p_msg);
	static std::string getMessageSubstr(std::string p_msg, Sint32 p_width);
	// Font Height * Spacing
	static GLfloat getSpacingHeight() { return m_font->m_height * 1.8f; };
	// Font Spacing, no height
	static GLfloat getSpacing() { return 1.8f; };

	static void print(std::string p_msg, Sint32 p_x, Sint32 p_y);
};
