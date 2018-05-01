#include "engine\utils\variable\manager\ScissorManager.h"

void MScissor::toggle()
{
	if(glIsEnabled(GL_SCISSOR_TEST) || m_unitList.empty())
		glDisable(GL_SCISSOR_TEST);
	else if(!m_unitList.empty())
		glEnable(GL_SCISSOR_TEST);
}

Uint32 MScissor::push(Rect& p_area, bool p_override)
{
	Rect _rect;
	float _mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, _mat);
	bool _sci = (glIsEnabled(GL_SCISSOR_TEST) != 0);
	int _scissorBox[4];

	if(m_unitList.empty() || p_override)
	{
		_rect = p_area;
		_rect.x += _mat[12];
		_rect.y = GScreen::m_screenSize.y - (p_area.y + _mat[13] + p_area.h);
		glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glGetIntegerv(GL_SCISSOR_BOX, _scissorBox);
		_rect.x = GLfloat(std::fmaxf(GLint(_mat[12] + p_area.x), _scissorBox[0]));
		_rect.y = GLfloat(std::fmaxf(GLint(GScreen::m_screenSize.y - (p_area.y + _mat[13] + p_area.h)), _scissorBox[1]));
		if((_mat[12] + p_area.x) + p_area.w <= _scissorBox[0] + _scissorBox[2])
			_rect.w = p_area.w;
		else
			_rect.w = std::fmaxf(0, p_area.w - (((_mat[12] + p_area.x) + p_area.w) - (_scissorBox[0] + _scissorBox[2])));
		if((GScreen::m_screenSize.y - (p_area.y + _mat[13] + p_area.h)) + p_area.h < _scissorBox[1] + _scissorBox[3])
			_rect.h = p_area.h;
		else
			_rect.h = std::fmaxf(0, p_area.h - (((GScreen::m_screenSize.y - (p_area.y + _mat[13] + p_area.h)) + p_area.h) - (_scissorBox[1] + _scissorBox[3])));
	}
	glScissor(GLint(_rect.x), GLint(_rect.y), GLsizei(_rect.w), GLsizei(_rect.h));
	m_unitList.push_back(_rect);
	return Uint16(m_unitList.size());
}

#pragma warning (push)
#pragma warning (disable:4172)

Rect& MScissor::pop()
{
	if(!m_unitList.empty()) {
		m_unitList.pop_back();
		if(!m_unitList.empty()) {
			Rect _rect = m_unitList[m_unitList.size() - 1];
			glScissor(GLint(_rect.x), GLint(_rect.y), GLsizei(_rect.w), GLsizei(_rect.h));
			return _rect;
		}
	}
	glDisable(GL_SCISSOR_TEST);
	return Rect();
}

#pragma warning (pop)
