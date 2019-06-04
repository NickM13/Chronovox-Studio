#include "engine\gfx\font\Font.h"
#include "engine\utils\Utilities.h"
#include "engine\gfx\gui\buffer\GBuffer.h"
#include "engine\gfx\shader\Shader.h"
#include "engine\utils\directory\LDirectory.h"

#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

std::map<std::string, Font::FontType*> Font::m_fontMap;
Font::FontType* Font::m_font = 0;

Alignment Font::m_alignment;

void Font::setAlignment(Alignment p_alignment) {
	m_alignment = p_alignment;
}

void Font::loadFont(std::string p_fontName, std::string p_src, Uint32 p_fontSize) {
	m_fontMap.insert({ p_fontName, init(p_src, p_fontSize) });
	m_fontMap[p_fontName]->m_fontName = p_fontName;
	m_font = m_fontMap[p_fontName];
}
void Font::setFont(std::string p_fontName) {
	m_font = m_fontMap[p_fontName];
}
Font::FontType* Font::init(std::string p_src, Uint32 p_fontSize) {
	std::string _src = LDirectory::getProjectPath() + "res\\font\\" + p_src;
	FontType* _font = new FontType();
	FT_Library library;
	if (FT_Init_FreeType(&library)) {
		Logger::logError("FT_Init_FreeType failed");
		return 0;
	}
	FT_Face face;
	if (FT_New_Face(library, _src.c_str(), 0, &face)) {
		Logger::logError("FT_New_Face failed: \"" + _src + "\"");
		return 0;
	}

	FT_Set_Pixel_Sizes(face, 0, p_fontSize);

	FT_Select_Charmap(face, FT_ENCODING_BIG5);

	FT_CharMap found = 0;
	FT_CharMap charmap;

	for (Sint32 n = 0; n < face->num_charmaps; n++) {
		charmap = face->charmaps[n];
		if (charmap->platform_id == 3 &&
			charmap->encoding_id == 1) {
			found = charmap;
			break;
		}
	}

	if (!found) {
		Logger::logError("FT_CharMap not found");
		return 0;
	}

	if (FT_Set_Charmap(face, found)) {
		Logger::logError("FT_Set_Charmap failed");
		return 0;
	}

	FT_GlyphSlot g = face->glyph;
	Sint32 w = 0;
	Sint32 h = 0;

	for (Sint32 i = 0; i < 256; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			Logger::logError("FT_Load_Char %c failed", i);
			continue;
		}

		w += g->bitmap.width;
		h = std::fmax(h, g->bitmap.rows);
	}
	_font->m_atlasWidth = w;
	_font->m_atlasHeight = h;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_font->m_texture);
	glBindTexture(GL_TEXTURE_2D, _font->m_texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Sint32 x = 0;

	for (Sint32 i = 0; i < 256; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			continue;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		_font->m_characters[i].advance.x = g->advance.x >> 6;
		_font->m_characters[i].advance.y = g->advance.y >> 6;

		_font->m_characters[i].size.x = g->bitmap.width;
		_font->m_characters[i].size.y = g->bitmap.rows;

		_font->m_characters[i].bearing.x = g->bitmap_left;
		_font->m_characters[i].bearing.y = g->bitmap_top;

		_font->m_characters[i].tx = (GLfloat)x / w;

		x += g->bitmap.width;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	_font->m_height = _font->m_characters['A'].size.y + 2;
	_font->m_spacing = _font->m_height * getSpacing();
	return _font;
}

void Font::clean() {
	for (auto font : m_fontMap) {
		glDeleteTextures(1, &font.second->m_texture);
		delete font.second;
	}
	m_fontMap.clear();
}

Sint16 Font::getHeight() {
	if (!m_font) return 0;
	return m_font->m_height;
}

GLfloat Font::getSpacingHeight() {
	if (!m_font) return 0;
	return m_font->m_height * 1.8f;
}

GLfloat Font::getSpacing() {
	return 1.8f;
}

Vector2<Sint32> Font::getMessageWidth(std::string p_msg) {
	if (!m_font) return {};
	Sint32 _y = getHeight();
	Sint32 _rVal = 0, _rMaxVal = 0;
	for (Sint32 i = 0; i < Sint32(p_msg.length()); i++) {
		if (p_msg[i] == '\n') {
			_y += getSpacingHeight();
			_rMaxVal = std::fmaxf(_rVal, _rMaxVal);
			_rVal = 0;
		}
		else {
			_rVal += m_font->m_characters[(Uint8)p_msg[i]].advance.x;
		}
	}
	_rMaxVal = std::fmaxf(_rVal, _rMaxVal);
	return Vector2<Sint32>(_rMaxVal, _y);
}

std::string Font::getMessageSubstr(std::string p_msg, Sint32 p_width) {
	if (!m_font) return p_msg;
	Sint32 _y = getHeight();
	Sint32 _rVal = 0;
	std::vector<Sint32> _msgLengths;
	for (Sint32 i = 0; i < Sint32(p_msg.length()); i++) {
		_rVal += m_font->m_characters[p_msg[i]].advance.x;
		_msgLengths.push_back(m_font->m_characters[p_msg[i]].advance.x);
		if (_rVal > p_width) {
			return p_msg.substr(0, i - 2) + "...";
		}
	}
	return p_msg;
}

void Font::print(std::string message, Sint32 px, Sint32 py) {
	if (!m_font) return;
	py += m_font->m_height / 2;

	std::vector<std::string> lines;
	lines.push_back(std::string());
	for (unsigned char c : message) {
		if (c == '\n') {
			lines.push_back(std::string());
		} else {
			lines.back() += c;
		}
	}

	Sint32 x = 0, y = 0;
	Sint32 offx = 0, offy = 0;
	Font::FontType::Character ch;
	
	GBuffer::setTexture(m_font->m_texture, true);
	for (Sint32 i = 0; i < (Sint32)lines.size(); i++) {
		switch (m_alignment) {
		case Alignment::ALIGN_CENTER:
			offx = Font::getMessageWidth(lines.at(i)).x / 2;
			break;
		case Alignment::ALIGN_RIGHT:
			offx = Font::getMessageWidth(lines.at(i)).x;
			break;
		default: break;
		}
		x = 0;
		for (unsigned char c : lines.at(i)) {
			ch = m_font->m_characters[c];

			GLfloat x2 = px + x + ch.bearing.x - offx;
			GLfloat y2 = py + y - ch.bearing.y + offy;
			GLfloat w = ch.size.x;
			GLfloat h = ch.size.y;

			// Advance cursor to start of next character
			x += ch.advance.x;
			y += ch.advance.y;

			// Skip empty glyphs
			if (!w || !h) {
				continue;
			}

			GBuffer::setUV(ch.tx,										0);
			GBuffer::addVertexQuad(x2,									y2);

			GBuffer::setUV(ch.tx + ch.size.x / m_font->m_atlasWidth,	0);
			GBuffer::addVertexQuad(x2 + w,								y2);

			GBuffer::setUV(ch.tx + ch.size.x / m_font->m_atlasWidth,	ch.size.y / m_font->m_atlasHeight);
			GBuffer::addVertexQuad(x2 + w,								y2 + h);

			GBuffer::setUV(ch.tx,										ch.size.y / m_font->m_atlasHeight);
			GBuffer::addVertexQuad(x2,									y2 + h);
		}
		offy += getSpacingHeight();
	}
}
