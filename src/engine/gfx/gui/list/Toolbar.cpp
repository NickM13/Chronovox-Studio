#include "engine\gfx\gui\list\Toolbar.h"


CToolbar::CToolbar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
{
	m_compName = p_compName;
	m_pos = p_pos;
	m_size = p_size;
	m_colorTheme = m_colorThemes[MENUBAR];
	m_panelMain = new Panel("", "", {0, 0}, p_size, MENUBAR, BORDER_BOTTOM);
	m_panelSub = new Panel("", "", {0, 0}, {0, 0}, MENUBAR, 0);
}

//Directory splits with '\' 
Component* CToolbar::addButton(std::string p_dir, std::string p_buttonName, std::string p_desc, function p_func)
{
	Uint16 i = 0, j = 0;
	std::vector<std::string> _splitDir;
	if(p_dir != "")
	{
		while(i < p_dir.length())
		{
			if(p_dir[i] == '\\')
			{
				_splitDir.push_back(p_dir.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if(p_dir[p_dir.length() - 1] != '\\')
			_splitDir.push_back(p_dir.substr(j, i));
	}

	SubList* _subList = &m_buttonsMain;
	for(i = 0; i < _splitDir.size(); i++)
		_subList = _subList->find(_splitDir[i]);
	_subList->addButton(p_buttonName, p_desc, p_func);
	return this;
}

void CToolbar::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	Uint16 i = 0, j = 0;
	Sint16 w = 0;
	Sint32 _buttonWidth;
	Sint32 _subWidth, _descWidth;
	if((p_interactFlags & EVENT_MOUSEOVER) && p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & GMouse::MOUSE_PRESS)
	{
		SubList* _subList = &m_buttonsMain;
		std::vector<std::string> _splitDir;
		if(m_selected != "")
		{
			while(i < m_selected.length())
			{
				if(m_selected[i] == '\\')
				{
					_splitDir.push_back(m_selected.substr(j, i));
					j = i + 1;
				}
				i++;
			}
			if(m_selected[m_selected.length() - 1] != '\\')
				_splitDir.push_back(m_selected.substr(j, i));
		}

		for(Uint16 k = 0; k < _splitDir.size(); k++)
			_subList = _subList->find(_splitDir[k]);

		if(_splitDir.size() == 1 && m_currDir == m_selected)
			m_currDir = "";
		else if(_subList != 0 && _subList->getFunction() != 0)
		{
			m_currDir = "";
			_subList->callPressFunction();
			p_interactFlags -= EVENT_MOUSEOVER;
		}
		else
			m_currDir = m_selected;
	}
	m_selected = "";
	if(p_interactFlags & EVENT_MOUSEOVER)
	{
		i = j = 0;
		std::vector<std::string> _splitDir;
		if(m_currDir != "")
		{
			while(i < m_currDir.length())
			{
				if(m_currDir[i] == '\\')
				{
					_splitDir.push_back(m_currDir.substr(j, i));
					j = i + 1;
				}
				i++;
			}
			if(m_currDir[m_currDir.length() - 1] != '\\')
				_splitDir.push_back(m_currDir.substr(j, i));
		}

		w = 0;
		for(i = 0; i < m_buttonsMain.m_buttons.size(); i++)
		{
			_buttonWidth = Font::getMessageWidth(m_buttonsMain.m_buttons[i].m_name).x;
			if(m_currDir != "" && _splitDir[0] == m_buttonsMain.m_buttons[i].m_name)
			{
				std::vector<std::string> _path;
				SubList* _subList = &m_buttonsMain;
				if(_subList != 0)
				{
					for(j = 0; j < _splitDir.size(); j++)
					{
						_path.push_back(_splitDir[j]);
						_subList = _subList->find(_splitDir[j]);
						_subWidth = _buttonWidth;
						_descWidth = 0;
						for(Uint16 k = 0; k < _subList->m_buttons.size(); k++)
							_subWidth = max(_subWidth, Font::getMessageWidth(_subList->m_buttons[k].m_name).x);
						for(Uint16 k = 0; k < _subList->m_buttons.size(); k++)
							_descWidth = max(_descWidth, Font::getMessageWidth(_subList->m_buttons[k].m_desc).x);
						_subWidth += _descWidth + 32;
						for(Uint16 g = 0; g < _subList->m_buttons.size(); g++)
						{
							if(p_mousePos.x - w >= 0 && p_mousePos.x - w < _subWidth &&
								p_mousePos.y - m_size.y >= g * Font::getSpacingHeight() && p_mousePos.y - m_size.y < (g + 1) * Font::getSpacingHeight())
							{
								m_selected = "";
								for(Uint16 h = 0; h < _path.size(); h++)
									m_selected += (_path[h] + "\\");
								m_selected += _subList->m_buttons[g].m_name;
								p_interactFlags -= EVENT_MOUSEOVER;
								break;
							}
						}
					}
				}
			}
			if(p_mousePos.x - w >= 0 && p_mousePos.x - w < Sint32(_buttonWidth + 16) &&
				p_mousePos.y >= 0 && p_mousePos.y < m_size.y)
			{
				m_selected = m_buttonsMain.m_buttons[i].m_name;
				p_interactFlags -= EVENT_MOUSEOVER;
				break;
			}
			w += Sint16(_buttonWidth + 16);
		}
	}
	if(m_currDir != "")
	{
		w = 0;
		for(i = 0; i < m_buttonsMain.m_buttons.size(); i++)
		{
			_buttonWidth = Font::getMessageWidth(m_buttonsMain.m_buttons[i].m_name).x;
			if(p_mousePos.x - w >= 0 && p_mousePos.x - w < Sint32(_buttonWidth + 16) &&
				p_mousePos.y >= 0 && p_mousePos.y < m_size.y)
			{
				m_currDir = m_buttonsMain.m_buttons[i].m_name;
				if(p_interactFlags & EVENT_MOUSEOVER)
					p_interactFlags -= EVENT_MOUSEOVER;
				break;
			}
			w += Sint16(_buttonWidth + 16);
		}
	}
	if((p_interactFlags & EVENT_MOUSEOVER) &&
		p_mousePos.x >= 0 && p_mousePos.x < m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y < m_size.y)
		p_interactFlags -= EVENT_MOUSEOVER;
}
void CToolbar::update(GLfloat p_deltaUpdate)
{
}
void CToolbar::render()
{
	std::vector<std::string> _splitDir, _splitSelect;
	if(m_currDir != "")
	{
		Uint16 i = 0, j = 0;
		while(i < m_currDir.length())
		{
			if(m_currDir[i] == '\\')
			{
				_splitDir.push_back(m_currDir.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if(m_currDir[m_currDir.length() - 1] != '\\')
			_splitDir.push_back(m_currDir.substr(j, i));
	}
	else
		_splitDir.push_back("");

	if(m_selected != "")
	{
		Uint16 i = 0, j = 0;
		while(i < m_selected.length())
		{
			if(m_selected[i] == '\\')
			{
				_splitSelect.push_back(m_selected.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if(m_selected[m_selected.length() - 1] != '\\')
			_splitSelect.push_back(m_selected.substr(j, i));
	}
	else
		_splitSelect.push_back("");

	glPushMatrix();
	{
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		m_panelMain->render();
		glPushMatrix();
		{
			std::string _buttonName;
			Sint32 _buttonWidth;
			Font::setAlignment(ALIGN_LEFT);
			for(Uint16 i = 0; i < m_buttonsMain.m_buttons.size(); i++)
			{
				_buttonName = m_buttonsMain.m_buttons[i].m_name;
				_buttonWidth = Font::getMessageWidth(_buttonName).x + 16;
				if(_splitDir[0] == m_buttonsMain.m_buttons[i].m_name)
				{
					m_colorTheme.m_select.useColor();
					glBegin(GL_QUADS);
					{
						glVertex2f(0, 0);
						glVertex2f(GLfloat(_buttonWidth), 0);
						glVertex2f(GLfloat(_buttonWidth), GLfloat(m_size.y));
						glVertex2f(0, GLfloat(m_size.y));
					}
					glEnd();
				}
				else if(m_selected == m_buttonsMain.m_buttons[i].m_name)
				{
					m_colorTheme.m_hover.useColor();
					glBegin(GL_QUADS);
					{
						glVertex2f(0, 0);
						glVertex2f(GLfloat(_buttonWidth), 0);
						glVertex2f(GLfloat(_buttonWidth), GLfloat(m_size.y));
						glVertex2f(0, GLfloat(m_size.y));
					}
					glEnd();
				}
				m_colorTheme.m_text.useColor();
				Font::setAlignment(Alignment::ALIGN_CENTER);
				Font::print(m_buttonsMain.m_buttons[i].m_name, _buttonWidth / 2, (m_size.y) / 2);

				if(_splitDir[0] == m_buttonsMain.m_buttons[i].m_name)
				{
					Font::setAlignment(Alignment::ALIGN_LEFT);
					SubList* _subList = &m_buttonsMain;
					std::string _subName;
					Sint32 _subWidth, _descWidth;
					for(Uint16 j = 0; j < _splitDir.size(); j++)
					{
						_subList = _subList->find(_splitDir[j]);
						_subName = _subList->m_name;
						_subWidth = _buttonWidth;
						_descWidth = 0;
						for(Uint16 k = 0; k < _subList->m_buttons.size(); k++)
							_subWidth = max(_subWidth, Font::getMessageWidth(_subList->m_buttons[k].m_name).x);
						for(Uint16 k = 0; k < _subList->m_buttons.size(); k++)
							_descWidth = max(_descWidth, Font::getMessageWidth(_subList->m_buttons[k].m_desc).x);
						if(_subList->m_buttons.size() > 0)
						{
							glPushMatrix();
							{
								glTranslatef(GLfloat(j * 92), GLfloat(m_size.y), 0);
								m_panelSub->setState(1);
								m_panelSub->setSize(Vector2<Sint32>(Sint32(_subWidth + _descWidth + 32), Sint32((_subList->m_buttons.size()) * Font::getSpacingHeight())));
								m_panelSub->render();

								m_colorTheme.m_text.useColor();
								for(Uint16 k = 0; k < _subList->m_buttons.size(); k++)
								{
									if((Uint16(_splitDir.size()) > j + 1 && _splitDir[j + 1] == _subList->m_buttons[k].m_name) || (Uint16(_splitSelect.size()) > j + 1 && _splitSelect[j + 1] == _subList->m_buttons[k].m_name))
									{
										m_colorTheme.m_hover.useColor();
										glBegin(GL_QUADS);
										{
											glVertex2f(0, GLfloat(k * Font::getSpacingHeight()));
											glVertex2f(GLfloat(_subWidth + _descWidth + 32), GLfloat(k * Font::getSpacingHeight()));
											glVertex2f(GLfloat(_subWidth + _descWidth + 32), GLfloat((k + 1) * Font::getSpacingHeight()));
											glVertex2f(0, GLfloat((k + 1) * Font::getSpacingHeight()));
										}
										glEnd();
										m_colorTheme.m_text.useColor();
									}
									Font::print(_subList->m_buttons[k].m_name, 8, Sint32((k + 0.5f) * Font::getSpacingHeight()));
									Font::print(_subList->m_buttons[k].m_desc, 24 + _subWidth, Sint32((k + 0.5f) * Font::getSpacingHeight()));
								}
							}
							glPopMatrix();
						}
					}
				}
				glTranslatef(GLfloat(_buttonWidth), 0, 0);
			}
		}
		glPopMatrix();
	}
	glPopMatrix();
}