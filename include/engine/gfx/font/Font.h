#pragma once

#include "engine\gfx\texture\MTexture.h"

#include "engine\utils\Utilities.h"
#include "engine\utils\Singleton.h"

enum Alignment {
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2
};

class Font {
private:
	struct FontType {
		struct Character {
			glm::vec2 advance;		// Glyph advance
			glm::vec2 size = {};	// Glyph size
			glm::vec2 bearing = {}; // Top left glyph coordinates
			GLfloat tx;				// Texture x offset
		};
		std::string m_fontName = "";
		Character m_characters[256];
		Sint32 m_height = 0;
		Sint32 m_atlasWidth, m_atlasHeight;
		GLuint m_texture;
		//GLuint* m_textures;
		//GLuint* m_charWidth;
		//GLuint m_listBase;

		GLfloat m_spacing = 0;
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

	static Sint16 getHeight();
	static glm::ivec2 getCharSize(char c) { return m_font->m_characters[c].size; }
	static Vector2<Sint32> getMessageWidth(std::string p_msg);
	static std::string getMessageSubstr(std::string p_msg, Sint32 p_width);
	// Font Height * Spacing
	static GLfloat getSpacingHeight();
	// Font Spacing, no height
	static GLfloat getSpacing();

	static void print(std::string p_msg, Sint32 p_x, Sint32 p_y);
};
