#include "engine\gfx\gui\Component.h"
#include "engine\utils\variable\manager\TextureManager.h"

std::vector<Component::ColorTheme> Component::m_colorThemes = {
	//			BORDER					PRIMARY					SELECT					HOVER					TEXT					TEXT INFO
	ColorTheme(Color(0x101010),		Color(0x414141),		Color(0x818181),		Color(0x818181),		Color(0xF0F0F0),		Color(0x0F0F0F)),	// 0: Primary
	ColorTheme(Color(0x101010),		Color(0x414141),		Color(0x363636),		Color(0x818181),		Color(0xF0F0F0),		Color(0x0F0F0F)),	// 1: Menubar
	ColorTheme(Color(0x444444),		Color(0x727272),		Color(0xBCBCBC),		Color(0x818181),		Color(0xF0F0F0),		Color(0xF0F0F0)),	// 2: Info
	ColorTheme(Color(0x101010),		Color(0x515151),		Color(0x303030),		Color(0x818181),		Color(0xF0F0F0),		Color(0xF0F0F0)),	// 3: Action
	ColorTheme(Color(0x101010),		Color(0xFFFFFF),		Color(0xBBBBBB),		Color(0xDDDDDD),		Color(0x0F0F0F),		Color(0x0F0F0F))	// 4: Action Light
};// 0x007ACC - light blue

Component::Component()
{
	m_colorTheme = m_colorThemes[0];
}
Component::Component(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_colorTheme)
{
	m_compName = p_compName;
	m_title = p_title;
	m_posInit = m_pos = p_pos;
	m_sizeInit = m_size = p_size;
	m_colorTheme = m_colorThemes[p_colorTheme];
}
Component::~Component()
{

}

Component* Component::addComponent(Component* p_comp, Anchor p_posAnchor, Anchor p_sizeAnchor)
{
	return 0;
}
Component* Component::findComponent(std::string p_compName)
{
	return 0;
}
Component* Component::addButton(std::string p_dir, std::string p_buttonName, std::string p_desc, function p_func)
{
	return this;
}
Component* Component::addItem(std::string p_item)
{
	return this;
}
void Component::setList(std::vector<std::string> p_items)
{

}
Uint16 Component::getItemCount()
{
	return 0;
}
Component* Component::setPressFunction(function p_func)
{
	m_pressFunction = p_func;
	return this;
}
Component* Component::setHoldFunction(function p_func)
{
	m_holdFunction = p_func;
	return this;
}
Component* Component::setReleaseFunction(function p_func)
{
	m_releaseFunction = p_func;
	return this;
}
void Component::setSelectedItem(Uint16 p_selectedItem)
{

}
void Component::setSelectedButton(Uint16 p_selectedButton)
{

}
Uint8 Component::isUpdated()
{
	return 0;
}
Sint16 Component::getSelectedItem()
{
	return 0;
}
Sint16 Component::getPrevSelectedItem()
{
	return 0;
}
std::string Component::getName()
{
	return m_compName;
}
std::string Component::getTitle()
{
	return m_title;
}

void Component::setTooltip(std::string p_tooltip)
{
	m_tooltip = p_tooltip;
}
void Component::addTooltip()
{
	if(!GMouse::m_mouseMoved || m_tooltipTime > 0)
	{
		m_tooltipCounted = true;
		m_tooltipTime += GScreen::m_deltaTime;
		if(m_tooltipTime > 0)
			GScreen::m_tooltip = m_tooltip;
	}
	else
		m_tooltipTime = 0;
}
void Component::resetTooltip()
{
	if(!m_tooltipCounted)
		m_tooltipTime = 0;
	m_tooltipCounted = false;
}

void Component::setTitle(std::string p_title)
{
	m_title = p_title;
}
void Component::resize() {}
void Component::setPosition(Vector2<Sint32> p_pos) { m_pos = p_pos; }
void Component::setSize(Vector2<Sint32> p_size) { m_size = p_size; resize(); }
Vector2<Sint32> Component::getInitialPosition() { return m_posInit; }
Vector2<Sint32> Component::getInitialSize() { return m_sizeInit; }
Vector2<Sint32> Component::getPosition() { return m_pos; }
Vector2<Sint32> Component::getSize() { return m_size; }
Vector2<Sint32> Component::getRealPosition() { return m_pos; }
Vector2<Sint32> Component::getRealSize() { return m_size; }
void Component::setState(Sint8 p_selected) { m_selected = p_selected; }

Component* Component::setVisible(bool p_visible)
{
	m_visible = p_visible;
	return this;
}
bool Component::isVisible() { return m_visible; }

void Component::input()
{

}
void Component::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{

}
void Component::update(GLfloat p_deltaUpdate)
{

}
void Component::renderBack()
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		m_colorTheme.m_border.useColor();
		glBegin(GL_QUADS);
		{
			if(m_border & BORDER_TOP)
			{
				glVertex2f(0, 0);
				glVertex2f(GLfloat(m_size.x), 0);
				glVertex2f(GLfloat(m_size.x), -1);
				glVertex2f(0, -1);
			}

			if(m_border & BORDER_RIGHT)
			{
				glVertex2f(GLfloat(m_size.x), 0);
				glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
				glVertex2f(GLfloat(m_size.x) + 1, GLfloat(m_size.y));
				glVertex2f(GLfloat(m_size.x) + 1, 0);
			}

			if(m_border & BORDER_BOTTOM)
			{
				glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
				glVertex2f(0, GLfloat(m_size.y));
				glVertex2f(0, GLfloat(m_size.y) + 1);
				glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y) + 1);
			}

			if(m_border & BORDER_LEFT)
			{
				glVertex2f(0, 0);
				glVertex2f(0, GLfloat(m_size.y));
				glVertex2f(-1, GLfloat(m_size.y));
				glVertex2f(-1, 0);
			}
		}
		glEnd();
	}
	glPopMatrix();
}
void Component::renderFill(bool p_setColor)
{
	glPushMatrix();
	{
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		if(p_setColor)
		{
			if(isSelected())
				m_colorTheme.m_select.useColor();
			else if(m_hovered)
				m_colorTheme.m_hover.useColor();
			else
				m_colorTheme.m_primary.useColor();
		}
		if(m_texture != -1)
			glBindTexture(GL_TEXTURE_2D, MTexture::getInstance().getUnit(m_texture).getId());

		glBegin(GL_QUADS);
		{
			if(m_texture != -1)
			{
				Vector2<Sint32> _texSize;
				if(m_textureStyle == TextureStyle::SCALE)
				{
					glTexCoord2f(0, 0);
					glVertex2f(0, 0);
					glTexCoord2f(1, 0);
					glVertex2f(GLfloat(m_size.x), 0);
					glTexCoord2f(1, 1);
					glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
					glTexCoord2f(0, 1);
					glVertex2f(0, GLfloat(m_size.y));
				}
				else if(m_textureStyle == TextureStyle::WRAP)
				{
					_texSize = MTexture::getInstance().getUnit(m_texture).getSize();
					glTexCoord2f(0, 0);
					glVertex2f(0, 0);
					glTexCoord2f(GLfloat(m_size.x) / _texSize.x, 0);
					glVertex2f(GLfloat(m_size.x), 0);
					glTexCoord2f(GLfloat(m_size.x) / _texSize.x, GLfloat(m_size.y) / _texSize.y);
					glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
					glTexCoord2f(0, GLfloat(m_size.y) / _texSize.y);
					glVertex2f(0, GLfloat(m_size.y));
				}
				else if(m_textureStyle == TextureStyle::SCALE)
				{
					_texSize = MTexture::getInstance().getUnit(m_texture).getSize();
					// Top Left corner
					glTexCoord2f(0, 1);
					glVertex2f(0, 0);
					glTexCoord2f(0.25f, 1);
					glVertex2f(_texSize.x / 4.f, 0);
					glTexCoord2f(0.25f, 0.75f);
					glVertex2f(_texSize.x / 4.f, _texSize.y / 4.f);
					glTexCoord2f(0, 0.75f);
					glVertex2f(0, _texSize.y / 4.f);

					// Top Right corner
					glTexCoord2f(0.75f, 1);
					glVertex2f(GLfloat(m_size.x) - _texSize.x / 4.f, 0);
					glTexCoord2f(1, 1);
					glVertex2f(GLfloat(m_size.x), 0);
					glTexCoord2f(1, 0.75f);
					glVertex2f(GLfloat(m_size.x), _texSize.y / 4.f);
					glTexCoord2f(0.75f, 0.75f);
					glVertex2f(GLfloat(m_size.x) - _texSize.x / 4.f, _texSize.y / 4.f);

					// Bottom Left corner
					glTexCoord2f(0, 0.25f);
					glVertex2f(0, GLfloat(m_size.y) - _texSize.y / 4.f);
					glTexCoord2f(0.25f, 0.25f);
					glVertex2f(_texSize.x / 4.f, GLfloat(m_size.y) - _texSize.y / 4.f);
					glTexCoord2f(0.25f, 0);
					glVertex2f(_texSize.x / 4.f, GLfloat(m_size.y));
					glTexCoord2f(0, 0);
					glVertex2f(0, GLfloat(m_size.y));

					// Bottom Right corner
					glTexCoord2f(0.75f, 0.25f);
					glVertex2f(GLfloat(m_size.x) - _texSize.x / 4.f, GLfloat(m_size.y) - _texSize.y / 4.f);
					glTexCoord2f(1, 0.25f);
					glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y) - _texSize.y / 4.f);
					glTexCoord2f(1, 0);
					glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
					glTexCoord2f(0.75f, 0);
					glVertex2f(GLfloat(m_size.x) - _texSize.x / 4.f, GLfloat(m_size.y));

					GLfloat _width = floor((GLfloat(m_size.x) - _texSize.x / 2.f) / (_texSize.x / 2.f));
					GLfloat _widthF = ((GLfloat(m_size.x) - _texSize.x / 2.f) / (_texSize.x / 2.f));

					// Top and Bottom side
					for(Uint16 i = 0; i < _width; i++)
					{
						glTexCoord2f(0.25f, 1);
						glVertex2f(_texSize.x / 4.f + (i * _texSize.x / 2.f), 0);
						glTexCoord2f(0.75f, 1);
						glVertex2f(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), 0);
						glTexCoord2f(0.75f, 0.75f);
						glVertex2f(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), _texSize.y / 4.f);
						glTexCoord2f(0.25f, 0.75f);
						glVertex2f(_texSize.x / 4.f + (i * _texSize.x / 2.f), _texSize.y / 4.f);

						glTexCoord2f(0.25f, 0.25f);
						glVertex2f(_texSize.x / 4.f + (i * _texSize.x / 2.f), GLfloat(m_size.y) - _texSize.y / 4.f);
						glTexCoord2f(0.75f, 0.25f);
						glVertex2f(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), GLfloat(m_size.y) - _texSize.y / 4.f);
						glTexCoord2f(0.75f, 0);
						glVertex2f(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), GLfloat(m_size.y));
						glTexCoord2f(0.25f, 0);
						glVertex2f(_texSize.x / 4.f + (i * _texSize.x / 2.f), GLfloat(m_size.y));
					}
					glTexCoord2f(0.25f, 1);
					glVertex2f(_texSize.x / 4.f + (_width * _texSize.x / 2.f), 0);
					glTexCoord2f(0.25f + 0.5f * (_widthF - _width), 1);
					glVertex2f(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), 0);
					glTexCoord2f(0.25f + 0.5f * (_widthF - _width), 0.75f);
					glVertex2f(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f);
					glTexCoord2f(0.25f, 0.75f);
					glVertex2f(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f);

					glTexCoord2f(0.25f, 0.25f);
					glVertex2f(_texSize.x / 4.f + (_width * _texSize.x / 2.f), GLfloat(m_size.y) - _texSize.y / 4.f);
					glTexCoord2f(0.25f + 0.5f * (_widthF - _width), 0.25f);
					glVertex2f(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), GLfloat(m_size.y) - _texSize.y / 4.f);
					glTexCoord2f(0.25f + 0.5f * (_widthF - _width), 0);
					glVertex2f(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), GLfloat(m_size.y));
					glTexCoord2f(0.25f, 0);
					glVertex2f(_texSize.x / 4.f + (_width * _texSize.x / 2.f), GLfloat(m_size.y));

					GLfloat _height = floor((GLfloat(m_size.y) - _texSize.y / 2.f) / (_texSize.y / 2.f));
					GLfloat _heightF = ((GLfloat(m_size.y) - _texSize.y / 2.f) / (_texSize.y / 2.f));

					// Left and Right side
					for(Uint16 i = 0; i < _height; i++)
					{
						glTexCoord2f(0, 0.75f);
						glVertex2f(0, _texSize.y / 4.f + (i * _texSize.y / 2.f));
						glTexCoord2f(0, 0.25f);
						glVertex2f(0, (_texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f)));
						glTexCoord2f(0.25f, 0.25f);
						glVertex2f(_texSize.x / 4.f, (_texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f)));
						glTexCoord2f(0.25f, 0.75f);
						glVertex2f(_texSize.x / 4.f, _texSize.y / 4.f + (i * _texSize.y / 2.f));

						glTexCoord2f(0.75f, 0.75f);
						glVertex2f(GLfloat(m_size.x) - _texSize.y / 4.f, _texSize.y / 4.f + (i * _texSize.y / 2.f));
						glTexCoord2f(0.75f, 0.25f);
						glVertex2f(GLfloat(m_size.x) - _texSize.y / 4.f, _texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f));
						glTexCoord2f(1, 0.25f);
						glVertex2f(GLfloat(m_size.x), _texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f));
						glTexCoord2f(1, 0.75f);
						glVertex2f(GLfloat(m_size.x), _texSize.y / 4.f + (i * _texSize.y / 2.f));
					}
					glTexCoord2f(0, 0.75f);
					glVertex2f(0, _texSize.y / 4.f + (_height * _texSize.y / 2.f));
					glTexCoord2f(0, 0.75f - 0.5f * (_heightF - _height));
					glVertex2f(0, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
					glTexCoord2f(0.25f, 0.75f - 0.5f * (_heightF - _height));
					glVertex2f(_texSize.x / 4.f, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
					glTexCoord2f(0.25f, 0.75f);
					glVertex2f(_texSize.x / 4.f, _texSize.y / 4.f + (_height * _texSize.y / 2.f));

					glTexCoord2f(0.75f, 0.75f);
					glVertex2f(GLfloat(m_size.x) - _texSize.x / 4.f, _texSize.y / 4.f + (_height * _texSize.y / 2.f));
					glTexCoord2f(0.75f, 0.75f - 0.5f * (_heightF - _height));
					glVertex2f(GLfloat(m_size.x) - _texSize.x / 4.f, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
					glTexCoord2f(1, 0.75f - 0.5f * (_heightF - _height));
					glVertex2f(GLfloat(m_size.x), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
					glTexCoord2f(1, 0.75f);
					glVertex2f(GLfloat(m_size.x), _texSize.y / 4.f + (_height * _texSize.y / 2.f));

					// Center
					for(Uint16 x = 0; x < _width; x++)
					{
						for(Uint16 y = 0; y < _height; y++)
						{
							glTexCoord2f(0.25f, 0.75f);
							glVertex2f(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
							glTexCoord2f(0.75f, 0.75f);
							glVertex2f(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
							glTexCoord2f(0.75f, 0.25f);
							glVertex2f(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
							glTexCoord2f(0.25f, 0.25f);
							glVertex2f(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));

						}
					}
					for(Uint16 x = 0; x < _width; x++)
					{
						glTexCoord2f(0.25f, 0.75f);
						glVertex2f(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
						glTexCoord2f(0.75f, 0.75f);
						glVertex2f(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
						glTexCoord2f(0.75f, 0.75f - 0.5f * (_heightF - _height));
						glVertex2f(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
						glTexCoord2f(0.25f, 0.75f - 0.5f * (_heightF - _height));
						glVertex2f(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
					}
					for(Uint16 y = 0; y < _height; y++)
					{
						glTexCoord2f(0.25f, 0.75f);
						glVertex2f(_texSize.x / 4.f + (_width * _texSize.y / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
						glTexCoord2f(0.25f + 0.5f * (_widthF - _width), 0.75f);
						glVertex2f(_texSize.x / 4.f + (_width * _texSize.y / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
						glTexCoord2f(0.25f + 0.5f * (_widthF - _width), 0.25f);
						glVertex2f(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.y / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
						glTexCoord2f(0.25f, 0.25f);
						glVertex2f(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.y / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
					}
					glTexCoord2f(0.25f, 0.75f);
					glVertex2f(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
					glTexCoord2f(0.25f + 0.5f * (_widthF - _width), 0.75f);
					glVertex2f(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
					glTexCoord2f(0.25f + 0.5f * (_widthF - _width), 0.75f - 0.5f * (_heightF - _height));
					glVertex2f(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
					glTexCoord2f(0.25f, 0.75f - 0.5f * (_heightF - _height));
					glVertex2f(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
				}
			}
			else
			{
				glVertex2f(0, 0);
				glVertex2f(m_size.x, 0);
				glVertex2f(m_size.x, m_size.y);
				glVertex2f(0, m_size.y);
			}
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
}
void Component::renderBorder()
{
	if(m_border & BORDER_TOP)
	{
		glColor4f(1, 1, 1, 0.1f);
		glVertex2f(0, 0);
		glVertex2f(m_size.x, 0);
		glVertex2f(m_size.x - 2, 2);
		glVertex2f(2, 2);
	}
	if(m_border & BORDER_RIGHT)
	{
		glColor4f(0, 0, 0, 0.1f);
		glVertex2f(m_size.x, 0);
		glVertex2f(m_size.x, m_size.y);
		glVertex2f(m_size.x - 2, m_size.y - 2);
		glVertex2f(m_size.x - 2, 2);
	}
	if(m_border & BORDER_BOTTOM)
	{
		glColor4f(0, 0, 0, 0.1f);
		glVertex2f(0, m_size.y);
		glVertex2f(m_size.x, m_size.y);
		glVertex2f(m_size.x - 2, m_size.y - 2);
		glVertex2f(2, m_size.y - 2);
	}
	if(m_border & BORDER_LEFT)
	{
		glColor4f(1, 1, 1, 0.1f);
		glVertex2f(0, 0);
		glVertex2f(0, m_size.y);
		glVertex2f(2, m_size.y - 2);
		glVertex2f(2, 2);
	}
}
void Component::render()
{
	renderBack();
	renderFill();
	renderBorder();
}
Sint8 Component::isSelected()
{
	return m_selected;
}

void Component::setValue(Sint32 p_value)
{
	*m_numValue = p_value;
}
Sint32* Component::getValue()
{
	return m_numValue;
}
Sint8 Component::getPriorityLayer()
{
	return m_priority + m_moveToFront;
}
Component* Component::setPriorityLayer(Sint8 p_priority)
{
	m_priority = p_priority;
	return this;
}
bool Component::hasList()
{
	return false;
}
