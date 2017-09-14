#include "engine\gfx\gui\button\ButtonRadio.h"

CButtonRadio::CButtonRadio(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_buttonSize, Vector2<Sint32> p_buttonStep, Uint16* p_selectedButton)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_buttonSize = p_buttonSize;
	m_buttonStep = p_buttonStep;
	m_colorTheme = m_colorThemes[Theme::ACTION];
	m_selectedButton = p_selectedButton;
}

Component* CButtonRadio::addButton(Texture p_tex)
{
	CButtonToggle *_button = new CButtonToggle("", p_tex, m_buttonStep * m_buttonList.size(), m_buttonSize);
	_button->setBorder(0);
	m_buttonList.push_back(_button);
	if(m_buttonList.size() > *m_selectedButton)
		m_buttonList[*m_selectedButton]->setState(1);
	callPressFunction();
	return _button;
}

void CButtonRadio::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	if(p_interactFlags & EVENT_MOUSEOVER)
	{
		for(Uint16 i = 0; i < m_buttonList.size(); i++)
		{
			m_buttonList[i]->input(p_interactFlags, p_keyStates, p_mouseStates, p_mousePos);
			if(m_buttonList[i] != 0 && *m_buttonList[i]->getValue() != 0)
			{
				*m_selectedButton = i;
				callPressFunction();
				return;
			}
		}
	}
}
void CButtonRadio::update(GLfloat p_deltaUpdate)
{
	m_buttonList[m_prevSelectedButton]->setState(0);
	m_buttonList[*m_selectedButton]->setState(1);
	if(m_prevSelectedButton != *m_selectedButton)
	{
		callPressFunction();
		m_prevSelectedButton = *m_selectedButton;
	}
}
void CButtonRadio::render()
{
	glPushMatrix();
	{
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		for(Uint16 i = 0; i < m_buttonList.size(); i++)
			m_buttonList[i]->render();
	}
	glPopMatrix();
}

void CButtonRadio::setSelectedButton(Uint16 p_selectedButton)
{
	m_prevSelectedButton = *m_selectedButton;
	*m_selectedButton = p_selectedButton;
	callPressFunction();
}
