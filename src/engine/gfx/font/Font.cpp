#include "engine\gfx\font\Font.h"
#include "engine\utils\Utilities.h"

// Majority of this code is from NeHe's FreeType2 tutorial http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/

std::vector<Font::FontType*> Font::m_fontList;
Font::FontType* Font::m_font = 0;

Alignment Font::m_alignment;

//Finds first power of 2 >= int p_a
inline int nextPower2(int p_a)
{
	int _rval = 1;
	while(_rval < p_a) _rval *= 2;
	return _rval;
}

GLuint make_dList(FT_Face face, Uint8 ch, GLuint list_base, GLuint* tex_base)
{
	if(FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
		throw std::runtime_error("FT_Load_Glyph failed");

	FT_Glyph glyph;
	if(FT_Get_Glyph(face->glyph, &glyph))
		throw std::runtime_error("FT_Get_Glyph failed");

	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	int width = nextPower2(bitmap.width);
	int height = nextPower2(bitmap.rows);

	GLubyte* expanded_data = new GLubyte[2 * width * height];

	for(Uint32 j = 0; j < Uint32(height); j++) {
		for(Uint32 i = 0; i < Uint32(width); i++) {
			expanded_data[2 * (i + j * width)] = 255;
			expanded_data[2 * (i + j * width) + 1] =
				(i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
		}
	}

	glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

	delete[] expanded_data;

	glNewList(list_base + ch, GL_COMPILE);
	{
		glBindTexture(GL_TEXTURE_2D, tex_base[ch]);

		glPushMatrix();
		{
			glTranslatef(GLfloat(bitmap_glyph->left), 0, 0);
			glTranslatef(0, GLfloat(bitmap_glyph->top) - bitmap.rows, 0);
			
			GLfloat x = GLfloat(bitmap.width) / GLfloat(width),
				y = GLfloat(bitmap.rows) / GLfloat(height);

			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0); glVertex2f(0, GLfloat(bitmap.rows));
				glTexCoord2f(0, GLfloat(y)); glVertex2f(0, 0);
				glTexCoord2f(GLfloat(x), GLfloat(y)); glVertex2f(GLfloat(bitmap.width), 0);
				glTexCoord2f(GLfloat(x), 0); glVertex2f(GLfloat(bitmap.width), GLfloat(bitmap.rows));
			}
			glEnd();
		}
		glPopMatrix();
		;
		glTranslatef(GLfloat(face->glyph->advance.x >> 6), 0, 0);
	}
	glEndList();
	FT_Done_Glyph(glyph);
	return (face->glyph->advance.x >> 6);
}

void Font::setAlignment(Alignment ALIGN)
{
	m_alignment = ALIGN;
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
	_font->m_textures = new GLuint[255];
	_font->m_charWidth = new GLuint[255];
	_font->m_height = p_fontSize;
	FT_Library library;
	if(FT_Init_FreeType(&library)) {
		Logger::logError("FT_Init_FreeType failed");
		return 0;
	}
	FT_Face face;
	if(FT_New_Face(library, _src.c_str(), 0, &face)) {
		Logger::logError("FT_New_Face failed: \"" + _src + "\"");
		return 0;
	}
	FT_Set_Char_Size(face, p_fontSize << 6, p_fontSize << 6, 96, 96);
	_font->m_listBase = glGenLists(255);
	glGenTextures(255, _font->m_textures);
	for(Uint8 i = 0; i < 255; i++)
		_font->m_charWidth[i] = make_dList(face, i, _font->m_listBase, _font->m_textures);
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	Logger::logNormal("Loaded font: \"" + p_src + "\"");
	return _font;
}

void Font::clean()
{
	for(Uint16 i = 0; i < m_fontList.size(); i++)
	{
		glDeleteLists(m_fontList[i]->m_listBase, 255);
		glDeleteTextures(255, m_fontList[i]->m_textures);
		delete[] m_fontList[i]->m_textures;
		delete[] m_fontList[i]->m_charWidth;
		delete m_fontList[i];
	}
	m_fontList.clear();
}

Vector2<Sint32> Font::getMessageWidth(std::string p_msg)
{
	Sint32 _y = getHeight();
	Sint32 _rVal = 0, _rMaxVal = 0;
	for(Sint32 i = 0; i < Sint32(p_msg.length()); i++)
	{
		if(p_msg[i] == '\n')
		{
			_y += getSpacingHeight();
			_rMaxVal = std::fmaxf(_rVal, _rMaxVal);
			_rVal = 0;
		}
		else
			_rVal += m_font->m_charWidth[p_msg[i]];
	}
	_rMaxVal = std::fmaxf(_rVal, _rMaxVal);
	return Vector2<Sint32>(_rMaxVal, _y);
}

std::string Font::getMessageSubstr(std::string p_msg, Sint32 p_width) {
	Sint32 _y = getHeight();
	Sint32 _rVal = 0;
	std::vector<Sint32> _msgLengths;
	for(Sint32 i = 0; i < Sint32(p_msg.length()); i++) {
		_rVal += m_font->m_charWidth[p_msg[i]];
		_msgLengths.push_back(m_font->m_charWidth[p_msg[i]]);
		if(_rVal > p_width) {

			return p_msg.substr(0, i - 2) + "...";
		}
	}
	return p_msg;
}

inline void pushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(viewport[0], viewport[2], viewport[3], viewport[1]);
	}
	glPopAttrib();
}

inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void Font::print(std::string message, Sint32 x, Sint32 y) {
	x = x;
	y = y + m_font->m_height / 2;

	Sint32 _offset = 0;

	int id;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);

	pushScreenCoordinateMatrix();
	glPushMatrix();
	{
		glScalef(1, -1, 1);

		GLuint font = m_font->m_listBase;

		Sint32 h = Sint32(getSpacingHeight());
		char text[256];
		va_list ap;

		if(message == "")
			*text = 0;
		else {
			va_start(ap, message);
			vsprintf_s(text, message.c_str(), ap);
			va_end(ap);
		}

		const char *start_line = text;
		std::vector<std::string> lines;
		const char *c;
		for(c = text; *c; c++) {
			if(*c=='\n') {
				std::string line;
				for(const char *n = start_line; n < c; n++) line.append(1, *n);
				lines.push_back(line);
				start_line=c+1;
			}
		}
		if(start_line) {
			std::string line;
			for(const char *n = start_line; n < c; n++) line.append(1, *n);
			lines.push_back(line);
		}

		glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
		{
			glMatrixMode(GL_MODELVIEW);
			glEnable(GL_BLEND);
			glListBase(font);

			GLfloat modelview_matrix[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

			for(Sint32 i=0; i < Sint32(lines.size()); i++) {
				glPushMatrix();
				{
					glLoadIdentity();
					switch(m_alignment)
					{
					case ALIGN_LEFT:
						break;
					case ALIGN_CENTER:
						glTranslatef(-(getMessageWidth(message).x / 2), 0, 0);
						break;
					case ALIGN_RIGHT:
						glTranslatef(-(getMessageWidth(message).x), 0, 0);
						break;
					default:
						break;
					}
					glTranslatef(x, y + h * i, 0);
					glMultMatrixf(modelview_matrix);

					glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
				}
				glPopMatrix();
			}
		}
		glPopAttrib();
	}
	glPopMatrix();

	pop_projection_matrix();

	glBindTexture(GL_TEXTURE_2D, id);
}
