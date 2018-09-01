#include "engine\gfx\font\Font.h"
#include "engine\utils\Utilities.h"
#include "engine\gfx\gui\buffer\GBuffer.h"
#include "engine\gfx\shader\Shader.h"

std::vector<Font::FontType*> Font::m_fontList;
Font::FontType* Font::m_font = 0;

Alignment Font::m_alignment;

void Font::setAlignment(Alignment p_alignment) {
	m_alignment = p_alignment;
}

void Font::loadFont(std::string p_fontName, std::string p_src, Uint32 p_fontSize) {
	m_fontList.push_back(init(p_src, p_fontSize));
	m_fontList[m_fontList.size() - 1]->m_fontName = p_fontName;
	m_font = m_fontList[m_fontList.size() - 1];
}
void Font::setFont(std::string p_fontName) {
	for (Uint16 i = 0; i < m_fontList.size(); i++) {
		if (m_fontList[i]->m_fontName == p_fontName) {
			m_font = m_fontList[i];
		}
	}
}
Font::FontType* Font::init(std::string p_src, Uint32 p_fontSize) {
	char result[MAX_PATH];
	std::string path = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	path = path.substr(0, path.find_last_of('\\') + 1);
	std::string _src = path + p_src;
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

	GLuint texture;
	for (Uint8 c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			Logger::logError("FT_Load_Char failed: " + c);
			continue;
		}
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		FontType::Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		_font->m_characters.insert(std::pair<GLchar, FontType::Character>((GLchar)c, character));
	}
	_font->m_height = _font->m_characters.at('A').size.y + 2;
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	Logger::logNormal("Loaded font: \"" + p_src + "\"");
	return _font;
}

void Font::clean() {
	for (FontType* ft : m_fontList) {
		for (std::pair<char, FontType::Character> c : ft->m_characters) {
			glDeleteTextures(1, &c.second.textureId);
		}
		delete ft;
	}
	m_fontList.clear();
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
			_rVal += m_font->m_characters.at(p_msg[i]).advance >> 6;
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
		_rVal += m_font->m_characters.at(p_msg[i]).advance >> 6;
		_msgLengths.push_back(m_font->m_characters.at(p_msg[i]).advance >> 6);
		if (_rVal > p_width) {
			return p_msg.substr(0, i - 2) + "...";
		}
	}
	return p_msg;
}

void Font::print(std::string message, Sint32 x, Sint32 y) {
	if (!m_font) return;
	y += m_font->m_height / 2;

	Sint32 _offset = 0;

	Shader::pushMatrixModel();
	{
		Sint32 h = Sint32(getSpacingHeight());

		std::vector<std::string> lines;
		lines.push_back(std::string());
		for (Sint8 c : message) {
			if (c == '\n') {
				lines.push_back(std::string());
			}
			else {
				lines.back() += c;
			}
		}

		FontType::Character ch;
		glm::ivec2 tpos(0, 0);

		for (Sint32 i = 0; i < Sint32(lines.size()); i++) {
			Shader::pushMatrixModel();
			{
				glLoadIdentity();
				switch (m_alignment) {
				case ALIGN_LEFT:
					break;
				case ALIGN_CENTER:
					Shader::translate(glm::vec3((GLfloat)(-getMessageWidth(message).x / 2), 0.f, 0.f));
					break;
				case ALIGN_RIGHT:
					Shader::translate(glm::vec3((GLfloat)-getMessageWidth(message).x, 0.f, 0.f));
					break;
				default:
					break;
				}
				Shader::translate(glm::vec3((GLfloat)x, (GLfloat)y + h * i, 0.f));

				for (Sint8 c : lines[i]) {
					ch = m_font->m_characters.at(c);
					GBuffer::setTexture(ch.textureId, true);
					GBuffer::setUV(0, 0);	GBuffer::addVertexQuad(tpos.x + ch.bearing.x,				tpos.y - ch.bearing.y);
					GBuffer::setUV(1, 0);	GBuffer::addVertexQuad(tpos.x + ch.bearing.x + ch.size.x,	tpos.y - ch.bearing.y);
					GBuffer::setUV(1, 1);	GBuffer::addVertexQuad(tpos.x + ch.bearing.x + ch.size.x,	tpos.y - ch.bearing.y + ch.size.y);
					GBuffer::setUV(0, 1);	GBuffer::addVertexQuad(tpos.x + ch.bearing.x,				tpos.y - ch.bearing.y + ch.size.y);
					tpos.x += ch.advance >> 6;
				}
			}
			Shader::popMatrixModel();
		}
	}
	Shader::popMatrixModel();
}
