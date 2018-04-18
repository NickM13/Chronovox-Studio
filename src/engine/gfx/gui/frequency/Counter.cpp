#include "engine\gfx\gui\frequency\Counter.h"

CCounter::CCounter(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32& p_currValue)
	: Component(p_compName, p_title, p_pos, p_size, Theme::MENUBAR)
{
	m_buttonAdd = new CButton("", "+", Vector2<Sint32>(p_pos.x + m_title.length() * 16 + 20, p_pos.y), Vector2<Sint32>(16, 16), CButton::RenderStyle::ALL);
	m_buttonSubtract = new CButton("", "-", Vector2<Sint32>(p_pos.x + m_title.length() * 16, p_pos.y), Vector2<Sint32>(16, 16), CButton::RenderStyle::ALL);
}
CCounter::~CCounter()
{
	delete m_buttonSubtract;
	delete m_buttonAdd;
	Component::~Component();
}

void CCounter::input(Sint8& p_interactFlags)
{
	/*
	TODO: Fix this
	m_buttonAdd->input(p_interactFlags, p_keyStates, p_mouseStates, _mousePos);
	m_buttonSubtract->input(p_interactFlags, p_keyStates, p_mouseStates, _mousePos);
	if(p_keyStates[GLFW_KEY_LEFT_SHIFT] & GKey::KEY_HOLD || p_keyStates[GLFW_KEY_RIGHT_SHIFT] & GKey::KEY_HOLD)
	{
		if(m_buttonAdd->isSelected() != 0)
			*m_numValue++;
		if(m_buttonSubtract->isSelected() != 0)
			*m_numValue--;
	}
	else if(GKey::m_specStates[GLFW_MOD_CONTROL] & GKey::KEY_HOLD || p_keyStates[GLFW_KEY_RIGHT_CONTROL] & GKey::KEY_HOLD)
	{
		if(m_buttonAdd->isSelected() != 0)
			*m_numValue+=5;
		if(m_buttonSubtract->isSelected() != 0)
			*m_numValue-=5;
	}
	else
	{
		if(m_buttonAdd->isSelected() == 1)
			*m_numValue++;
		if(m_buttonSubtract->isSelected() == 1)
			*m_numValue--;
	}

	if(*m_numValue < m_size.x)
		*m_numValue = m_size.x;
	if(*m_numValue > m_size.y)
		*m_numValue = m_size.y;
	*/
}
void CCounter::update(GLfloat p_deltaUpdate)
{

}

void CCounter::render()
{
	glColor3f(1, 1, 1);
	Font::setAlignment(ALIGN_LEFT);
	Font::print(m_title, m_pos.x, m_pos.y);
	//Font::print(Util::numToString(*m_numValue, 0), m_pos.x + m_title.length() * 16 + 48, m_pos.y);

	m_buttonSubtract->render();
	m_buttonAdd->render();
}

void CCounter::setPos(Vector2<Sint32> p_pos)
{
	m_pos = p_pos;
}
Vector2<Sint32> CCounter::getPos()
{
	return m_pos;
}