#include "engine\gfx\gui\button\ButtonToggle.h"

CButtonToggle::CButtonToggle(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
	: Component(p_compName, p_title, p_pos, p_size, ACTION)
{
	m_selected = p_state;
	m_texType = 0;
	m_pressFunction = p_func;

	m_soundClick.setSound(MBuffer::getInstance().getUnit("gui\\Click.wav"));
	m_soundHover.setSound(MBuffer::getInstance().getUnit("gui\\Hover.wav"));

	m_numValue = new Sint32(0);
}
CButtonToggle::CButtonToggle(std::string p_compName, Texture p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
	: Component(p_compName, "", p_pos, p_size, ACTION)
{
	m_selected = p_state;
	m_pressFunction = p_func;

	m_soundClick.setSound(MBuffer::getInstance().getUnit("gui\\Click.wav"));
	m_soundHover.setSound(MBuffer::getInstance().getUnit("gui\\Hover.wav"));

	m_buttonTex[0] = p_buttonTex;
	m_texType = 1;

	m_numValue = new Sint32(0);
}
CButtonToggle::CButtonToggle(std::string p_compName, Texture p_activeTex, Texture p_inactiveTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
	: Component(p_compName, "", p_pos, p_size, ACTION)
{
	m_selected = p_state;
	m_pressFunction = p_func;

	m_soundClick.setSound(MBuffer::getInstance().getUnit("gui\\Click.wav"));
	m_soundHover.setSound(MBuffer::getInstance().getUnit("gui\\Hover.wav"));

	m_buttonTex[0] = p_activeTex;
	m_buttonTex[1] = p_inactiveTex;
	m_texType = 2;

	m_numValue = new Sint32(0);
}

void CButtonToggle::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	*m_numValue = 0;
	if((p_interactFlags & EVENT_MOUSEOVER) &&
		p_mousePos.x >= 0 && p_mousePos.x <= m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y <= m_size.y)
	{
		addTooltip();
		if(!m_hovered)
		{
			m_soundHover.play2d();
			m_hovered = true;
		}
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS)
		{
			*m_numValue = 1;
			m_selected = !m_selected;
			if(m_pressFunction != 0)
			{
				m_soundClick.play2d();
				m_pressFunction();
			}
		}
		p_interactFlags -= EVENT_MOUSEOVER;
	}
	else
	{
		resetTooltip();
	}
}
void CButtonToggle::update(GLfloat p_deltaUpdate)
{

}
void CButtonToggle::render()
{
	glPushMatrix();
	{
		Component::renderBack();
		Component::renderFill();
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);

		glTranslatef(GLfloat(m_size.x / 2), GLfloat(m_size.y / 2), 0);
		if(m_texType != 0)
		{
				glColor3f(1, 1, 1);
			if(m_selected != 0 || m_texType == 1)
				glBindTexture(GL_TEXTURE_2D, m_buttonTex[0].getId());
			else
				glBindTexture(GL_TEXTURE_2D, m_buttonTex[1].getId());
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 1);
				if(m_selected != 0 || m_texType == 1) glVertex2f(-GLfloat(m_buttonTex[0].getSize().x) / 2, -GLfloat(m_buttonTex[0].getSize().y) / 2);
				else glVertex2f(-GLfloat(m_buttonTex[1].getSize().x) / 2, -GLfloat(m_buttonTex[1].getSize().y) / 2);
				glTexCoord2f(1, 1);
				if(m_selected != 0 || m_texType == 1) glVertex2f(GLfloat(m_buttonTex[0].getSize().x) / 2, -GLfloat(m_buttonTex[0].getSize().y) / 2);
				else glVertex2f(GLfloat(m_buttonTex[1].getSize().x) / 2, -GLfloat(m_buttonTex[1].getSize().y) / 2);
				glTexCoord2f(1, 0);
				if(m_selected != 0 || m_texType == 1) glVertex2f(GLfloat(m_buttonTex[0].getSize().x) / 2, GLfloat(m_buttonTex[0].getSize().y) / 2);
				else glVertex2f(GLfloat(m_buttonTex[1].getSize().x) / 2, GLfloat(m_buttonTex[1].getSize().y) / 2);
				glTexCoord2f(0, 0);
				if(m_selected != 0 || m_texType == 1) glVertex2f(-GLfloat(m_buttonTex[0].getSize().x) / 2, GLfloat(m_buttonTex[0].getSize().y) / 2);
				else glVertex2f(-GLfloat(m_buttonTex[1].getSize().x) / 2, GLfloat(m_buttonTex[1].getSize().y) / 2);
			}
			glEnd();
		}
		m_colorTheme.m_text.useColor();
		Font::setAlignment(ALIGN_CENTER);
		Font::print(m_title, 0, 0);
	}
	glPopMatrix();

	m_hovered = false;
}
